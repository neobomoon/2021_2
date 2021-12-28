#lang plai

; Problem 1: (0.5) Write the function dollar->won, which comsumes an integer number(dollar) and returns the integer(won). Suppose that 1 dollar is converted as 1,162 Korean won
; Solved by myself: Y
; Time taken: 3 min
; [contract] dollar->won: number->number
; [purpose] To convert dollar to won
; [tests] (test (dollar->won 1) 1162)
;         (test (dollar->won 2) 2324)
(define (dollar->won a)
  (* a 1162))

(test (dollar->won 1) 1162)
(test (dollar->won 2) 2324)


; Problem 2: (0.5) Write the funcion digit_sum, which consumes three integer numbers and returns the sum of them.
; Solved by myself: Y
; Time taken: 3
; [contract] digit_sum: number->number
; [purpose] To sum three integer numbers
; [tests] (test (digit_sum 1 3 5) 9)
;         (test (digit_sum 2 2 3) 7)
(define (digit_sum a b c)
  (+ a b c))

(test (digit_sum 1 3 5) 9)
(test (digit_sum 2 2 3) 7)


; Problem 3: (0.5) An inchworm can travel n inches per one hour(speed). Write the function inchworm-travel, which consumes two integer numbers that denote speed n, and hours, and returns the distance an Inchworm can travel in that time in centimeters.
; Assume that an inch is 2.54 centimeters.
; Solved by myself: Y
; Time taken: 3
; [contract] inchworm-define: number -> number
; [purpose] To get the distance traveled to centimeters
; [tests] (test (inchworm-define 2 2) 10.16)
;         (test (inchworm-define 1 3) 7.62)
(define (inchworm-define n h)
  (* n h 2.54))

(test (inchworm-define 2 2) 10.16)
(test (inchworm-define 1 3) 7.62)


; Problem 4: (0.5) Write the function is-odd?, which consumes an integer number and returns whether the number is odd.
; Solved by myself: Y
; Time taken: 3
; [contract] is-odd?: number -> boolean
; [purpose] To check number is odd
; [tests] (test (is-odd? 3) true)
;         (test (is-odd? 2) false)
(define (is-odd? a)
  (= 1 (modulo a 2)))

(test (is-odd? 3) true)
(test (is-odd? 2) false)


; Problem 5: Write the function combination, which consumes two integer numbers and returns the number of combinations that can be there,
; when first function parameter is n (which is the number of elements in the set), and second function parameter is k (which is the number of elements in each subset).
; Implement the factorial function as well. (Do not use a built-in function in Racket.)
; Solved by myself: Y
; Time taken: 5
; [contract] factorial: number -> number
; [purpose] To caculate permutation
; [tests] (test (factorial 5) 120)
;         (test (factorial 0) 1)
(define (factorial a) ; recursion function
  (cond
    [ (= a 0) 1 ] ; case 0!
    [ (= a 1) a ] ; stop and return
    [ (> a 1) (* a (factorial (- a 1))) ] ))

(test (factorial 5) 120)
(test (factorial 0) 1)

; Solved by myself: Y
; Time taken: 5
; [contract] combination: number -> number
; [purpose] To caculate combination
; [test] (test (combination 8 3) 56)
;        (test (combination 1 3) 0)
(define (combination n k)
  (cond
    [ (>= n k) (/ (factorial n) (* (factorial k) (factorial (- n k)))) ]
    [ (< n k) 0 ] ))

(test (combination 8 3) 56)
(test (combination 1 3) 0)


; Problem 6:
; a. (0.5) Define the type PERSON, which is Professor,  UndergraduateStudent, or GraduateStudent.
; Professor has two attributes: 'courses' for the number of courses taught and 'projects' for the number of research projects conducted.
; UndergraduateStudent has one attribute: 'courses' for the number of courses taken.
; GraduateStudent has two attributes: 'courses' and 'papers' for the number of papers submitted.
; b. (0.5) Define the function have-courses, which consumes any person instance and produces the nummmber of courses taken or taught for the given person.
; c. (0.5) Define the function ready-to-graduate, which consumes a person. If a given person is a GraduateStudent with more than or equal to three papers, then return true, otherwise produce false.
; Solved by myself: Y
; Time taken: 5
; [contract] PERSON
; [purpose] To define type
; [tests] (define professor (Professor 4 8))
;         (define student1 (UndergraduateStudent 2))
;         (define student2 (GraduateStudent 4 3))
;         (define student3 (GraduateStudent 2 2))

