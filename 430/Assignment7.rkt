#lang plai-typed

(print-only-errors #t)
(require plai-typed/s-exp-match)

;; Gucci Lang v3
;; EBNF Specification:
;; GUCI3 = num
;;       | true/false
;;       | id
;;       | (if GUCI3 GUCI3 GUCI3)
;;       | (fn (id ...) GUCI3)
;;       | (operator GUCI3 GUCI3
;;       | (GUCI3 GUCI3 ...)
;;       | {let-stx {any-id = {[pat-sexp => sexp] ...}} GUCI7}
;; operator = +, -, *, /, eq?, <=

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Types
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Represents a value
(define-type Value
  [num (n : number)]
  [bool (b : boolean)]
  [clos (args : (listof symbol)) (body : ExprC) (env : Environment)])

;; Represents an expression
(define-type ExprC
  [numC (n : number)]
  [idC (i : symbol)]
  [boolC (b : boolean)]
  [binop (op : (Value Value -> Value)) (l : ExprC) (r : ExprC)]
  [ifC (cond : ExprC) (success : ExprC) (fail : ExprC)]
  [appC (fun : ExprC) (args : (listof ExprC))]
  [lam (args : (listof symbol)) (body : ExprC)])

;; Represents an environment
(define-type-alias Environment (hashof symbol Value))

;; the empty environment
(define empty-env (hash (list)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Serialization
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (serialize [val : Value])
  (type-case Value val
    [num (n) (to-string n)]
    [bool (b) (cond
                [b "true"]
                [else "false"])]
    [clos (a b e) "#<procedure>"]))

;; Test cases
(test (serialize (num 32)) "32")
(test (serialize (bool true)) "true")
(test (serialize (bool false)) "false")
(test (serialize (clos (list 'a) (numC 2) empty-env)) "#<procedure>")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Arithmetic Helpers
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Helper functions to create binary operations
(define err-not-number (lambda (v) (error 'value 
                                          (string-append "Not a number: "
                                                         (serialize v)))))
(define (arith-op [op : (number number -> number)]) : (Value Value -> Value)
  (lambda (a b) 
    (if (num? a)
        (if (num? b)
            (num (op (num-n a) (num-n b)))
            (err-not-number b))
        (err-not-number a))))
(define (if-op [op : (number number -> boolean)])
  (lambda (a b) 
    (if (num? a)
        (if (num? b)
            (bool (op (num-n a) (num-n b)))
            (err-not-number b))
        (err-not-number a))))
(define (binary-op [type : s-expression]) : (optionof (Value Value -> Value))
  (cond 
    [(equal? type `+) (some (arith-op +))]
    [(equal? type `-) (some (arith-op -))]
    [(equal? type `*) (some (arith-op *))]
    [(equal? type `/) (some (arith-op (lambda (a b)
                                        (if (equal? b 0)
                                            (error 'eval 
                                                   (string-append (string-append 
                                                                   "Division of "
                                                                   (to-string a))
                                                                  " by zero"))
                                            (/ a b)))))]
    [(equal? type `eq?) (some (lambda (a b) (bool (equal? a b))))]
    [(equal? type `<=) (some (if-op <=))]
    [else (none)]))

;; Gets an arith op for tests
(define (get-arith-op [op : s-expression])
  (some-v (binary-op op)))

;; Test cases
(test ((get-arith-op `+) (num 5) (num 6)) (num 11))
(test/exn ((get-arith-op `-) (bool true) (num 6)) "Not a number: true")
(test/exn ((get-arith-op `/) (num 6) (bool false)) "Not a number: false")
(test ((get-arith-op `*) (num 5) (num 6)) (num 30))
(test ((get-arith-op `<=) (num 5) (num 6)) (bool true))
(test ((get-arith-op `eq?) (num 5) (bool false)) (bool false))
(test/exn ((get-arith-op `<=) (bool true) (num 6)) "Not a number: true")
(test/exn ((get-arith-op `<=) (num 6) (bool true)) "Not a number: true")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Expansion
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Expressions for let-stx
(define-type pat-sexp
  [pat-id (i : symbol)]
  [pat-list (l : (listof pat-sexp))])
(define-type sexp
  [sexp-id (i : symbol)]
  [sexp-num (n : number)]
  [sexp-list (l : (listof sexp))])
(define-type-alias stx (listof (pat-sexp * sexp)))
(define empty-stx (list))

;; Pattern matching checking
(define (pat-sexp->matcher [pat : pat-sexp]) : s-expression
  (type-case pat-sexp pat
    [pat-id (i) `ANY]
    [pat-list (l) (list->s-exp (map pat-sexp->matcher l))]))

;; Get stx vars from s-expression
(define-type-alias stx-vars (hashof symbol s-expression))
(define empty-vars (hash (list)))
(define (pat-sexp-env [expr : s-expression] [pat : pat-sexp] [env : stx-vars]) : stx-vars
  (type-case pat-sexp pat
    [pat-id (i) (hash-set env i expr)]
    [pat-list (l)
              (local [(define (recur exprs patlist env)
                        (cond
                          [(empty? exprs) env]
                          [else (recur (rest exprs) 
                                  (rest patlist)
                                  (pat-sexp-env (first exprs) (first patlist) env))]))]
                (recur (s-exp->list expr) l env))]))

;; Substitute in stx vars
(define (sexp-subst [exp : sexp] [vars : stx-vars]) : s-expression
  (type-case sexp exp
    [sexp-id (i) (type-case (optionof s-expression) (hash-ref vars i)
                   [some (s) s]
                   [none () (symbol->s-exp i)])]
    [sexp-num (n) (number->s-exp n)]
    [sexp-list (l) (list->s-exp (map (lambda (exp) (sexp-subst exp vars)) l))]))

;; Convert an s-expression into pat-sexp and sexp
(define (s-exp->pat-sexp [exp : s-expression]) : pat-sexp
  (cond
    [(s-exp-match? `SYMBOL exp) (pat-id (s-exp->symbol exp))]
    [(s-exp-match? `{ANY ...} exp) (pat-list (map s-exp->pat-sexp (s-exp->list exp)))]
    [else (error 'type "Could not convert s-expression to pat-sexp")]))
(define (s-exp->sexp [exp : s-expression]) : sexp
  (cond
    [(s-exp-match? `SYMBOL exp) (sexp-id (s-exp->symbol exp))]
    [(s-exp-match? `NUMBER exp) (sexp-num (s-exp->number exp))]
    [(s-exp-match? `{ANY ...} exp) (sexp-list (map s-exp->sexp (s-exp->list exp)))]
    #;[else (error 'type "Could not convert s-expression to sexp")]))
;; Code coverage
(test/exn (s-exp->pat-sexp `10) "Could not convert s-expression to pat-sexp")

;; Substitute a stx s-expression given the environment hash
(define (subst [expr : s-expression] [syn : stx] [env : (hashof symbol stx)])
  (cond
    [(empty? syn) (error 'expand "No matching syntax found")]
    [(equal? (length (s-exp->list expr)) (length (pat-list-l (fst (first syn)))))
     (local [(define syntax (first syn))]
       (cond 
         [(s-exp-match? (pat-sexp->matcher (fst syntax)) expr)
          (local [(define vars (pat-sexp-env expr (fst syntax) empty-vars))]
            (expand-stx (sexp-subst (snd syntax) vars) env))]
         [else (subst expr (rest syn) env)]))]
    [else (subst expr (rest syn) env)]))

;; Expand an s-expression using let-stx syntax
(define (expand-rules [exprs : (listof s-expression)] [h : stx])
  (cond
    [(empty? exprs) h]
    [else (local [(define parts (s-exp->list (first exprs)))]
            (append (list (values (s-exp->pat-sexp (first parts))
                                  (s-exp->sexp (third parts))))
                    (expand-rules (rest exprs) h)))]))
(define (expand-stx [expr : s-expression] [env : (hashof symbol stx)]) : s-expression
  (cond
    ;; Is it a list?
    [(s-exp-match? `{ANY ...} expr)
     (local [(define exp-parts (s-exp->list expr)) (define (recur e) (expand-stx e env))]
       (cond 
         [(empty? exp-parts) expr]
         [(equal? (first exp-parts) `let-stx)
          (cond
            [(s-exp-match? `{let-stx {SYMBOL = {[ANY => ANY] ...}} ANY} expr)
             (expand-stx (third exp-parts) 
                         (hash-set env
                                   (s-exp->symbol (first (s-exp->list (second exp-parts))))
                                   (expand-rules (s-exp->list (third (s-exp->list (second exp-parts)))) 
                                                 empty-stx)))]
            [else (error 'expand "Syntax does not match EBNF spec")])]
         ;; Check for substitutions
         [(s-exp-match? `SYMBOL (first exp-parts))
          (type-case (optionof stx) (hash-ref env (s-exp->symbol (first exp-parts)))
            [some (s) (subst expr s env)]
            [none () (list->s-exp (map recur exp-parts))])]
         [else (list->s-exp (map recur exp-parts))]))]
    [else expr]))
(define (expand [expr : s-expression])
  (expand-stx expr (hash (list))))

;; Test cases
(test (expand `{}) `{})
(test (expand `{let-stx {or = {[{or a b} => {{fn {temp} {if temp temp b}} a}]}}
                        {or false {+ 12 1}}})
      `{{fn {temp} {if temp temp {+ 12 1}}} false})
(test (expand `{let-stx {or = 
                            {[{or a b c} => {nothing}]
                             [{or a b} => {{fn {temp} {if temp temp b}} a}]}}
                        {or false {+ 12 1}}})
      `{{fn {temp} {if temp temp {+ 12 1}}} false})
(test (expand `{let-stx {or = {[{or a b} => {{fn {temp} {if temp temp b}} a}]}}
                        {let-stx {nor = {[{nor a b} => {{fn {temp} {if temp temp b}} a}]}}
                                 {or false {+ 12 1}}}})
      `{{fn {temp} {if temp temp {+ 12 1}}} false})
(test (expand `{let-stx {nor = {[{nor a b} => {{fn {temp} {if temp temp b}} a}]}}
                        {let-stx {or = {[{or a b} => {{fn {temp} {if temp temp b}} a}]}}
                                 {or false {+ 12 1}}}})
      `{{fn {temp} {if temp temp {+ 12 1}}} false})
(test (expand `{let-stx {nor = {[{nor a b} => {{fn {temp} {if temp temp b}} a}]}}
                        {let-stx {or = {[{or a b} => {{fn {temp} {if temp temp b}} a}]}}
                                 {or (nor false true) {+ 12 1}}}})
      `{{fn {temp} {if temp temp {+ 12 1}}} {{fn {temp} {if temp temp true}} false}})
(test (expand `{let-stx {rec = {[{rec a b} => {+ {rec a} {rec b}}]
                                [{rec a} => {* 2 a}]}}
                        {let-stx {if-not-a = {[{if-not-a a b} => {if a b a}]}}
                                 {rec {if-not-a false 10} 12}}})
      `{+ {* 2 {if false 10 false}} {* 2 12}})
(test (expand `{let-stx {rec = {[{rec a b} => {+ {rec a} {rec b}}]
                                [{rec a {b c}} => {wrong}]
                                [{rec a} => {* 2 a}]}}
                        {let-stx {if-not-a = {[{if-not-a a b} => {if a b a}]}}
                                 {rec {if-not-a false 10} 12}}})
      `{+ {* 2 {if false 10 false}} {* 2 12}})
(test (expand `{let-stx {rec = {[{rec a {b c}} => {correct}]
                                [{rec a b} => {+ {rec a} {rec b}}]
                                [{rec a} => {* 2 a}]}}
                        {let-stx {if-not-a = {[{if-not-a a b} => {if a b a}]}}
                                 {rec {if-not-a false 10} {12 1}}}})
      `{correct})
#;(test (expand `{let-stx {nothing = {}}
                        {nothing 12 2}})
      `{nothing 12 2})
(test/exn (expand '(let-stx (zip = (((zip a b c) => 234))) (zip 12))) "No matching syntax found")
(test/exn (expand `{let-stx 10}) "Syntax does not match EBNF spec")
(test (expand (quote (let-stx (s-eval = (((s-eval (a b)) => (+ (s-eval a) (s-eval b))) 
                                         ((s-eval a) => a))) 
                              (s-eval ((1 (2 3)) ((4 5) (6 7)))))))
      `(+ (+ 1 (+ 2 3)) (+ (+ 4 5) (+ 6 7))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Parsing
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Reserved symbols
(define reserved (list `fn `true `false `if `+ `- `* `/ `<= `eq? `=))

;; Converts an s-expression into a list of arguments
(define (fn-args [exprs : (listof s-expression)]) : (listof symbol)
  (cond
    [(empty? exprs) empty]
    [(member (first exprs) (rest exprs))
     (error 'argument "Argument identifiers are not unique")]
    [(member (first exprs) reserved)
     (error 'argument "Reserved word cannot be used as identifier")]
    [else (append (list (s-exp->symbol (first exprs))) 
                  (fn-args (rest exprs)))]))


(define (parse-expr [expr : s-expression]) : ExprC
  (cond
    [(s-exp-match? `NUMBER expr) (numC (s-exp->number expr))]
    [(s-exp-match? `true expr) (boolC true)]
    [(s-exp-match? `false expr) (boolC  false)]
    [(s-exp-match? `SYMBOL expr) 
     (if (member expr reserved)
         (error 'parse "Reserved word cannot be used as identifier")
         (idC (s-exp->symbol expr)))]
    [(s-exp-match? `(if ANY ANY ANY) expr)
     (ifC (parse (second (s-exp->list expr)))
          (parse (third (s-exp->list expr)))
          (parse (fourth (s-exp->list expr))))]
    [(s-exp-match? `(fn {SYMBOL ...} ANY) expr)
     (lam (fn-args (s-exp->list (second (s-exp->list expr))))
                (parse (third (s-exp->list expr))))]
    [(s-exp-match? `(SYMBOL ANY ANY) expr)
     (type-case (optionof (Value Value -> Value)) (binary-op (first (s-exp->list expr)))
       [some (fun) (binop fun
                          (parse (second (s-exp->list expr)))
                          (parse (third (s-exp->list expr))))]
       [none () 
             (cond [(member (first (s-exp->list expr)) reserved)
                    (error 'parse "Syntax does not match EBNF spec")]
                   [else (appC (idC (s-exp->symbol 
                                         (first (s-exp->list expr))))
                               (map (lambda (arg) (parse arg))
                                    (rest (s-exp->list expr))))])])]
    [(s-exp-match? `(SYMBOL ANY ...) expr)
     (if (member (first (s-exp->list expr)) reserved)
         (error 'parse "Reserved word cannot be used as identifier")
         (appC (idC (s-exp->symbol (first (s-exp->list expr))))
               (map (lambda (arg) (parse arg))
                    (rest (s-exp->list expr)))))]
    [(s-exp-match? `(ANY ...) expr)
     (appC (parse (first (s-exp->list expr)))
              (map (lambda (arg) (parse arg))
                   (rest (s-exp->list expr))))]))

(define (parse [exp : s-expression]) : ExprC
  (parse-expr (expand exp)))

;; Test cases
(test (parse `true) (boolC true))
(test (parse `false) (boolC false))
(test (parse `134) (numC 134))
(test (parse `abc) (idC 'abc))
(test (serialize (clos (list) (parse `(+ 4 5)) empty-env)) "#<procedure>")
(test (parse `(f 5 6)) (appC (idC 'f) (list (numC 5) (numC 6))))
#;(test (parse `(if {<= a b} 19 (if {eq? c d} 20 21))) 
      (ifC (binop <=
                  (idC 'a)
                  (idC 'b))
           (numC 19)
           (ifC (binop eq? (idC 'c) (idC 'd)) (numC 20) (numC 21))))
(test (parse `(fn {} 6)) (lam (list) (numC 6)))

;; Exception testing
(test/exn (parse `(if 4 5)) "Syntax does not match EBNF spec")
(test/exn (parse `(fn {a -} {- a -})) "Reserved word cannot be used as identifier")
(test/exn (parse `(fn {a a} {- a a})) "Argument identifiers are not unique")
(test/exn (parse `(13 - 10)) "Reserved word cannot be used as identifier")
(test/exn (parse `(if {a = 5})) "Reserved word cannot be used as identifier")
(test/exn (parse `(fn {* a a})) "Reserved word cannot be used as identifier")
(test/exn (parse `(if (<= 5 10) 'a 'b 'c)) "Reserved word cannot be used as identifier")
(test/exn (parse `if) "Reserved word cannot be used as identifier")
(test/exn (parse `(if 1)) "Reserved word cannot be used as identifier")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Evaluation
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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
    [else 
     (env-vars (rest args)
               (rest evals)
               env
               (hash-set result (first args) 
                         (interp (first evals) env)))]))

;; Error helper
(define (not-a [type : string] [val : Value])
  (error 'type (string-append (string-append "Not a " type)
                              (string-append ": " (serialize val)))))

;; Interpret an ExprC
(define (interp [e : ExprC] [env : Environment]) : Value
  (type-case ExprC e
    [numC (n) (num n)]
    [boolC (b) (bool b)]
    [idC (i) (find-symbol i env)]
    [binop (op a b) (op (interp a env) (interp b env))]
    [ifC (test success fail)
         (type-case Value (interp test env)
           [bool (b) (if b (interp success env) (interp fail env))]
           [else (not-a "boolean" (interp test env))])]
    [appC (f a) (type-case Value (interp f env)
                  [clos (args body clos-env)
                        (interp body (env-vars args a env clos-env))]
                  [else (not-a "function" (interp f env))])]
    [lam (args body) (clos args body env)]))

;; Top-eval
(define (top-eval [s : s-expression]) : string
  (serialize (interp (parse s) empty-env)))

;; Test cases
;; Environment
(define (test-interp [expr : s-expression])
  (interp (parse expr) empty-env))

;; Basic EBNF Tests
(test (top-eval `{if false {+ 2 3} {+ 3 4}}) "7")
(test (test-interp `false) (bool false))
(test (test-interp `(+ 4 7)) (num 11))
(test (test-interp `(/ 3 1)) (num 3))
(test (test-interp `(if (eq? (- 10 5) 5) 10 11)) (num 10))

(test (test-interp `((fn (seven) (seven))
                     ((fn (minus) (fn () (minus (+ 3 10) (* 2 3))))
                      (fn (x y) (+ x (* -1 y)))))) (num 7))
(test (test-interp `((fn (a) 
                   ((fn (b)
                        ((fn (c) (a (b c))) 10))
                    (fn (num) (a num))))
               (fn (num) (* num num)))) (num 10000))
(test (test-interp `(fn {} 6)) (clos (list) (numC 6) empty-env))

;; Exception testing
(test/exn (interp (appC (numC 10) (list (numC 12))) empty-env)
          "Not a function: 10")
(test/exn (test-interp `(if {<= a b} 19 (if {eq? c d} 20 21)))
                 "Unbound Identifier")
(test/exn (test-interp `(/ true 0)) "Not a number: true")
(test/exn (test-interp `(if 10 1 0)) "Not a boolean: 10")
(test/exn (test-interp `(/ 12 0)) "Division of 12 by zero")
(test/exn (test-interp `ab) "Unbound Identifier")
(test/exn (test-interp `(ab 12)) "Unbound Identifier")
(test/exn (test-interp `(+ / 2)) "Reserved word cannot be used as identifier")
(test/exn (test-interp `(if (+ 10 2) 'a 'b)) "Not a boolean: 12")
(test/exn (test-interp `(bad-fn 2)) "Unbound Identifier")
(test/exn (test-interp `(+ a b)) "Unbound Identifier")
(test/exn (test-interp `((fn () 6) 1 2 3)) "Incorrect number of arguments")