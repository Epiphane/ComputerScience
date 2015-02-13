#lang plai-typed

(print-only-errors #t)
(require plai-typed/s-exp-match)

;; Gucci Lang v3
;; EBNF Specification:
;; GUCI3 = num
;;       | true/false
;;       | id
;;       | (if GUCI3 GUCI3 GUCI3)
;;       | (with (id = GUCI3) ... GUCI3)
;;       | (fn (id ...) GUCI3)
;;       | (operator GUCI3 GUCI3
;;       | (GUCI3 GUCI3 ...)
;; operator = +, -, *, /, eq?, <=

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Types
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Represents a value
(define-type Value
  [num (n : number)]
  [id (i : symbol)]
  [bool (b : boolean)]
  [clos (args : (listof symbol)) (body : ExprC) (env : Environment)])

;; Represents an expression
(define-type ExprC
  [val (v : Value)]
  [binop (op : (Value Value -> Value)) (l : ExprC) (r : ExprC)]
  [ifC (cond : ExprC) (success : ExprC) (fail : ExprC)]
  [appC (fun : ExprC) (args : (listof ExprC))]
  [lam (args : (listof symbol)) (body : ExprC)])

;; Represents an environment
(define-type-alias Environment (hashof symbol Value))

;; the empty environment
(define empty-env (hash (list)))

;; Converts binary operation to value-returning
(define (arith-op [fun : (number number -> number)]) : (Value Value -> Value)
  (lambda (a b) (num (fun (num-n a) (num-n b)))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Serialization
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (serialize [val : Value])
  (type-case Value val
    [num (n) (to-string n)]
    [id (i) (symbol->string i)]
    [bool (b) (cond
                [b "true"]
                [else "false"])]
    [clos (a b e) "#<procedure>"]))

;; Test cases
(test (serialize (num 32)) "32")
(test (serialize (id 'abc)) "abc")
(test (serialize (bool true)) "true")
(test (serialize (bool false)) "false")
(test (serialize (clos (list 'a) (binop (arith-op +) (val (num 5)) (val (num 2))) empty-env)) "#<procedure>")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Parsing
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Represents any binary arithmetic operator
(define (binary-op [type : s-expression]) : (optionof (Value Value -> Value))
  (cond 
    [(equal? type `+) (some (arith-op +))]
    [(equal? type `-) (some (arith-op -))]
    [(equal? type `*) (some (arith-op *))]
    [(equal? type `/) (some (arith-op /))]
    [(equal? type `<=) (some (lambda (a b) (bool (<= (num-n a) (num-n b)))))]
    [(equal? type `eq?)
     (some (lambda (a b)
             (type-case Value a
               [clos (a b c) (bool #f)]
               [else (type-case Value b
                       [clos (a b c) (bool #f)]
                       [else (bool (equal? a b))])])))]
    [else (none)]))

;; Equality tests
(define (get-op [op : s-expression]) : (Value Value -> Value)
  (type-case (optionof (Value Value -> Value)) (binary-op op)
    [some (s) s]
    [else (error 'e "Error!")]))
;; This is annoying
(test/exn (get-op `abc) "Error!")
(define eq-test (get-op `eq?))
(test (eq-test (num 3) (num 3)) (bool #t))
(test (eq-test (num 3) (num 1)) (bool #f))
(test (eq-test (num 1) (bool true)) (bool #f))
(test (eq-test (bool true) (bool false)) (bool #f))
(test (eq-test (bool true) (bool true)) (bool #t))
(test (eq-test
       (clos (list) (val (num 3)) empty-env)
       (clos (list) (val (num 3)) empty-env))
      (bool #f))
(test (eq-test
       (num 10)
       (clos (list) (val (num 3)) empty-env))
      (bool #f))
;; Finish equality tests

;; Reserved words
(define reserved (list `fn `true `false `if `with `+ `- `* `/ `<= `eq? `=))

;; Converts an s-expression into a list of symbols
(define (fn-args [exprs : (listof s-expression)]) : (listof symbol)
  (cond
    [(empty? exprs) empty]
    [(member (first exprs) (rest exprs))
     (error 'argument "Argument identifiers are not unique")]
    [(member (first exprs) reserved)
     (error 'argument "Reserved word cannot be used as identifier")]
    [else (append (list (s-exp->symbol (first exprs))) 
                  (fn-args (rest exprs)))]))

;; Gucci Lang Parser
(define (parse [expr : s-expression]) : ExprC
  (cond
    [(s-exp-match? `NUMBER expr) (val (num (s-exp->number expr)))]    
    [(s-exp-match? `SYMBOL expr)
     (cond [(equal? expr `true) (val (bool true))]
           [(equal? expr `false) (val (bool false))]
           [(member expr reserved)
            (error 'parse "Reserved word cannot be used as identifier")]
           [else (val (id (s-exp->symbol expr)))])]
    [(s-exp-match? `(if ANY ANY ANY) expr) 
     (ifC (parse (second (s-exp->list expr)))
          (parse (third (s-exp->list expr)))
          (parse (fourth (s-exp->list expr))))]
    [(s-exp-match? `(with {SYMBOL = ANY} ... ANY) expr)
     (appC
      (lam (fn-args (map (lambda (arg) (first (s-exp->list arg)))
                               (reverse (rest (reverse (rest (s-exp->list expr)))))))
                 (parse (first (reverse (s-exp->list expr)))))
      (map (lambda (arg) (parse (third (s-exp->list arg))))
           (reverse (rest (reverse (rest (s-exp->list expr)))))))]
    [(s-exp-match? `(fn {SYMBOL ...} ANY) expr)
     (lam (fn-args (s-exp->list (second (s-exp->list expr))))
                (parse (third (s-exp->list expr))))]
    [(s-exp-match? `(SYMBOL ANY ANY) expr)
     (type-case (optionof (Value Value -> Value))
       (binary-op (first (s-exp->list expr)))
       [some (fun) (binop fun
                          (parse (second (s-exp->list expr)))
                          (parse (third (s-exp->list expr))))]
       [none () 
             (cond [(member (first (s-exp->list expr)) reserved)
                    (error 'parse "Syntax does not match EBNF spec")]
                   [else (appC (val (id (s-exp->symbol 
                                         (first (s-exp->list expr)))))
                               (map (lambda (arg) (parse arg))
                                    (rest (s-exp->list expr))))])])]
    [(s-exp-match? `(ANY ...) expr)
     (cond
       [(member (first (s-exp->list expr)) reserved)
        (error 'parse "Syntax does not match EBNF spec")]
       [(s-exp-symbol? (first (s-exp->list expr)))
        (appC (val (id (s-exp->symbol (first (s-exp->list expr)))))
              (map (lambda (arg) (parse arg))
                   (rest (s-exp->list expr))))]
       [else 
        (appC (parse (first (s-exp->list expr)))
              (map (lambda (arg) (parse arg))
                   (rest (s-exp->list expr))))])]))

;; Test cases
(test (parse `true) (val (bool true)))
(test (parse `false) (val (bool false)))
(test (parse `((g 15) 10)) (appC (appC (val (id 'g)) (list (val (num 15)))) 
                                 (list (val (num 10)))))
(test (parse `134) (val (num 134)))
(test (parse `abc) (val (id 'abc)))
(test (parse `(fn {} 6)) (lam (list) (val (num 6))))

;; Exception testing
(test/exn (parse `(fn {a -} {- a -}))
          "Reserved word cannot be used as identifier")
(test/exn (parse `(with 1 2)) "Syntax does not match EBNF spec")
(test/exn (parse `(fn {a a} {- a a})) "Argument identifiers are not unique")
(test/exn (parse `(with {a = 5})) "Reserved word cannot be used as identifier")
(test/exn (parse `(fn {* a a})) "Syntax does not match EBNF spec")
(test/exn (parse `(if (<= 5 10) 'a 'b 'c)) "Syntax does not match EBNF spec")
(test/exn (parse `if) "Reserved word cannot be used as identifier")
(test/exn (parse `(if 1)) "Syntax does not match EBNF spec")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Evaluation
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Find a symbol in the given environment
(define (find-symbol [name : symbol] [env : Environment]) : Value
  (type-case (optionof Value) (hash-ref env name)
    [some (v) v]
    [else (error 'unbound-identifier (string-append "Unbound Identifier "
                                                    (symbol->string name)))]))

;; Add list of arguments to an environment
(define (env-vars [args : (listof symbol)] 
                  [evals : (listof ExprC)]
                  [env : Environment]
                  [result : Environment]) : Environment
  (cond
    [(and (empty? args) (empty? evals)) result]
    [(or (empty? args) (empty? evals))
     (error 'num-args "Incorrect number of arguments")]
    [else (env-vars (rest args)
                    (rest evals)
                    env
                    (hash-set result
                              (first args)
                              (interp (first evals) env)))]))

;; Main interpretation function
(define (interp [expr : ExprC] [env : Environment]) : Value
  (local [(define (find n) (find-symbol n env))]
    (local [(define (recur e) (interp e env))]
      (type-case ExprC expr
        [val (v) (type-case Value v
                   [id (name) (find name)]
                   [else v])]
        [binop (op l r)
               (cond
                 [(equal? (recur r) (num 0)) (error 'binop "Division by zero")]
                 [else (op (recur l) 
                           (recur r))])]
        [ifC (test success fail) (cond
                                   [(not (bool? (recur test)))
                                    (error 'if "Non-boolean test in if statement")]
                                   [(bool-b (recur test)) (recur success)]
                                   [else (recur fail)])]
        [lam (a b) (clos a b env)]
        [appC (func args)
              (local [(define fun (interp func env))]
                (type-case Value fun
                  [clos (arguments body clos-env)
                       (interp body (env-vars arguments args env clos-env))]
                  [else (error 'interp "Application of number or boolean")]))]))))

(define (top-eval [s : s-expression]) : string
  (serialize (interp (parse s) empty-env)))

;; Test cases
;; Environment
(define test-env (hash-set empty-env 'a*2b
                           (interp
                            (parse `(fn {a b} {* a (* 2 b)})) empty-env)))

(define (test-interp [expr : s-expression] [res : Value])
  (test (interp (parse expr) test-env) res))
(define (test-interp/exn [expr : s-expression] [err : string])
  (test/exn (interp (parse expr) test-env) err))

;; Basic EBNF Tests
(test-interp `(if (eq? (- 10 5) 5) 10 11) (num 10))
(test-interp `(with (val = 10)
                    (fun = (fn (a b) (* a b)))
                    (fun val 7)) (num 70))
(test-interp `(with (val = (if (<= (/ 9 3) (+ 1 1)) 10 11))
                    val) (num 11))
(test-interp `(a*2b 10 3) (num 60))
(test-interp `((fn (seven) (seven))
               ((fn (minus) (fn () (minus (+ 3 10) (* 2 3))))
                (fn (x y) (+ x (* -1 y))))) (num 7))
(test-interp `((fn (a) 
                  ((fn (b)
                      ((fn (c) (a (b c))) 10))
                   (fn (num) (a num))))
               (fn (num) (* num num))) (num 10000))

;; Exception testing
(test-interp/exn `(with (res = (if (<= (/ 9 3) (+ 1 1)) 10 11))
                    (res)) "Application of number or boolean")
(test/exn (interp (appC (val (num 10)) (list (val (num 12)))) test-env)
          "Application of number or boolean")
(test-interp/exn `(/ 12 0) "Division by zero")
(test-interp/exn `(ab 12) "Unbound Identifier")
(test-interp/exn `(+ / 2) "Reserved word cannot be used as identifier")
(test-interp/exn `(if (+ 10 2) 'a 'b) "Non-boolean test in if statement")
(test-interp/exn `(bad-fn 2) "Unbound Identifier")
(test-interp/exn `(+ a b) "Unbound Identifier")
(test-interp/exn `(a*2b 1 2 3) "Incorrect number of arguments")

;; Top-eval testing
(test (top-eval `(+ 10 2)) "12")