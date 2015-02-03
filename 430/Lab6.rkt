#lang plai-typed

(print-only-errors #t)

;; Warmup
;; Accept a function and an argument and apply the function to the argument
(define one (lambda (fun) 
              (lambda (arg) (fun arg))))

;; Test cases
(test ((one (lambda (c) (* c c))) 10) 100)

;; 1. Accept a function/argument and apply the function to the argument twice
(define two (lambda (fun) 
              (lambda (arg) (fun (fun arg)))))

;; Test cases
(test ((two (lambda (c) (* c c))) 10) 10000)

;; 2. Accept a function/argument and return the argument
(define zero (lambda (fun) 
               (lambda (arg) arg)))

;; Test cases
(test ((zero +) 10) 10)

;; 3. Take a number-like function and returns a (number+1)-like function
(define add1 (lambda (nlike)
               (lambda (fun) 
                 (lambda (arg) ((nlike fun) (fun arg))))))

;; Test cases
(test (((add1 zero) (lambda (c) (* 2 c))) 10) 20)
(test (((add1 (add1 zero)) (lambda (c) (* 2 c))) 10) 40)

;; 4. Accept two number-like functions and returns a (a+b)-like function
(define add (lambda (nlike1 nlike2)
              (lambda (fun)
                (lambda (arg) ((nlike1 fun) ((nlike2 fun) arg))))))

;; Test cases
(test (((add one two) (lambda (c) (+ c 10))) 3) 33)
(test (((add (add one two) 
             (add1 two)) (lambda (c) (+ c 10))) 3) 63)

;; 5/6. Accept two arguments and return the first/second one, respectively
(define tru (lambda (a)
              (lambda (b) a)))
(define fals (lambda (a)
              (lambda (b) b)))

;; Test cases
(test ((tru 5) 6) 5)
(test ((fals 5) 6) 6)

;; 7. Accepts three arguments. If arg1 is tru, return the result of arg2
;;                          If arg1 is fals, return the result of arg3
(define if (lambda (test)
             (lambda (truarg)
               (lambda (falsarg)
                 ((test truarg) falsarg)))))

;; Test cases
(test (((if tru) 10) 9) 10)
(test (((if fals) 10) 9) 9)

;; SUB1
#;(define sub1 (lambda (nlike)
               (lambda (fun)
                 (lambda (arg) 
                   (nlike 
                    (lambda (fun)
                      (let (not-first fals)
                        (((if not-first) (lambda () arg)) fun))))))))