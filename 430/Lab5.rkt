#lang plai-typed

(print-only-errors #t)

;; Take a number a and returns a function.
;; That function takes a number b and returns a + b
(define (curried-add [a : number]) : (number -> number)
  (lambda ([b : number]) (+ a b)))

;; Test cases
(test ((curried-add 13) 9) 22)
(test ((curried-add 0) 4) 4)

;; Take a function of two arguments and produce a function.
;; That function takes one argument, produces another function
;; that takes one argument and calls the input function.
(define (curry2 function)
  (lambda (arg1) 
    (lambda (arg2) (function arg1 arg2))))

;; Test cases
(test (((curry2 (lambda (a b) (* a b))) 10) 12) 120)
(test (((curry2 (lambda (a b) (append a b))) (list 10)) (list 12)) (list 10 12))

;; Take a function of three arguments and do the same idea
;; as curry2: (a b c -> d) -> (a -> (b -> (c -> d)))
(define (curry3 function)
  (lambda (a)
    (lambda (b)
      (lambda (c) (function a b c)))))

;; Test cases
(test ((((curry3 (lambda (a b c) (+ (* a c) b))) 3) 2) 1) 5)
(test ((((curry3 (lambda (a b c) (+ (* a c) b))) 6) 4) 2) 16)

;; Consumes a list and determines whether a given element exists
(define (contains? [list : (listof symbol)] element) : boolean
  (cond
    [(empty? list) #f]
    [(equal? (first list) element) #t]
    [else (contains? (rest list) element)]))

;; Test cases
(test (contains? (list 'a 'b 'c) 'c) #t)
(test (contains? (list 'a 'b 'c) 'e) #f)
(test (contains? (list) 'e) #f)

;; Consume a list of symbols and query elements and returns
;; a list of booleans indicating what query elements exist in source.
(define (in-list-many? [src : (listof symbol)] [query : (listof symbol)])
  : (listof boolean)
  (cond
    [(empty? query) (list)]
    [else (append (list (contains? src (first query)))
                     (in-list-many? src (rest query)))]))

;; Test cases
(test (in-list-many? (list 'a 'd 'f) (list 'd 'e 'f)) (list #t #f #t))
(test (in-list-many? (list 'a 'b) (list 'b 'a)) (list #t #t))
(test (in-list-many? (list) (list 'b 'a)) (list #f #f))