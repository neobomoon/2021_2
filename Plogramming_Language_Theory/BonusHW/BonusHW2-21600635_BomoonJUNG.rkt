#lang plai
; declare type
(define-type RFAE
  [num (n number?)]
  [add (lhs RFAE?) (rhs RFAE?)]
  [sub (lhs RFAE?) (rhs RFAE?)]
  [mul (lhs RFAE?) (rhs RFAE?)]
  [id (name symbol?)]
  [fun (param symbol?) (body RFAE?)]
  [app (ftn RFAE?) (arg RFAE?)]
  [if0 (test-expr RFAE?)
       (then-expr RFAE?)
       (else-expr RFAE?)])

(define-type RFAE-Value
  [numV (n number?)]
  [closureV (param symbol?) (body RFAE?) (ds DefrdSub?)])


(define-type DefrdSub
  [mtSub]
  [aSub (name symbol?) ; name
        (value RFAE-Value?) ; value
        (ds DefrdSub?)])


; [contract] parse: sexpr->RFAE
; [purpose] To convert sexp into RFAE
(define (parse sexp)
  (match sexp
    [(? number?) (num sexp)]
    [(list '+ l r) (add (parse l) (parse r))]
    [(list '- l r) (sub (parse l) (parse r))]
    [(list '* l r) (mul (parse l) (parse r))]
    [(list 'with (list i v) e) (app (fun i (parse e)) (parse v))]
    [(? symbol?) (id sexp)]
    [(list 'fun (list p) b) (fun p (parse b))]
    [(list f a) (app (parse f) (parse a))]
    [(list 'if0 test then else) (if0 (parse test) (parse then) (parse else))]
    [else (error 'parse "bad syntax: ~a" sexp)]))


; [contract] interp: RFAE -> RFAE-Value
; [purpose] To convert sexp into RFAE-Value
(define (interp rfae ds)
  (type-case RFAE rfae
    [num (n) (numV n)]
    [add (l r) (num+ (interp l ds) (interp r ds))]
    [sub (l r) (num- (interp l ds) (interp r ds))]
    [mul (l r) (num* (interp l ds) (interp r ds))]
    [id (s) (lookup s ds)]
    [fun (p b) (closureV p b ds)]
    [app (f a) (local [(define f-val (interp f ds))
                       (define a-val (interp a ds))]; check 'a' and if there is free -> use just ds
                 (interp (closureV-body f-val)
                         (aSub (closureV-param f-val)
                               a-val
                               (type-case DefrdSub (closureV-ds f-val)
                                 [mtSub () ds]
                                 [aSub (name value ds) (closureV-ds f-val)]))))] ;(closureV-ds f-val) is normal
    [if0 (test then else) (if (numzero? (interp test ds))
                              (interp then ds)
                              (interp else ds))]))


; num-op: (number number -> number) -> (RFAE RFAE -> RFAE)
(define (num-op op) (lambda (x y) (numV (op (numV-n x) (numV-n y)))))
(define num+ (num-op +))
(define num- (num-op -))
(define num* (num-op *))


; [contract] numzero? RFAE-Value -> boolean
; [purpose] To check zero
(define (numzero? n)
  (zero? (numV-n n)))


; [contract] lookup: symbol -> RFAE-Value
; [purpose] To look up value
(define (lookup name ds)
  (type-case DefrdSub ds
    [mtSub () (error 'lookup "free identifier")]
    [aSub (i v saved) (if (symbol=? i name)
                          v
                          (lookup name saved))]))

; run
(define (run sexp ds)
  (interp (parse sexp) ds))


;Example 1
(run '{with {count {fun {n} {if0 n 0 {+ 1 {count {- n 1}}}}}}
         {count 8}} (mtSub))

;Example 2
(run '{with {fac {fun {n} {if0 n 1 {* n {fac {- n 1}}}}}}
        {fac 10}} (mtSub))

;Example 3
(run '{with {count {fun {n} 0}}
         {with {count {fun {n} {if0 n 0 {+ 1 {count {- n 1}}}}}}
                  {count 8}}} (mtSub))


