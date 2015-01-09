#lang plai-typed

;; Lab 3

;; 1. Combines a list of strings in reverse order
(define (rev-str-app strings)
  (cond
    [(cons? strings) (string-append (rev-str-app (rest strings)) (first strings))]
    [else ""]))


;; Test cases
(test (rev-str-app (list "ball" "juice" "frog")) "frogjuiceball")
(test (rev-str-app empty) "")

;; 2. Processor Representation
(define-type Processor
  [Intel (field : number)]
  [AMD (field : number)]
  [ARM (field : number)])

;; 3. Returns a list containing only Intel processors
(define (onlyIntels processors)
  (cond
    [(cons? processors)]
    [else ...]))

;; Test Cases
(test (onlyIntels (list (Intel 10) (AMD 15) (Intel 0) (ARM 7) (ARM 3))) (list (Intel 10) (Intel 0)))