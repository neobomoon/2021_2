#lang plai
; declare type
(define-type LFAE
  [num (n number?)]
  [add (lhs LFAE?) (rhs LFAE?)]
  [sub (lhs LFAE?) (rhs LFAE?)]
  [id (name symbol?)]
  [fun (param symbol?) (body LFAE?)]
  [app (ftn LFAE?) (arg LFAE?)])

(define-type LFAE-Value
  [numV (n number?)]
  [closureV (param symbol?) (body LFAE?) (ds DefrdSub?)]
  [exprV (expr LFAE?) (ds DefrdSub?) (value (box/c (or/c false LFAE-Value?)))])

(define-type DefrdSub
  [mtSub]
  [aSub (name symbol?) ; name
        (value LFAE-Value?) ; value
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

; strict
(define (strict v)
  (type-case LFAE-Value v
    [exprV (expr ds v-box)
           (if (not (unbox v-box))
               (local [(define v (strict (interp expr ds)))]
                 (begin (set-box! v-box v)
                        v))
               (unbox v-box))]
    [else v]))

; num-op: (number number -> number) -> (FAE FAE -> FAE)
(define (num-op op) (lambda (x y) (numV (op (numV-n (strict x))
                                            (numV-n (strict y))))))
(define num+ (num-op +))
(define num- (num-op -))

;lookup
(define (lookup name ds)
  (type-case DefrdSub ds
    [mtSub () (error 'lookup "free identifier")]
    [aSub (i v saved) (if (symbol=? i name)
                          (strict v)
                          (lookup name saved))]))

; interp: WAE DefrdSub -> FAE-Value
(define (interp lfae ds)
  (type-case LFAE lfae
    [num (n) (numV n)]
    [add (l r) (num+ (interp l ds) (interp r ds))]
    [sub (l r) (num- (interp l ds) (interp r ds))]
    [id (s) (lookup s ds)]
    [fun (p b) (closureV p b ds)]
    ;[dsfun (p b) (clouserV p b ds)]
    [app (f a) (local [(define f-val (strict (interp f ds))); (app (fun i (parse e)) (parse v))]
                       (define a-val (exprV a ds (box #f)))]
                 (interp (closureV-body f-val)
                         (aSub (closureV-param f-val)
                               a-val
                               (closureV-ds f-val))))]))

; (interp (parse '{{fun {x} x} {+ 1 {fun {y} 2}}}) (mtSub))
;(interp (parse '{with {y 10} {fun {x} {+ y x}}}) (mtSub))
(interp (parse '{with {x 3} {with {f {fun {y} {+ x y}}} {with {x 5} {f 4}}}}) (mtSub))
(parse '{{fun {x} x} {+ 1 {fun {y} 2}}})
(parse '{with {x 3} {with {f {fun {y} {+ x y}}} {with {x 5} {f 4}}}})
(interp (parse '{with {y 10} {fun {x} {+ x y}}}) (mtSub))
(interp (parse '{with {y 10} {fun {x} {+ y x}}}) (mtSub))
(interp (parse '{{fun {f} {f 1}} {fun {x} x}}) (mtSub))
(parse '{{fun {f} {f 1}} {fun {x} {+ x 1}}})
