#lang plai
;run
(define (run sexp ds st)
  (interp (parse sexp) ds st))

; declare type
(define-type RBMFAE
  [num (n number?)]
  [add (lhs RBMFAE?) (rhs RBMFAE?)]
  [sub (lhs RBMFAE?) (rhs RBMFAE?)]
  [id (name symbol?)]
  [fun (param symbol?) (body RBMFAE?)]
  [refun (param symbol?) (body RBMFAE?)]
  [newbox (v RBMFAE?)]
  [setbox (bn RBMFAE?) (v RBMFAE?)]
  [openbox (v RBMFAE?)]
  [seqn (ex1 RBMFAE?) (ex2 RBMFAE?)]
  [app (ftn RBMFAE?) (arg RBMFAE?)]
  [setvar (id symbol?) (ex RBMFAE?)])

(define-type RBMFAE-Value
  [numV (n number?)]
  [closureV (param symbol?) (body RBMFAE?) (ds DefrdSub?)]
  [refclosV (param symbol?) (body RBMFAE?) (ds DefrdSub?)]
  [boxV (address integer?)])

(define-type DefrdSub
  [mtSub]
  [aSub (name symbol?) (address integer?) (ds DefrdSub?)])

(define-type Store
  [mtSto]
  [aSto (address integer?) (value RBMFAE-Value?) (rest Store?)])

(define-type Value*Store
  [v*s (value RBMFAE-Value?) (store Store?)])

; parse
(define (parse sexp)
  (match sexp
    [(? number?) (num sexp)]
    [(list '+ l r) (add (parse l) (parse r))]
    [(list '- l r) (add (parse l) (parse r))]
    [(list 'with (list i v) e) (app (fun i (parse e)) (parse v))]
    [(? symbol?) (id sexp)]
    [(list 'newbox v) (newbox (parse v))]
    [(list 'setbox i v) (setbox (parse i) (parse v))]
    [(list 'openbox i) (openbox (parse i))]
    [(list 'seqn ex1 ex2) (seqn (parse ex1) (parse ex2))]
    [(list 'fun (list p) b) (fun p (parse b))] ; ex) (fun {x} {+ 1 x})
    [(list 'refun (list p) b) (refun p (parse b))]
    [(list f a) (app (parse f) (parse a))]
    [(list 'setvar n v) (setvar n (parse v))]
    [else (error 'parse "bad syntax: ~a" sexp)]))

; interp: RBMFAE DefrdSub Store -> Value*Store
(define (interp expr ds st)
  (type-case RBMFAE expr
    [num (n) (v*s (numV n) st)]
    [add (l r) (type-case Value*Store (interp l ds st)
                 (v*s (l-value l-store)
                      (type-case Value*Store (interp r ds l-store)
                        (v*s (r-value r-store)
                             (v*s (num+ l-value r-value)
                                  r-store)))))]
    (sub (l r) (type-case Value*Store (interp l ds st)
                 (v*s (l-value l-store)
                      (type-case Value*Store (interp r ds l-store)
                        (v*s (r-value r-store)
                             (v*s (num- l-value r-value)
                                  r-store))))))
    [id (s) (v*s (store-lookup (lookup s ds) st) st)]
    [newbox (val) (type-case Value*Store (interp val ds st)
                    [v*s (vl st1)
                         (local [(define a (malloc st1))]
                           (v*s (boxV a)
                                (aSto a vl st1)))])]
    [setbox (bx-expr val-expr)
            (type-case Value*Store (interp bx-expr ds st)
              [v*s (bx-val st2)
                   (type-case Value*Store (interp val-expr ds st2)
                     [v*s (val st3)
                          (v*s val
                               (aSto (boxV-address bx-val)
                                     val
                                     st3))])])]
    [openbox (bx-expr)
             (type-case Value*Store (interp bx-expr ds st)
                        [v*s (bx-val st1)
                             (v*s (store-lookup (boxV-address bx-val)
                                                st1)
                                  st1)])]
    [seqn (a b) (type-case Value*Store (interp a ds st)
                  [v*s (a-value a-store)
                       (interp b ds a-store)])]
    [fun (p b) (v*s (closureV p b ds) st)]
    [refun (p b) (v*s (refclosV p b ds) st)]
    [app (f a) (type-case Value*Store (interp f ds st)
                 [v*s (f-value f-store)
                      (type-case RBMFAE-Value f-value
                        [closureV (c-param c-body c-ds)
                                  (type-case Value*Store (interp a ds f-store)
                                    [v*s (a-value a-store)
                                         (local ([define new-address (malloc a-store)])
                                           (interp c-body
                                                   (aSub c-param
                                                         new-address
                                                         c-ds)
                                                   (aSto new-address
                                                         a-value
                                                         a-store)))])]

                        [refclosV (rc-param rc-body rc-ds)
                                  (if (id? a)
                                      (type-case Value*Store (interp a ds f-store)
                                        [v*s (a-value a-store)
                                             (local [(define address (lookup (id-name a) ds))]
                                               (interp rc-body
                                                       (aSub rc-param
                                                             address
                                                             rc-ds)
                                                       (aSto address
                                                             a-value
                                                             a-store)))])
                                      (error interp "not identifier"))]
                        [else (error interp "trying to apply a number")])])]
    [setvar (id val-expr) (local [(define a (lookup id ds))]
                            (type-case Value*Store (interp val-expr ds st)
                              [v*s (val st) (v*s val
                                                 (aSto a
                                                       val
                                                       st))]))]))
                                       

; num-op: (number number -> number) -> (FAE FAE -> FAE)
(define (num-op op) (lambda (x y) (numV (op (numV-n x) (numV-n y)))))
(define num+ (num-op +))
(define num- (num-op -))

; malloc: Store -> Interger
(define (malloc st)
  (+ 1 (max-address st)))

; max-address: Store -> Integer
(define (max-address st)
  (type-case Store st
    [mtSto () 0]
    [aSto (n v st)
          (max n (max-address st))]))

; store-lookup
(define (store-lookup address sto)
  (type-case Store sto
    [mtSto () (error 'store-lookup "No value at address")]
    [aSto (location value rest-store)
          (if (= location address)
              value
              (store-lookup address rest-store))]))
 

; lookup
(define (lookup name ds)
  (type-case DefrdSub ds
    [mtSub () (error 'lookup "free identifier")]
    [aSub (i v saved) (if (symbol=? i name)
                          v
                          (lookup name saved))]))

; example
(run '{with {a 7} a} (mtSub) (mtSto))
(run '{with {a 3} {setvar a 5}} (mtSub) (mtSto))
(run '{with {a 3} {seqn {{fun {x} {setvar x 5}} a} a}} (mtSub) (mtSto))
(run '{with {a 3} {seqn {{refun {x} {setvar x 5}} a} a}} (mtSub) (mtSto))
(run '{with {a 3} {seqn {{fun {x} x} a} a}} (mtSub) (mtSto))
(run '{with {swap {refun {x}
                       {refun {y}
                            {with {z x}
                                  {seqn {setvar x y}
                                        {setvar y z}}}}}}
            {with {a 10}
                  {with {b 20}
                        {seqn {{swap a} b}
                              b}}}} (mtSub) (mtSto))
(run '{with {swap {refun {x}
                       {refun {y}
                            {with {z x}
                                  {seqn {setvar x y}
                                        {setvar y z}}}}}}
            {with {a 10}
                  {with {b 20}
                        {seqn {{swap a} b}
                              a}}}} (mtSub) (mtSto))