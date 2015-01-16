#lang plai-typed

(require plai-typed/s-exp-match)

;; AE Language from Chapter 3
(define-type ArithC
  [numC (n : number)]
  [plusC (l : ArithC) (r : ArithC)]
  [multC (l : ArithC) (r : ArithC)])

;; Evaluation for ArithC Expressions
(define (eval [exp : ArithC]) : number
  (type-case ArithC exp
    [numC (n) n]
    [plusC (l r) (+ (eval l) (eval r))]
    [multC (l r) (* (eval l) (eval r))]))

;; Takes an AE and returns how many numbers it contains
(define (num-nums [exp : ArithC]) : number
  (type-case ArithC exp
    [numC (n) 1]
    [plusC (l r) (+ (num-nums l) (num-nums r))]
    [multC (l r) (+ (num-nums l) (num-nums r))]))

;; Test cases
(test (num-nums (plusC (numC 10) (multC (numC 5) (plusC (numC 3) (numC 3))))) 4)

;; Parser for the AE Language
(define (ae-parse [expr : s-expression]) : ArithC
  (cond
    [(s-exp-match? `NUMBER expr) (numC (s-exp->number expr))]
    [(s-exp-match? `(- ANY) expr) (multC (numC -1) (ae-parse (first (rest (s-exp->list expr)))))]
    [(s-exp-match? `(+ ANY ANY) expr) (plusC (ae-parse (first (rest (s-exp->list expr))))
                                             (ae-parse (first (rest (rest (s-exp->list expr))))))]
    [(s-exp-match? `(* ANY ANY) expr) (multC (ae-parse (first (rest (s-exp->list expr))))
                                             (ae-parse (first (rest (rest (s-exp->list expr))))))]
    [(s-exp-match? `(- ANY ANY) expr) (plusC (ae-parse (first (rest (s-exp->list expr))))
                                             (multC (numC -1)
                                              (ae-parse (first (rest (rest (s-exp->list expr)))))))]
    [else (error 'parse "invalid list input")]))

;; Test cases
(test/exn (ae-parse '(4)) "invalid list input")
(test (ae-parse '(+ 4 (* (- 3 (- 4)) 2))) (plusC (numC 4)
                                                 (multC (plusC (numC 3) 
                                                        (multC (numC -1)
                                                               (multC (numC -1) (numC 4))))
                                                        (numC 2))))

;; Parse and eval a s-exp
(define (parse-eval [expr : s-expression]) (eval (ae-parse expr)))

;; Test cases
(test (parse-eval '(+ 4 (* (- 3 (- 4)) 2))) 18)

;; Forms a list by applying a given function twice to each element of a list
(define (doublemap fn my-list)
  (map fn (map fn my-list)))

;; Test cases
(define (test-fn [n : number]) (* n 3))
(test (doublemap test-fn (list 3 4 5)) (list 27 36 45))

;; Consumes two lists of the same length and returns a list of lists
;; Where each element contains corresponding elements
(define (zip list1 list2)
  (cond
    [(empty? list1) empty]
    [else (cons (list (first list1) (first list2)) (zip (rest list1) (rest list2)))]))

;; Test cases
(test (zip (list 'a 'b 'c) (list 'd 'e 'f)) (list (list 'a 'd) (list 'b 'e) (list 'c 'f)))

;; Quicksort
;(define (quicksortt elements)
;  (cond
;    [(empty? elements) empty]
;    [else (let [smaller] empty)]))