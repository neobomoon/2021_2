#lang plai
; declare type
(define-type RCFAE
  [num (n number?)]
  [add (lhs RCFAE?) (rhs RCFAE?)]
  [sub (lhs RCFAE?) (rhs RCFAE?)]
  [id (name symbol?)]
  [fun (param symbol?) (body RCFAE?)]
  [app (fun-expr RCFAE?) (arg-expr RCFAE?)]
  [if0 (test-expr RCFAE?)
       (then-expr RCFAE?) (else-expr RCFAE?)]
  [rec (name symbol?) (named-expr RCFAE?) (fst-all RCFAE?)])

(define-type RCFAE-Value
  [numV (n number?)]
  [closureV (param symbol?) (body RCFAE?) (ds DefrdSub?)]
  [exprV (expr RCFAE?) (ds DefrdSub?) (value (box/c (or/c false RCFAE-Value?)))])

(define-type DefrdSub
  [mtSub]
  [aSub (name symbol?)
        (value RCFAE-Value?)
        (ds DefrdSub?)]
  [aRecSub (name symbol?)
           (value-box (box/c RCFAE-Value?))
           (ds DefrdSub?)])


; parse
(define (parse sexp)
  (match sexp
    [(? number?) (num sexp)]
    [(list '+ l r) (add (parse l) (parse r))]
    [(list '- l r) (sub (parse l) (parse r))]
    [(list 'with (list i v) e) (app (fun i (parse e)) (parse v))]
    [(? symbol?) (id sexp)]
    [(list 'fun (list p) b) (fun p (parse b))]
    [(list f a) (app (parse f) (parse a))]
    [(list 'if0 test then else) (if0 (parse test) (parse then) (parse else))]
    [(list 'rec (list name named-expr) fst-call) (rec name (parse named-expr) (parse fst-call))]
    [else (error 'parse "bad syntax: ~a" sexp)]))


; interp
(define (interp rcfae ds)
  (type-case RCFAE rcfae
    [num (n) (numV n)]
    [add (l r) (num+ (interp l ds) (interp r ds))]
    [sub (l r) (num- (interp l ds) (interp r ds))]
    [id (s) (lookup s ds)]
    [fun (p b) (closureV p b ds)]
    ; [dsfun (p b) (closureV p b ds)]
    [app (f a) (local [(define f-val (strict (interp f ds)))
                       (define a-val (exprV a ds (box #f)))]
                 (interp (closureV-body f-val)
                         (aSub (closureV-param f-val)
                               a-val
                               (closureV-ds f-val))))]
    [if0 (test then else) (if (numzero? (interp test ds))
                              (interp then ds)
                              (interp else ds))]
    [rec (bound-id named-expr fst-call)
      (local [(define value-holder (box (numV 198))) ; dummy value
              (define new-ds (aRecSub bound-id value-holder ds))]
        (begin
          (set-box! value-holder (interp named-expr new-ds))
          (interp fst-call new-ds)))]))

; lookup
(define (lookup name ds)
  (type-case DefrdSub ds
    [mtSub () (error 'lookup "free variable")]
    [aSub (sub-name val rest-ds) (if (symbol=? sub-name name)
                                     (strict val)
                                     (lookup name rest-ds))]
    [aRecSub (sub-name val-box rest-ds) (if (symbol=? sub-name name)
                                            (unbox val-box)
                                            (lookup name rest-ds))]))

; strict
(define (strict v)
  (type-case RCFAE-Value v
    [exprV (expr ds v-box)
           (if (not (unbox v-box))
               (local [(define v (strict (interp expr ds)))]
                 (begin (set-box! v-box v)
                        v))
               (unbox v-box))]
    [else v]))

; num-op
(define (num-op op) (lambda (x y) (numV (op (numV-n (strict x))
                                         (numV-n (strict y))))))
(define num+ (num-op +))
(define num- (num-op -))
(define (numzero? n)
  (zero? (numV-n n)))

; run
(define (run sexp ds)
  (interp (parse sexp) ds))

; example
(run '{rec {count {fun {n} {if0 n 0 {+ 1 {count {- n 1}}}}}} {count 8}} (mtSub))