(define-type PERSON
  [ Professor (courses number?)
              (projects number?) ]
  [ UndergraduateStudent (courses number?) ]
  [ GraduateStudent (courses number?)
                    (papers number?) ] )

(define professor (Professor 4 8))
(define student1 (UndergraduateStudent 2))
(define student2 (GraduateStudent 4 3))
(define student3 (GraduateStudent 2 2))

; Solved by myself: Y
; Time taken: 7
; [contract] have-courses: PERSON->number
; [purpose] To return number of taken courses
; [tests] (test (have-courses professor) 4)
;         (test (have-courses student1) 2)
;         (test (have-courses student2) 4)
(define (have-courses person)
  (cond
    [ (Professor? person) (Professor-courses person) ]
    [ (UndergraduateStudent? person) (UndergraduateStudent-courses person) ]
    [ (GraduateStudent? person) (GraduateStudent-courses person) ] ))

(test (have-courses professor) 4)
(test (have-courses student1) 2)
(test (have-courses student2) 4)

; Solved by myself: Y
; Time taken: 5
; [contract] ready-to-graduate: PERSON->boolean
; [purpose] To check whether can graduate or not
; [tests] (test (ready-to-graduate student2) true)
;         (test (ready-to-graduate student3) false)
(define (ready-to-graduate person)
  (cond
    [ (GraduateStudent? person) (>= (GraduateStudent-papers person) 3) ]
    [ else false ] ))

(test (ready-to-graduate student2) true)
(test (ready-to-graduate student3) false)


; Problem 7: (1.0) Define the function name-alphabet, which consumes a list of alphabets and produces a corresponding list of an alphabetical character
; with names starting with the alphabet character; it names all occurrences of 'a with 'alice, 'c with 'cherry, 'j with 'jc, 'k with 'kate and keeps the other
; characters as unnamed (either 'unnamed or empty '()' is ok).
; '(a b n) => '(alice unnamed unnamed)
; Solved by myself: Y
; Time taken: 20
; [contract] name-alphabet: list -> list
; [purpose] To change input names as condition
; [tests] (test (name-alphabet '(a c j k n)) '(alice cherry jc kate unnamed))
;         (test (name-alphabet '(a b c d e)) '(alice unnamed cherry unnamed unnamed))
(define (name-alphabet list_of_alphabet)
  (cond
    [ (empty? list_of_alphabet) empty]
    [ (symbol=? 'a (first list_of_alphabet)) (append '(alice) (name-alphabet (rest list_of_alphabet))) ]
    [ (symbol=? 'c (first list_of_alphabet)) (append '(cherry) (name-alphabet (rest list_of_alphabet))) ]
    [ (symbol=? 'j (first list_of_alphabet)) (append '(jc) (name-alphabet (rest list_of_alphabet))) ]
    [ (symbol=? 'k (first list_of_alphabet)) (append '(kate) (name-alphabet (rest list_of_alphabet))) ]
    [ else (append '(unnamed) (name-alphabet (rest list_of_alphabet))) ] ))

(test (name-alphabet '(a c j k n)) '(alice cherry jc kate unnamed))
(test (name-alphabet '(a b c d e)) '(alice unnamed cherry unnamed unnamed))


; Problem 8: (1.0) Define the function update-name. The new function consumes two symbols, called old and new,
; and a list of symbols. It produces a list of symbols by replacing all occurrences of old by new. For example,
; (update-name 'cherry 'claire (cons 'jc (cons 'cherry (cons 'kate empty))))
;       produces
; '(jc claire kate)
; Solved by myself: Y
; Time taken: 30
; [contract] update-name: list -> list
; [purpose] To change old name to new name
; [tests] (test (update-name 'cherry 'claire (cons 'jc (cons 'cherry (cons 'kate empty)))) '(jc claire kate))
;         (test (update-name 'lee 'jung '(lee bo moon)) '(jung bo moon))
(define (update-name old new list_symbol)
  (cond
    [ (empty? list_symbol) empty]
    [ (symbol=? old (first list_symbol)) (append (list new) (update-name old new (rest list_symbol))) ]
    [ else (append (list (first list_symbol)) (update-name old new (rest list_symbol))) ] ))

(test (update-name 'cherry 'claire (cons 'jc (cons 'cherry (cons 'kate empty)))) '(jc claire kate))
(test (update-name 'lee 'jung '(lee bo moon)) '(jung bo moon))