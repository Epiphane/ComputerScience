#lang plai-typed

(require (typed-in racket
                   [random : (number -> number)]
                   [random-seed : (number -> void)]))

;; Represents a value
(define-type Value
  [num (n : number)]
  [id (i : symbol)]
  [bool (b : boolean)]
  [clos (args : (listof symbol)) (body : ExprC) (env : Environment)])

;; Represents an expression
(define-type ExprC
  [val (v : Value)]
  [ifop (op : (number number -> boolean)) (l : ExprC) (r : ExprC)]
  [binop (op : (number number -> number)) (l : ExprC) (r : ExprC)]
  [ifC (cond : ExprC) (success : ExprC) (fail : ExprC)]
  [appC (fun : ExprC) (args : (listof ExprC))]
  [lam (args : (listof symbol)) (body : ExprC)])

;; Represents an environment
(define-type-alias Environment (hashof symbol Value))
(define empty-env (hash (list (values 'a (num 2)))))

;; Random containment
(define symbol-set (list 'a 'b 'c 'd 'x 'y 'z 'w))
(define max-rand-number 2)

;; Function to pick a random symbol
(define next-sym?
  (lambda (symbols)
     (lambda ()
       (cond
         [(equal? (random (length symbols)) 0) (first symbols)]
         [else ((next-sym? (rest symbols)))]))))
(define randomSymbol (next-sym? symbol-set))

(define randomBaseTerm
  (lambda ()
    (let [(rand (random 3))]
      (cond
        [(equal? rand 0) (val (num (random max-rand-number)))]
        [(equal? rand 1) (val (id (randomSymbol)))]
        [else (val (bool (equal? (random 2) 1)))]))))

(define random-ifop
  (lambda () (if (equal? (random 2) 1)
                 equal?
                 <=)))
(define random-binop
  (lambda ()
    (let [(op (random 4))]
      (cond
        [(equal? op 0) +]
        [(equal? op 1) -]
        [(equal? op 2) *]
        [(equal? op 3) /]))))
(define random-arg-list
  (lambda (depth max-length)
    (cond
      [(equal? (random max-length) 0) empty]
      [else (cons (randomTerm depth) (random-arg-list depth
                                                      (- max-length 1)))])))
(define random-symbol-list
  (lambda (max-length)
    (cond
      [(equal? (random max-length) 0) empty]
      [else (cons (randomSymbol) (random-symbol-list (- max-length 1)))])))
(define randomTerm
  (lambda (depth)
    (cond
      [(equal? depth 0) (randomBaseTerm)]
      [else (let [(option (random 8))]
              (cond
                [(<= option 2) (randomBaseTerm)]
                [(equal? option 3) (ifop (random-ifop)
                                         (randomTerm (- depth 1))
                                         (randomTerm (- depth 1)))]
                [(equal? option 4) (binop (random-binop)
                                         (randomTerm (- depth 1))
                                         (randomTerm (- depth 1)))]
                [(equal? option 5) (ifC (randomTerm (- depth 1))
                                        (randomTerm (- depth 1))
                                        (randomTerm (- depth 1)))]
                [(equal? option 6) (appC (randomTerm (- depth 1))
                                         (random-arg-list (- depth 1) 3))]
                [(equal? option 7) (lam (random-symbol-list 3)
                                        (randomTerm (- depth 1)))]))])))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Evaluation (Assignment 3)
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
                              (eval (first evals) env)))]))

;; Main interpretation function
(define (eval [expr : ExprC] [env : Environment]) : Value
  (local [(define (find n) (find-symbol n env))]
    (local [(define (recur e) (eval e env))]
      (type-case ExprC expr
        [val (v) (type-case Value v
                   [id (name) (find name)]
                   [else v])]
        [binop (op l r)
               (cond
                 [(equal? (recur r) (num 0)) (error 'binop "Division by zero")]
                 [else (num (op (num-n (recur l)) 
                                (num-n (recur r))))])]
        [ifop (op l r) (bool (op (num-n (recur l))
                                 (num-n (recur r))))]
        [ifC (test success fail) (cond
                                   [(not (bool? (recur test)))
                                    (error 'if "Non-boolean test in if statement")]
                                   [(bool-b (recur test)) (recur success)]
                                   [else (recur fail)])]
        [lam (a b) (clos a b env)]
        [appC (func args)
              (local [(define fun (eval func env))]
                (type-case Value fun
                  [clos (arguments body env)
                       (eval body (env-vars arguments args env env))]
                  [else (error 'eval "Application of number or boolean")]))]))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; End Assignment 3
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Runs a number of trials randomly
(define (runtrials [trials : number] [depth : number])
  (local [(define (runtrial [trial : number])
          (cond
            [(equal? trial 0) 0.0]
            [else (try (begin
                         (eval (randomTerm depth) empty-env)
                         (+ (runtrial (- trial 1)) 1))
                       (lambda () (runtrial (- trial 1))))]))]
    (/ (runtrial trials) trials)))

(runtrials 1000 1)