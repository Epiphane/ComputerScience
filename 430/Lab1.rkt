#lang plai-typed

;; 2.2.2 Compute euros, given $
(define (dollar->euro [dollars : number])
  (* dollars 0.84))

;; Test cases
(test (dollar->euro 1) 0.84)

;; 2.3.2 Compute the value of a bag of coins
(define (sum-coins [pennies : number] [nickels : number] [dimes : number]
                   [quarters : number]) 
  (+ (+    pennies   (* nickels   5)) 
     (+ (* dimes 10) (* quarters 25))))

;; Test cases
(test (sum-coins 10 3 1 2) 85)

;; 4.4.1 Amount of interest that money earns
(define (interest [deposit : number])
  (cond
    [(<= deposit 1000) (* deposit 0.04) ]
    [(<= deposit 5000) (* deposit 0.045)]
    [(>  deposit 5000) (* deposit 0.05) ]))

;; Test cases
(test (interest 1000) 40)
(test (interest 5000) 225)
(test (interest 10000) 500)

;; 4.4.4 How many solutions for a quadratic equation?
(define (how-many [a : number] [b : number] [c : number])
  (cond
    [(> (* b b) (* (* 4 a) c)) 2]
    [(= (* b b) (* (* 4 a) c)) 1]
    [(< (* b b) (* (* 4 a) c)) 0]))
  
;; Test cases
(test (how-many 1 0 -1) 2)
(test (how-many 2 4 2) 1)
(test (how-many 1 0 1) 0)

;; 5.1.4 Determine form of a quadratic equation
(define (what-kind [a : number] [b : number] [c : number])
  (cond
    [(= a 0) 'degenerate]
    [(= (how-many a b c) 2) 'two]
    [(= (how-many a b c) 1) 'one]
    [(= (how-many a b c) 0) 'none]))

;; Test cases
(test (what-kind 0 5 -1) 'degenerate)
(test (what-kind 1 0 -1) 'two)
(test (what-kind 2 4 2) 'one)
(test (what-kind 1 0 1) 'none)