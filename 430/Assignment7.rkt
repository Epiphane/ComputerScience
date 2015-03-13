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
;;       | (GUCI7 GUCI7 ...)
;;       | (let-stx (id = ([pat-sexp => sexp] ...)) GUCI7)
;; pat-sexp = any-id
;;          | num
;;          | (pat-sexp ...)
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
  [val (v : Value)]
  [idC (i : symbol)]
  [ifop (op : (number number -> boolean)) (l : ExprC) (r : ExprC)]
  [binop (op : (number number -> number)) (l : ExprC) (r : ExprC)]
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
(test (serialize (clos (list 'a) (binop + (val (num 5)) (val (num 2))) empty-env)) "#<procedure>")

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
(define-type-alias stx (hashof string (type pat-sexp sexp)))
(define empty-stx (hash (list)))

;; Convert an s-expression into pat-sexp and sexp
(define (string-append-list [l : (listof string)]) : string
  (cond
    [(empty? l) ""]
    [else (string-append (first l) (string-append-list (rest l)))]))
(define (pat-sexp->bitmap [pat : pat-sexp]) : string
  (type-case pat-sexp pat
    [pat-id (i) "_"]
    [pat-list (l) (string-append (string-append "-"
                                                (string-append-list 
                                                 (map pat-sexp->bitmap l)))
                                 "|")]))
(define (s-exp->pat-sexp [exp : s-expression]) : string
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
;;()

;; Expand an s-expression using let-stx syntax
(define (expand-rules [exprs : (listof s-expression)] [h : stx])
  (cond
    [(empty? exprs) h]
    [else (local [(define parts (s-exp->list (first exprs)))]
            (hash-set (expand-rules (rest exprs) h)
                      (s-exp->pat-sexp (first parts))
                      (s-exp->sexp (third parts))))]))
(define (expand-stx [expr : s-expression] [env : (hashof symbol stx)]) : s-expression
  (cond
    ;; Is it a list?
    [(s-exp-match? `{ANY ...} expr)
     (local [(define exp-parts (s-exp->list expr)) (define (recur e) (expand-stx e env))]
       (cond 
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
            [some (s)
                  (type-case (optionof sexp) (hash-ref s 
                                                       (s-exp->pat-sexp 
                                                        (list->s-exp
                                                         (rest exp-parts))))
                    ;;[some (exp) (substitute)]
                    [none () expr])]
            [none () expr])]
         [else (list->s-exp (map recur exp-parts))]))]
    [else expr]))
(define (expand [expr : s-expression])
  (expand-stx expr (hash (list))))

;; Test cases
(test (expand `{let-stx {or = {[{or a b} => {{fn {temp} {if temp temp b}} a}]}}
                        {or false {+ 12 1}}})
      `{{fn {temp} {if temp temp {+ 12 1}}} false})
(test (expand `{let-stx {rec = {[{rec a b} => {{+ rec a} {rec b}}]
                                [{rec a} => {* 2 a}]}}
                        {let-stx {if-not-a = {[{if-not-a a b} => {if a b a}]}}
                                 {rec {if-not-a false 10} 12}}})
      `{{+ {* 2 {if false 10 false}} {* 2 12}}})
(test (expand `{let-stx {nothing = {}}
                        {nothing 12 2}})
      `{nothing 12 2})
(test/exn (expand `{let-stx 10}) "Syntax does not match EBNF spec")

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

(define (if-op [type : s-expression]) : (optionof (number number -> boolean))
  (cond
    [(equal? type `<=) (some <=)]
    [(equal? type `eq?) (some equal?)]
    [else (none)]))

;; Reserved words
(define reserved (list `fn `true `false `if `let-stx `+ `- `* `/ `<= `eq? `= `=>))

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
           [else (idC (s-exp->symbol expr))])]
    [(s-exp-match? `(if ANY ANY ANY) expr) 
     (ifC (parse (second (s-exp->list expr)))
          (parse (third (s-exp->list expr)))
          (parse (fourth (s-exp->list expr))))]
    [(s-exp-match? `(fn {SYMBOL ...} ANY) expr)
     (lam (fn-args (s-exp->list (second (s-exp->list expr))))
                (parse (third (s-exp->list expr))))]
    [(s-exp-match? `(SYMBOL ANY ANY) expr)
     (type-case (optionof (number number -> number))
       (binary-op (first (s-exp->list expr)))
       [some (fun) (binop fun
                          (parse (second (s-exp->list expr)))
                          (parse (third (s-exp->list expr))))]
       [none () 
             (type-case (optionof (number number -> boolean))
               (if-op (first (s-exp->list expr)))
               [some (fun) (ifop fun
                                 (parse (second (s-exp->list expr)))
                                 (parse (third (s-exp->list expr))))]
               [none () (cond [(member (first (s-exp->list expr)) reserved)
                               (error 'parse "Syntax does not match EBNF spec")]
                              [else (appC (idC (s-exp->symbol 
                                                (first (s-exp->list expr))))
                                          (map (lambda (arg) (parse arg))
                                               (rest (s-exp->list expr))))])])])]
    [(s-exp-match? `(ANY ...) expr)
     (cond
       [(member (first (s-exp->list expr)) reserved)
        (error 'parse "Syntax does not match EBNF spec")]
       [(s-exp-symbol? (first (s-exp->list expr)))
        (appC (idC (s-exp->symbol (first (s-exp->list expr))))
              (map (lambda (arg) (parse arg))
                   (rest (s-exp->list expr))))]
       [else 
        (appC (parse (first (s-exp->list expr)))
              (map (lambda (arg) (parse arg))
                   (rest (s-exp->list expr))))])]))

;; Test cases
(test (parse `true) (val (bool true)))
(test (parse `false) (val (bool false)))
(test (parse `((g 15) 10)) (appC (appC (idC 'g) (list (val (num 15)))) 
                                 (list (val (num 10)))))
(test (parse `134) (val (num 134)))
(test (parse `abc) (idC 'abc))
(test (parse `(if {<= a b} 19 21)) 
      (ifC (ifop <=
                  (idC 'a)
                  (idC 'b))
           (val (num 19))
           (val (num 21))))
(test (parse `(fn {} 6)) (lam (list) (val (num 6))))
(test (parse `{let-stx {or = [{or a b} => {{fn {temp} {if temp temp b}} a}]}
                       {or false {+ 12 1}}})
      (appC (lam (list 'val) (ifC (idC 'val) 
                                  (idC 'val) 
                                  (binop + (val (num 12)) (val (num 1))))) 
            (list (val (bool false)))))

;; Exception testing
(test/exn (parse `(fn {a -} {- a -}))
          "Reserved word cannot be used as identifier")
(test/exn (parse `(fn {a a} {- a a})) "Argument identifiers are not unique")
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

(define (getnum n)
  (if (num? n) (num-n n) (error 'type "Not a number")))
;; Code coverage
(test/exn (getnum (bool true)) "Not a number")

;; Main interpretation function
(define (interp [expr : ExprC] [env : Environment]) : Value
  (local [(define (find n) (find-symbol n env))]
    (local [(define (recur e) (interp e env))]
      (type-case ExprC expr
        [idC (i) (find i)]
        [val (v) v]
        [binop (op l r)
               (cond
                 [(equal? (recur r) (num 0)) (error 'binop "Division by zero")]
                 [else (num (op (getnum (recur l)) 
                                (getnum (recur r))))])]
        [ifop (op l r) (bool (op (getnum (recur l))
                                 (getnum (recur r))))]
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
(test-interp `(if (eq? (- 10 3) 5) 10 11) (num 11))
(test-interp `(a*2b 10 3) (num 60))
(test-interp `((fn (seven) (seven))
               ((fn (minus) (fn () (minus (+ 3 10) (* 2 3))))
                (fn (x y) (+ x (* -1 y))))) (num 7))
(test-interp `((fn (a) 
                  ((fn (b)
                      ((fn (c) (a (b c))) 10))
                   (fn (num) (a num))))
               (fn (num) (* num num))) (num 10000))
(test-interp `{let-stx {or = {[{or a b} => {{fn {temp} {if temp temp b}} a}]}}
                        {or false {+ 12 1}}}
             (num 13))
(test-interp `{let-stx {rec = {[{rec a b} => {{+ rec a} {rec b}}]
                                [{rec a} => {* 2 a}]}}
                        {let-stx {if-not-a = {[{if-not-a a b} => {if a b a}]}}
                                 {rec {if-not-a false 10} 12}}}
             (num 44))
(test (expand `{let-stx {nothing = {}}
                        {nothing 12 2}})
      `{nothing 12 2})

;; Exception testing
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