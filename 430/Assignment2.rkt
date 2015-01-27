#lang plai-typed

;; Assignment 2
;; Gucci Lang

(print-only-errors #t)
(require plai-typed/s-exp-match)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Parsing
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Represents any binary arithmetic operator
(define (binary-op [type : s-expression]) : (optionof (number number -> number))
  (cond 
    [(equal? type `+) (some +)]
    [(equal? type `-) (some -)]
    [(equal? type `*) (some *)]
    [(equal? type `/) (some /)]
    [else (none)]))

;; Represents an expression
(define-type ExprC
  [id (n : symbol)]
  [num (n : number)]
  [binop (op : (number number -> number)) (l : ExprC) (r : ExprC)]
  [if0 (cond : ExprC) (success : ExprC) (fail : ExprC)]
  [app (fun : symbol) (args : (listof ExprC))])

;; Function definition
(define-type FundefC
  [fdC (name : symbol) (args : (listof symbol)) (body : ExprC)])

;; Gucci Lang Parser
(define (parse [expr : s-expression]) : ExprC
  (cond
    [(s-exp-match? `NUMBER expr) (num (s-exp->number expr))]
    [(s-exp-match? `SYMBOL expr)
     (cond [(equal? expr `if0) (error 'parse "Bad if0 form")]
           [else (type-case (optionof (number number -> number)) (binary-op expr)
                   [some (fun) (error 'parse "Invalid binary operation")]
                   [none () (id (s-exp->symbol expr))])])]
    [(s-exp-match? `(if0 ANY ANY ANY) expr) (if0 (parse (second (s-exp->list expr)))
                                                 (parse (third (s-exp->list expr)))
                                                 (parse (fourth (s-exp->list expr))))]
    [(s-exp-match? `(SYMBOL ANY ...) expr)
     (type-case (optionof (number number -> number)) (binary-op (first (s-exp->list expr)))
       [some (fun) (cond
                     [(equal? (length (s-exp->list expr)) 3)
                      (binop fun 
                             (parse (second (s-exp->list expr)))
                             (parse (third (s-exp->list expr))))]
                     [else (error 'parse "Incorrect number of arguments")])]
       [none () (cond 
                  [(equal? (first (s-exp->list expr)) `if0)
                   (error 'parse "Bad if0 form")]
                  [else (app (s-exp->symbol (first (s-exp->list expr)))
                             (map (lambda (arg) (parse arg)) (rest (s-exp->list expr))))])])]
    [else (error 'parse "Invalid list input")]))

;; Converts an s-expression into a list of symbols
(define (s-exp->listofsymbol [expr : s-expression]) : (listof symbol)
  (map (lambda (piece) (s-exp->symbol piece)) (s-exp->list expr)))

;; Gucci Function Parser
(define (parse-fundef[expr : s-expression]) : FundefC
  (cond
    [(s-exp-match? `(fn SYMBOL (SYMBOL ...) ANY) expr)
     (let [(parts (s-exp->list expr))]
       (type-case (optionof (number number -> number)) (binary-op (second parts))
         [some (fun) (error 'parse "No binary operations!")]
         [none () 
               (fdC (s-exp->symbol (second parts))
                    (s-exp->listofsymbol (third parts))
                    (parse (fourth parts)))]))]
    [else (error 'parse-fundef "Invalid function form")]))

;; Test cases
(test/exn (parse `if0) "Bad if0 form")
(test/exn (parse `(if0 1)) "Bad if0 form")
(test/exn (parse-fundef `(fn + (a b) (+ a b))) "No binary operations!")
(test/exn (parse `(+ / 2)) "Invalid binary operation")
(test/exn (parse `(+ 10 2 3)) "Incorrect number of arguments")
(test (parse `(+ 10 2)) (binop + (num 10) (num 2)))
(test (parse `(* 10 (if0 0 (/ 6 2) -1))) (binop *
                                                (num 10) 
                                                (if0 (num 0) 
                                                     (binop / (num 6) (num 2)) 
                                                     (num -1))))
(test (parse `(if0 (- 10 10) a b)) (if0 (binop - (num 10) (num 10)) (id 'a) (id 'b)))
(test/exn (parse `(10 10 10)) "Invalid list input")
(test (parse-fundef `(fn square (x) (* x x))) (fdC 'square 
                                                   (list 'x)
                                                   (binop * (id 'x) (id 'x))))
(test/exn (parse-fundef `(square (x y) (10 * x))) "Invalid function form")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Evaluating
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (subst-arg [name : symbol] [fn-args : (listof symbol)] [args : (listof number)]) : ExprC
  (cond
    [(empty? fn-args) (error 'eval-error "Unbound identifier")]
    [else (cond
            [(equal? name (first fn-args)) (num (first args))]
            [else (subst-arg name (rest fn-args) (rest args))])]))

(define (subst [expr : ExprC] [fun-args : (listof symbol)] [args : (listof number)]) : ExprC
  (cond
    [(equal? (length fun-args) (length args))
     (local [(define (recur e) (subst e fun-args args))]
       (type-case ExprC expr
         [id (name) (subst-arg name fun-args args)]
         [num (n) expr]
         [binop (op l r) (binop op (recur l) (recur r))]
         [if0 (test success fail)
              (if0 (recur test) (recur success) (recur fail))]
         [app (fun args) (app fun (map recur args))]))]
    [else (error 'eval "Incorrect number of arguments")]))

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
      [app (name args)
           (let [(fun (find-function name funs))]
             (recur (subst (fdC-body fun) (fdC-args fun) (map recur args))))])))

(define (top-eval [s : s-expression] [fun-sexps : (listof s-expression)])
  : number
  (eval (parse s) (map parse-fundef fun-sexps)))

;; Test-cases
(define funs (list
              `(fn six () 6)
              `(fn double (x) (* 2 x))
              `(fn if1 (x a b) (if0 (- x 1) a b))
              `(fn double2 (x a b) (if0 (- (double x) 2) a b))
              `(fn bad-fn (x) (* x y))))

(define (test-eval [expr : s-expression] [res : number])
  (test (top-eval expr funs) res))
(define (test-eval/exn [expr : s-expression] [err : string])
  (test/exn (top-eval expr funs) err))

(test-eval `(six) 6)
(test-eval `(if0 (- 10 10) (double 2) 5) 4)
(test-eval/exn `(bad-fn 2) "Unbound identifier")
(test-eval/exn `(+ a b) "Unbound identifier")
(test-eval/exn `(double 1 2 3) "Incorrect number of arguments")
(test-eval/exn `(missing-fn a b) "No function with that name")
(test-eval `(if1 (+ (- 0 3) 4) 10 9) 10)
(test-eval `(double2 1 10 9) 10)

;; EBNF Test
(test-eval `10 10)
(test-eval `(+ 10 20) 30)
(test-eval `(* 10 20) 200)
(test-eval `(/ 10 (- 20 15)) 2)
(test-eval `(double 4) 8)
(test-eval `(if0 (double 2) 10 -1) -1)

#|

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