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
    [(cons? processors) (cond
                          [(Intel? (first processors)) 
                           (cons (first processors) (onlyIntels (rest processors)))]
                          [else (onlyIntels (rest processors))])]
    [else empty]))

;; Test Cases
(test (onlyIntels (list (Intel 10) (AMD 15) (Intel 0) (ARM 7) (ARM 3))) (list (Intel 10) (Intel 0)))

;; 4. Returns a list containing only AMD processors
(define (onlyAMDs processors)
  (cond
    [(cons? processors) (cond
                          [(AMD? (first processors)) 
                           (cons (first processors) (onlyAMDs (rest processors)))]
                          [else (onlyAMDs (rest processors))])]
    [else empty]))

;; Test Cases
(test (onlyAMDs (list (Intel 10) (AMD 15) (Intel 0) (ARM 7) (AMD 3))) (list (AMD 15) (AMD 3)))

;; 5. Consume a list of processors and predicate and filter accordingly
(define (onlyThese processors f)
  (cond
    [(cons? processors) (cond
                          [(f (first processors)) 
                           (cons (first processors) (onlyThese (rest processors) f))]
                          [else (onlyThese (rest processors) f)])]
    [else empty]))

;; Test Cases
(test (onlyThese (list (Intel 10) (AMD 15) (Intel 0) (ARM 7) (ARM 3)) ARM?) (list (ARM 7) (ARM 3)))

;; 6. Appends the second list to the first list
(define (my-append a b)
  (cond
    [(cons? a) (cons (first a) (my-append (rest a) b))]
    [else b]))

;; Test cases
(test (my-append (list 'a 'b 'c) (list 'd 'e 'f)) (list 'a 'b 'c 'd 'e 'f))

;; 7. Returns a list after dropping the first n elements
(define (my-drop list n)
  (cond
    [(and (cons? list) (> n 0)) (my-drop (rest list) (- n 1))]
    [else list]))

;; Test cases
(test (my-drop (list 'a 'b 'c) 1) (list 'b 'c))
(test (my-drop (list 'a 'b 'c) 5) empty)
(test (my-drop (list 'a 'b 'c 'd 'e) 3) (list 'd 'e))