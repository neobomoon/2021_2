#lang plai
; [contract] AE
; [purpose] type deine
; [tests] (test (num 3) (num 3))
;         (test (add (num 1) (num 2)) (add (num 1) (num 2)))
;         (test (sub (num 4) (num 1)) (sub (num 4) (num 1)))
;         (test (mul (num 5) (num 2)) (mul (num 5) (num 2)))
;         (test (div (num 10) (num 2)) (div (num 10) (num 2)))
(define-type AE
  [num (n number?)]
  [add (lhs AE?) (rhs AE?)]
  [sub (lhs AE?) (rhs AE?)]
  [mul (lhs AE?) (rhs AE?)]
  [div (lhs AE?) (rhs AE?)])

(test (num 3) (num 3))
(test (add (num 1) (num 2)) (add (num 1) (num 2)))
(test (sub (num 4) (num 1)) (sub (num 4) (num 1)))
(test (mul (num 5) (num 2)) (mul (num 5) (num 2)))
(test (div (num 10) (num 2)) (div (num 10) (num 2)))


; [contract] infix-parse: sexp -> AE
; [purpose] To convert s-expression to AE
; [tests] (test (infix-parse '{3 + 4}) (add (num 3) (num 4)))
;         (test (infix-parse '{2 * 2}) (mul (num 2) (num 2)))
;         (test (infix-parse '{{5 / {3 - 2}} + 5}) (add (div (num 5) (sub (num 3) (num 2))) (num 5)))
;         (test (infix-parse '{{3 - 4} + 7}) (add (sub (num 3) (num 4)) (num 7)))
;         (test/exn (infix-parse '{5 - 1 2}) "infix-parse: bad syntax: (5 - 1 2)")
(define (infix-parse sexp)
  (match sexp
    [(? number?) (num sexp)]
    [(list l '+ r) (add (infix-parse l) (infix-parse r))]
    [(list l '- r) (sub (infix-parse l) (infix-parse r))]
    [(list l '* r) (mul (infix-parse l) (infix-parse r))]
    [(list l '/ r) (div (infix-parse l) (infix-parse r))]
    [else (error 'infix-parse "bad syntax: ~a" sexp)]))

(test (infix-parse '{3 + 4}) (add (num 3) (num 4)))
(test (infix-parse '{2 * 2}) (mul (num 2) (num 2)))
(test (infix-parse '{{5 / {3 - 2}} + 5}) (add (div (num 5) (sub (num 3) (num 2))) (num 5)))
(test (infix-parse '{{3 - 4} + 7}) (add (sub (num 3) (num 4)) (num 7)))
(test/exn (infix-parse '{5 - 1 2}) "infix-parse: bad syntax: (5 - 1 2)")


; [contract] interp: AE -> number
; [purpose] To get results from AE
; [tests] (test (interp (infix-parse '{2 + 1})) 3)
;         (test (interp (infix-parse '{5 * 3})) 15)
;         (test (interp (infix-parse '{{6 * 2} / {4 - 1}})) 4)
;         (test (interp (infix-parse '{3 * {4 + 7}})) 33)
;         (test (interp (infix-parse '{{{81 / 3} / 3} / 3})) 3)
(define (interp ae)
  (type-case AE ae
    [num (n) n]
    [add (l r) (+ (interp l) (interp r))]
    [sub (l r) (- (interp l) (interp r))]
    [mul (l r) (* (interp l) (interp r))]
    [div (l r) (/ (interp l) (interp r))]))

(test (interp (infix-parse '{2 + 1})) 3)
(test (interp (infix-parse '{5 * 3})) 15)
(test (interp (infix-parse '{{6 * 2} / {4 - 1}})) 4)
(test (interp (infix-parse '{3 * {4 + 7}})) 33)
(test (interp (infix-parse '{{{81 / 3} / 3} / 3})) 3)