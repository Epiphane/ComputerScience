#lang plai-typed

;; Assignment 2
;; Gucci Lang - tsteinke

;; Represents any binary arithmetic operator
(define (BinaryOp [type : symbol]) : (number number -> number)
  (cond
    [(equal? type '+) +]
    [(equal? type '-) -]
    [(equal? type '*) *]
    [(equal? type '/) /]
    [else (error 'bin-op-error "Binary operation not found")]))

;; Expression representation
(define-type ExprC
  [numC (n : number)]
  <idC-def>
  <app-def>
  [plusC (l : ExprC) (r : ExprC)]
  [multC (l : ExprC) (r : ExprC)])

;; Function definition
(define-type FunDefC
  [fdC (name : symbol) (arg : symbol) (body : ExprC)])

;; Test cases
(test (+ 4 (- 3 5)) 2)
(test (* 2 3) 6)
(test (- (/ 4 2) (* 2 3)) -4)

