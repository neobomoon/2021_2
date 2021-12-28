#lang plai
(define-type AE
  [num (n number?)]
  [add (lhs AE?) (rhs AE?)]
  [sub (lhs AE?) (rhs AE?)])

; [contract] infix-parse: sexp -> AE
; [purpose] To convert s-expression to AE
; [tests]
(define (infix-parse sexp)
  (match sexp
    [(? number?) (num sexp)]
    [(list l + r) ((infix-parse l) add (infix-parse r))]
    [(list l - r) ((infix-parse l) sub (infix-parse r))]
    [(list l * r) ((infix-parse l) mul (infix-parse r))]
    [(list l / r) ((infix-parse l) div (infix-parse r))]
    [else (error 'infix-parse "bad syntax ~a" sexp)]))


(test (infix-parse '3) (num 3))
(test (parse '{3 + 4}) ((num 3) add (num 4)))
(test (parse '{{3 - 4} + 7}) (((num 3) sub (num 4)) add (num 7)))
(test/exn (parse '{ 5 - 1 2}) "parse: bad syntax: (5 - 1 2)")

; interp: AE -> number
; to get results from AE
(define (interp an-ae)
  (type-case AE an-ae
    [num (n) n]
    [add (l r) (+ (interp l) (interp r))]
    [sub (l r) (- (interp l) (interp r))])
)

;(test (interp (parse '3)) 3)
;(test (interp (parse '{+ 3 4})) 7)
;(test (interp (parse '{+ {- 3 4} 7})) 6)