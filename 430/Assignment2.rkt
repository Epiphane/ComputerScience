#lang plai-typed

;; Assignment 2
;; Gucci Lang

(print-only-errors #t)
(require plai-typed/s-exp-match)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Parsing
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Represents any binary arithmetic operator
(define-type-alias BinaryOp (hashof symbol (number number -> number)))
(define binary-ops (hash (list ('+ +) ('- -) ('* *) ('/ /))))

;; Represents an expression
(define-type ExprC
  [id (n : symbol)]
  [num (n : number)]
  [binop (op : (number number -> number)) (l : ExprC) (r : ExprC)]
  [if0 (test : ExprC) (then : ExprC) (else : ExprC)]
  [app (fun : symbol) (args : (listof ExprC))])

;; Function definition
(define-type FundefC
  [fdC (name : symbol) (args : (listof symbol)) (body : ExprC)])

;; Gucci Lang Parser
(define (parse [expr : s-expression]) : ExprC
  (cond
    [(s-exp-match? `NUMBER expr) (num (s-exp->number expr))]
    [(s-exp-match? `SYMBOL expr) (id (s-exp->symbol expr))]
    [(s-exp-match? `(if0 ANY ANY ANY) expr) (if0 (parse (second (s-exp->list expr)))
                                                 (parse (third (s-exp->list expr)))
                                                 (parse (fourth (s-exp->list expr))))]
    [(s-exp-match? `(SYMBOL ANY ...) expr)
     (type-case (optionof (number number -> number)) (hash-ref binary-ops 
                                                               (first (s-exp->list expr)))
       [some (fun) (binop fun 
                          (parse (second (s-exp->list expr)))
                          (parse (third (s-exp->list expr))))]
       [none () (app (s-exp->symbol (first (s-exp->list expr)))
                     (map (lambda (arg) (parse arg)) (rest (s-exp->list expr))))])]
    [else (error 'parse "invalid list input")]))

;; Converts an s-expression into a list of symbols
(define (s-exp->listofsymbol [expr : s-expression]) : (listof symbol)
  (map (lambda (piece) (s-exp->symbol piece)) (s-exp->list expr)))

;; Gucci Function Parser
(define (parse-fundef[expr : s-expression]) : FundefC
  (cond
    [(s-exp-match? `(fn SYMBOL (SYMBOL ...) (ANY)) expr)
     (let [(parts (s-exp->list expr))]
       (fdC (s-exp->symbol (second parts))
            (s-exp->listofsymbol (third parts))
            (parse (fourth parts))))]))

;; Test cases
(test (parse `(+ 10 2)) (binop + (num 10) (num 2)))
(test (parse `(* 10 (if0 0 (/ 6 2) -1))) (binop *
                                                (num 10) 
                                                (if0 (num 0) 
                                                     (binop / (num 6) (num 2)) 
                                                     (num -1))))
(test (parse `(if0 (- 10 10) a b)) (if0 (binop - (num 10) (num 10)) (id 'a) (id 'b)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Evaluating
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (subst-arg [name : symbol] [fn-args : listof symbol] [args : listof number])
  (cond
    [(empty? fn-args) (errpr 'eval-error "Unbound identifier")]
    [(equal? name (first fn-args)) (first args)]
    [else (subst-arg name (rest fn-args) (rest args))]))

(define (subst [expr : ExprC] [fun-args : (listof symbol)] [args : (listof number)])
  (type-case ExprC expr
    [id (name) (subst-arg name fun-args args)]
    [num (n : number)]
    [binop (op : (number number -> number)) (l : ExprC) (r : ExprC)]
    [if0 (test : ExprC) (then : ExprC) (else : ExprC)]
    [app (fun : symbol) (args : (listof ExprC))]))

(define (find-function [f : symbol] [funs : (listof FundefC)]) : FundefC
  (cond [(empty? funs) (error 'find-function "No function with that name")]
        [else (cond [(equal? f (fdC-name (first funs))) (first funs)]
                    [else (find-function f (rest funs))])]))

(define (eval [expr : ExprC] [funs : (listof FundefC)]) : number
  (local [(define (recur e) (eval e funs))]
    (type-case ExprC expr
      [id (s)
          (error 'eval-error "Unbound identifier")]
      [num (n) n]
      [binop (op l r) (op (recur l) 
                          (recur r))]
      [if0 (test success fail) (cond
                                 [(equal? (recur test) 0) (recur success)]
                                 [else (recur fail)])]
      [app (fun args)
           (recur (subst fun args))])))

(define (top-eval [s : s-expression] [fun-sexps : (listof s-expression)])
  : number
  (eval (parse s) (map parse-fundef fun-sexps)))


#|

;; Expression representation
(define-type ExprC
  [numC (n : number)]
  [plusC (l : ExprC) (r : ExprC)]
  [multC (l : ExprC) (r : ExprC)]
  <idC-def>
  <app-def>)


;; Test cases
|#


#|;; Evaluation for ArithC Expressions
(define (eval [exp : ExprC]) : number
  (type-case ExprC exp
    [numC (n) n]
    [plusC (l r) (+ (eval l) (eval r))]
    [multC (l r) (* (eval l) (eval r))]))

;; Takes an AE and returns how many numbers it contains
(define (num-nums [exp : ExprC]) : number
  (type-case ExprC exp
    [numC (n) 1]
    [plusC (l r) (+ (num-nums l) (num-nums r))]
    [multC (l r) (+ (num-nums l) (num-nums r))]))

;; Test cases
(test (num-nums (plusC (numC 10) (multC (numC 5) (plusC (numC 3) (numC 3))))) 4)

;; Test cases
(test/exn (parse '(4)) "invalid list input")
(test (parse '(+ 4 (* (- 3 (- 4)) 2))) (plusC (numC 4)
                                                 (multC (plusC (numC 3) 
                                                        (multC (numC -1)
                                                               (multC (numC -1) (numC 4))))
                                                        (numC 2))))

;; Parse and eval a s-exp
(define (parse-eval [expr : s-expression]) (eval (parse expr)))

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

;; Test cases
(test (top-eval `(+ 4 (square 3)) (list `{fn square (x) (* x x)})) 
      13)|#