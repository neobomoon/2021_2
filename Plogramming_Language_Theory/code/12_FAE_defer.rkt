#lang plai
; declare type
(define-type FAE
  [num (n number?)]
  [add (lhs FAE?) (rhs FAE?)]
  [sub (lhs FAE?) (rhs FAE?)]
  [id (name symbol?)]
  [fun (param symbol?) (body FAE?)]
  [app (ftn FAE?) (arg FAE?)])

(define-type FAE-Value
  [numV (n number?)]
  [closureV (param symbol?) (body FAE?) (ds DefrdSub?)])


(define-type DefrdSub
  [mtSub]
  [aSub (name symbol?) ; name
        (value FAE-Value?) ; value
        (ds DefrdSub?)])

; parse
(define (parse sexp)
  (match sexp
    [(? number?) (num sexp)]
    [(list '+ l r) (add (parse l) (parse r))]
    [(list '- l r) (add (parse l) (parse r))]
    [(list 'with (list i v) e) (app (fun i (parse e)) (parse v))]
    [(? symbol?) (id sexp)]
    [(list 'fun (list p) b) (fun p (parse b))] ; ex) (fun {x} {+ 1 x})
    [(list f a) (app (parse f) (parse a))]
    [else (error 'parse "bad syntax: ~a" sexp)]))

; interp: WAE DefrdSub -> FAE-Value
(define (interp fae ds)
  (type-case FAE fae
    [num (n) (numV n)]
    [add (l r) (num+ (interp l ds) (interp r ds))] ; [add (l r) (+ (interp l ds) (interp r ds))]
    [sub (l r) (num- (interp l ds) (interp r ds))]
    [id (s) (lookup s ds)]
    [fun (p b) (closureV p b ds)]
    [app (f a) (local [(define f-val (interp f ds))
                       (define a-val (interp a ds))]
                 (interp (closureV-body f-val)
                         (aSub (closureV-param f-val)
                               a-val
                               (closureV-ds f-val))))]))

; num-op: (number number -> number) -> (FAE FAE -> FAE)
(define (num-op op) (lambda (x y) (numV (op (numV-n x) (numV-n y)))))
(define num+ (num-op +))
(define num- (num-op -))


; lookup
(define (lookup name ds)
  (type-case DefrdSub ds
    [mtSub () (error 'lookup "free identifier")]
    [aSub (i v saved) (if (symbol=? i name)
                          v
                          (lookup name saved))]))


(interp (parse '{with {x 3} {+ x x}}) (mtSub)) ; change with -> app
(parse '{with {z {fun {x} {+ x y}}} {with {y 10} z}})
(parse '{with {x 3} {with {f {fun {y} {+ x y}}} {with {x 5} {f 4}}}})
(parse '{with {y 10} {fun {x} {+ x y}}})
(mtSub)
(interp (parse '{with {x 3} {with {f {fun {y} {+ x y}}} {with {x 5} {f 4}}}}) (mtSub))
(parse '{with {y 10} {fun {x} {+ y x}}})
(parse '{fun {x} {+ 1 y}})
(parse '{with {x 3} {with {x 5} {f 4}}})
(parse '{with {x 5} {f 4}})
(parse '{with {y 10} {fun {x} {+ y x}}})
(interp (parse '{with {y 10} {fun {x} {+ y x}}}) (mtSub))
(interp (parse '{fun {x} {+ y x}}) (mtSub))
(interp (parse '{+ 2 4}) (mtSub))
(interp (parse '{+ 4 {with {x 4} {+ x 1}}}) (mtSub))
(parse '{+ 4 {with {x 4} {+ x 1}}})
(interp (parse 4) (mtSub))
