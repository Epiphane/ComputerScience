#lang plai-typed

(print-only-errors #t)
(require plai-typed/s-exp-match)

;; Gucci Lang v4
;; EBNF Specification:
;; GUCI4 = num
;;       | true
;;       | false
;;       | id
;;       | {new-array GUCI4 GUCI4}
;;       | {ref GUCI4[GUCI4]}
;;       | {GUCI4[GUCI4] <- GUCI4}
;;       | {id <- GUCI4}
;;       | {begin GUCI4 GUCI4 ...}
;;       | {if GUCI4 GUCI4 GUCI4}
;;       | {with {id = GUCI4} ... GUCI4}
;;       | {fn {id ...} GUCI4}
;;       | {operator GUCI4 GUCI4}
;;       | {GUCI4 GUCI4 ...}
;; operator	=	+, -, *, /, eq?, <=

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Types
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Represents a value
(define-type Value
  [num (n : number)]
  [bool (b : boolean)]
  [array (start : number) (l : number)]
  [clos (args : (listof symbol)) (body : ExprC) (env : Environment)])

;; Represents an expression
(define-type ExprC
  [numC (n : number)]
  [idC (i : symbol)]
  [boolC (b : boolean)]
  [new-arrayC (len : ExprC) (val : ExprC)]
  [arr-setC (arr : ExprC) (ndx : ExprC) (val : ExprC)]
  [refC (arr : ExprC) (ndx : ExprC)]
  [binop (op : (Value Value -> Value)) (l : ExprC) (r : ExprC)]
  [ifC (cond : ExprC) (success : ExprC) (fail : ExprC)]
  [appC (fun : ExprC) (args : (listof ExprC))]
  [lam (args : (listof symbol)) (body : ExprC)]
  [beginC (tasks : (listof ExprC))]
  [setC (var : symbol) (val : ExprC)])

;; Store
(define-type-alias Location number)
(define-type Storage
  [cell (location : Location) (val : Value)])

(define-type-alias Sto ((listof Storage) * Location))

;; Fetch value from the store
(define (fetch [loc : Location] [sto : Sto]) : Value
  (local [(define (search loc cells) : Value
            (if (equal? (cell-location (first cells)) loc)
                (cell-val (first cells))
                (search loc (rest cells))))]
    (search loc (fst sto))))

;; Insert val into store
(define (override-store [val : Storage] [sto : Sto]) : Sto 
  (values (cons val (fst sto)) (snd sto)))

(define (empty-sto) : Sto
  (values (list) 0))

;; Helper function to allocate array space in the store
(define (allocate [sto : Sto] [space : number] [init : Value]) : (number * Sto)
  (local [(define (insert-item [left : number] [sto : Sto]) : Sto
            (if (equal? left 0)
                sto
                (insert-item (- left 1)
                             (values (cons 
                                      (cell (snd sto) init) 
                                      (fst sto)) (add1 (snd sto))))))]
    (values (snd sto) (insert-item space sto))))

;; Represents an environment
(define-type-alias Environment (hashof symbol Location))

;; the empty environment
(define empty-env (hash (list)))

;; Value-store type
(define-type V*S
  [v*s (v : Value) (s : Sto)])

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Serialization
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (serialize [val : Value])
  (type-case Value val
    [num (n) (to-string n)]
    [bool (b) (cond
                [b "true"]
                [else "false"])]
    [clos (a b e) "#<procedure>"]
    [array (s l) "#<array>"]))

;; Test cases
(test (serialize (num 32)) "32")
(test (serialize (bool true)) "true")
(test (serialize (bool false)) "false")
(test (serialize (clos (list 'a) (numC 2) empty-env)) "#<procedure>")
(test (serialize (array 3 4)) "#<array>")

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
                                            (error 'eval "Division by zero")
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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Parsing
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Reserved symbols
(define reserved (list `fn `true `false `if `with `+ `- `* `/ `<= `eq? `=))

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


(define (parse [expr : s-expression]) : ExprC
  (cond
    [(s-exp-match? `NUMBER expr) (numC (s-exp->number expr))]
    [(s-exp-match? `true expr) (boolC true)]
    [(s-exp-match? `false expr) (boolC  false)]
    [(s-exp-match? `SYMBOL expr) 
     (if (member expr reserved)
         (error 'parse "Reserved word cannot be used as identifier")
         (idC (s-exp->symbol expr)))]
    [(s-exp-match? `(new-array ANY ANY) expr)
     (new-arrayC (parse (second (s-exp->list expr)))
                 (parse (third (s-exp->list expr))))]
    [(s-exp-match? `(ref ANY[ANY]) expr)
     (refC (parse (second (s-exp->list expr)))
           (parse (first (s-exp->list (third (s-exp->list expr))))))]
    [(s-exp-match? `(ANY[ANY] <- ANY) expr)
     (arr-setC (parse (first (s-exp->list expr)))
               (parse (first (s-exp->list (second (s-exp->list expr)))))
               (parse (fourth (s-exp->list expr))))]
    [(s-exp-match? `(SYMBOL <- ANY) expr)
     (setC (s-exp->symbol (first (s-exp->list expr)))
           (parse (third (s-exp->list expr))))]
    [(s-exp-match? `(with {SYMBOL = ANY} ... ANY) expr)
     (appC
      (lam (fn-args (map (lambda (arg) (first (s-exp->list arg)))
                               (reverse (rest (reverse (rest (s-exp->list expr)))))))
                 (parse (first (reverse (s-exp->list expr)))))
      (map (lambda (arg) (parse (third (s-exp->list arg))))
           (reverse (rest (reverse (rest (s-exp->list expr)))))))]
    [(s-exp-match? `(if ANY ANY ANY) expr)
     (ifC (parse (second (s-exp->list expr)))
          (parse (third (s-exp->list expr)))
          (parse (fourth (s-exp->list expr))))]
    [(s-exp-match? `(fn {SYMBOL ...} ANY) expr)
     (lam (fn-args (s-exp->list (second (s-exp->list expr))))
                (parse (third (s-exp->list expr))))]
    [(s-exp-match? `(begin ANY ...) expr)
     (beginC (map (lambda (exp) (parse exp)) (rest (s-exp->list expr))))]
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


;; Test cases
(test (parse `true) (boolC true))
(test (parse `false) (boolC false))
(test (parse `134) (numC 134))
(test (parse `abc) (idC 'abc))
(test (serialize (clos (list) (parse `(+ 4 5)) empty-env)) "#<procedure>")
(test (parse `(f 5 6)) (appC (idC 'f) (list (numC 5) (numC 6))))
(test (parse `{new-array 5 1}) (new-arrayC (numC 5) (numC 1)))
(test (parse `{ref (new-array 2 0)[1]}) (refC (new-arrayC (numC 2) (numC 0)) (numC 1)))
(test (parse `{(new-array 2 0)[1] <- 10})
      (arr-setC (new-arrayC (numC 2) (numC 0)) (numC 1) (numC 10)))
(test (parse `(with {arr = (new-array 3 0)}
                    {arr[1] <- {ref arr[1]}}))
      (appC (lam (list 'arr) (arr-setC (idC 'arr) 
                                       (numC 1) 
                                       (refC (idC 'arr) (numC 1)))) (list (new-arrayC (numC 3) (numC 0)))))
(test (parse `({fn (a b) (a <- b)} 6 4))
      (appC (lam (list 'a 'b) (setC 'a (idC 'b))) (list (numC 6) (numC 4))))
(test (parse `(with {a = 10}
                    (begin (a <- 2)
                           (a <- 4)
                           (a <- 6)))) 
      (appC (lam (list 'a) (beginC (list (setC 'a (numC 2))
                                         (setC 'a (numC 4))
                                         (setC 'a (numC 6)))))
            (list (numC 10))))
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
(test/exn (parse `(with {a = 5})) "Reserved word cannot be used as identifier")
(test/exn (parse `(fn {* a a})) "Reserved word cannot be used as identifier")
(test/exn (parse `(if (<= 5 10) 'a 'b 'c)) "Reserved word cannot be used as identifier")
(test/exn (parse `if) "Reserved word cannot be used as identifier")
(test/exn (parse `(if 1)) "Reserved word cannot be used as identifier")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Evaluation
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Find a symbol in the given environment
(define (find-symbol [name : symbol] [env : Environment] [sto : Sto]) : Value
  (type-case (optionof Location) (hash-ref env name)
    [some (l) (fetch l sto)]
    [else (error 'unbound-identifier (string-append "Unbound Identifier "
                                                    (symbol->string name)))]))

;; Helper to extract number from a Value
(define (get-num [v : Value]) : number
  (type-case Value v
    [num (n) n]
    [else (not-a "number" v)]))

;; Add list of arguments to an environment
(define (env-vars [args : (listof symbol)] 
                  [evals : (listof ExprC)]
                  [env : Environment]
                  [sto : Sto]
                  [result : Environment]) : (Environment * Sto)
  (cond
    [(and (empty? args) (empty? evals)) (values result sto)]
    [(or (empty? args) (empty? evals))
     (error 'num-args "Incorrect number of arguments")]
    [else 
     (type-case V*S (interp (first evals) env sto)
       [v*s (val sto1)
            (let [(res (allocate sto1 1 val))]
              (env-vars (rest args)
                        (rest evals)
                        env (snd res)
                        (hash-set result (first args) (fst res))))])]))

;; Error helper
(define (not-a [type : string] [val : Value])
  (error 'type (string-append (string-append "Not a " type)
                              (string-append ": " (serialize val)))))

;; Interpret multiple ExprCs (for begin)
(define (interp-exprs [exprs : (listof ExprC)] [env : Environment] [sto : Sto]) : V*S
  (cond
    [(empty? exprs) (error 'interp  "No expressions to execute")]
    [(equal? (length exprs) 1) (interp (first exprs) env sto)]
    [else (type-case V*S (interp (first exprs) env sto)
            [v*s (v sto1) (interp-exprs (rest exprs) env sto1)])]))

;; Interpret an ExprC
(define (interp [e : ExprC] [env : Environment] [sto : Sto]) : V*S
  (type-case ExprC e
    [numC (n) (v*s (num n) sto)]
    [boolC (b) (v*s (bool b) sto)]
    [idC (i) (v*s (find-symbol i env sto) sto)]
    [binop (op a b) (type-case V*S (interp a env sto)
                      [v*s (l sto1) (type-case V*S (interp b env sto1)
                                      [v*s (r sto2) (v*s (op l r) sto2)])])]
    [ifC (test success fail)
         (type-case V*S (interp test env sto)
           [v*s (v sto1)
                (type-case Value v
                  [bool (b) (if b (interp success env sto1) (interp fail env sto1))]
                  [else (not-a "boolean" v)])])]
    [new-arrayC (l v) (type-case V*S (interp l env sto)
                        [v*s (len sto1) (type-case V*S (interp v env sto1)
                                          [v*s (val sto2) 
                                               (let ([res (allocate sto (get-num len) val)])
                                                 (v*s (array (fst res) (get-num len)) (snd res)))])])]
    [refC (a n) (type-case V*S (interp a env sto)
                  [v*s (arr sto1) 
                       (type-case Value arr
                         [array (start length)
                                (type-case V*S (interp n env sto1)
                                  [v*s (ndx sto2)
                                       (type-case Value ndx
                                         [num (ndx)
                                              (if (< ndx length)
                                                  (v*s (fetch (+ start ndx) sto2) sto2)
                                                  (error 'bounds (string-append
                                                                  "Array index out of bounds: "
                                                                  (serialize (num ndx)))))]
                                         [else (not-a "number" ndx)])])]
                         [else (not-a "array" arr)])])]
    [arr-setC (a n v) (type-case V*S (interp a env sto)
                        [v*s (arr sto1) 
                             (type-case Value arr
                               [array (start length)
                                      (type-case V*S (interp n env sto1)
                                        [v*s (ndx sto2)
                                             (type-case Value ndx
                                               [num (ndx)
                                                    (if (< ndx length)
                                                        (type-case V*S (interp v env sto2)
                                                          [v*s (val sto3)
                                                               (v*s val
                                                                    (override-store (cell (+ start ndx) val) sto3))])
                                                        (error 'bounds (string-append
                                                                        "Array index out of bounds: "
                                                                        (serialize (num ndx)))))]
                                               [else (not-a "number" ndx)])])]
                               [else (not-a "array" arr)])])]
    [appC (f a) (type-case V*S (interp f env sto)
                  [v*s (fun sto1)
                       (type-case Value fun
                         [clos (args body clos-env)
                               (let [(env*sto (env-vars args a env sto1 clos-env))]
                                 (interp body (fst env*sto) (snd env*sto)))]
                         [else (not-a "function" fun)])])]
    [lam (args body) (v*s (clos args body env) sto)]
    [setC (var val) (type-case V*S (interp val env sto)
                        [v*s (value sto1) 
                             (type-case (optionof Location) (hash-ref env var)
                               [some (l) (v*s value (override-store (cell l value) sto1))]
                               [else (error 'unbound-identifier (string-append "Unbound Identifier "
                                                                               (symbol->string var)))])])]
    [beginC (exprs) (interp-exprs exprs env sto)]))

;; Top-eval
(define (top-eval [s : s-expression]) : string
  (serialize (v*s-v (interp (parse s) empty-env (empty-sto)))))

;; Test cases
;; Environment
(define (test-interp [expr : s-expression])
  (v*s-v (interp (parse expr) empty-env (empty-sto))))

;; Basic EBNF Tests
(test (test-interp `false) (bool false))
(test (test-interp `(+ 4 7)) (num 11))
(test (test-interp `(/ 3 1)) (num 3))
(test (test-interp `(if (eq? (- 10 5) 5) 10 11)) (num 10))
(test (test-interp `(with (val = 10)
                    (fun = (fn (a b) (* a b)))
                    (fun val 7))) (num 70))
(test (test-interp `(with (val = (if (<= (/ 9 3) (+ 1 1)) 10 11))
                    val)) (num 11))
(test (test-interp `((fn (seven) (seven))
                     ((fn (minus) (fn () (minus (+ 3 10) (* 2 3))))
                      (fn (x y) (+ x (* -1 y)))))) (num 7))
(test (test-interp `((fn (a) 
                   ((fn (b)
                        ((fn (c) (a (b c))) 10))
                    (fn (num) (a num))))
               (fn (num) (* num num)))) (num 10000))
(test (test-interp `{new-array (+ 2 3) 1}) (array 0 5))
(test (test-interp `{ref (new-array 2 0)[0]}) (num 0))
(test (test-interp `{ref (new-array 2 0)[1]}) (num 0))
(test (test-interp `{(new-array 3 0)[1] <- 2})
             (num 2))
(test (test-interp `(with {arr = (new-array 3 0)}
                    {arr[1] <- (+ {ref arr[1]} 2)}))
             (num 2))
(test (test-interp `({fn (a b) (a <- b)} 6 4))
             (num 4))
(test (test-interp `(with {a = 10}
                    (begin (a <- (/ a a))
                           (+ a 4)
                           (- a 2))))
             (num -1))
(test (test-interp `(with (a = (+ 4 6)) (b = 12) (* a b))) (num 120))
(test (test-interp `(fn {} 6)) (clos (list) (numC 6) empty-env))

;; Exception testing
(test/exn (test-interp `(l <- 12)) "Unbound Identifier")
(test/exn (test-interp `(ref (new-array 10 3)[15])) "Array index out of bounds: 15")
(test/exn (test-interp `((new-array 10 3)[15] <- 9)) "Array index out of bounds: 15")
(test/exn (test-interp `(ref 10[15])) "Not a array: 10")
(test/exn (test-interp `(10[15] <- 9)) "Not a array: 10")
(test/exn (test-interp `(ref (new-array 10 3)[true])) "Not a number: true")
(test/exn (test-interp `((new-array 10 3)[true] <- 9)) "Not a number: true")
(test/exn (test-interp `(with (res = (if (<= (/ 9 3) (+ 1 1)) 10 11))
                        (res))) "Not a function: 11")
(test/exn (interp (appC (numC 10) (list (numC 12))) empty-env (empty-sto))
          "Not a function: 10")
(test/exn (test-interp `(if {<= a b} 19 (if {eq? c d} 20 21)))
                 "Unbound Identifier")
(test/exn (test-interp `(new-array true 10)) "Not a number: true")
(test/exn (test-interp `(/ true 0)) "Not a number: true")
(test/exn (test-interp `(if 10 1 0)) "Not a boolean: 10")
(test/exn (test-interp `(/ 12 0)) "Division by zero")
(test/exn (test-interp `ab) "Unbound Identifier")
(test/exn (test-interp `(ab 12)) "Unbound Identifier")
(test/exn (test-interp `(+ / 2)) "Reserved word cannot be used as identifier")
(test/exn (test-interp `(if (+ 10 2) 'a 'b)) "Not a boolean: 12")
(test/exn (test-interp `(bad-fn 2)) "Unbound Identifier")
(test/exn (test-interp `(+ a b)) "Unbound Identifier")
(test/exn (test-interp `((fn () 6) 1 2 3)) "Incorrect number of arguments")
(test/exn (test-interp `(begin)) "No expressions to execute")

(test (top-eval `(eq? (new-array 2 2) (new-array 2 3))) "false")
(test (top-eval `(with 
                  (make-incr = (fn (x) (fn () (begin (x <- (+ x 1)) x)))) 
                  (with (incr = (make-incr 23)) 
                        (begin (incr) 
                               (incr) 
                               (incr))))) "26")
(test (top-eval
       '{with {m1 = {fn {f n} {if {<= n 0} -1 {+ 1 {f f {- n 1}}}}}}
              {m1 m1 500}})
      "499")

(top-eval `(with (a = 0) 
                 (with (a! = (fn (expected) (if (eq? a expected) (a <- (+ 1 a)) (/ 1 0))))
                       (begin (+ (a! 0) (a! 1))
                              (if (begin (a! 2) true) 
                                  (a! 3)
                                  (/ 1 0))
                              (new-array (a! 4) (fn () (a! 5)))))))

(test (top-eval `(with (a = 1)
                       (b = 23)
                       (array-length = 10)
                       (array-initVal = false)
                       (with (my-array = (new-array array-length array-initVal))
                             (diff = (fn (x y) (- x y)))
                             (begin ((if (eq? (diff (+ 3 4) (* 2 3)) 1) a b) <- 19)
                                    (my-array[b] <- (if (<= a b) (+ a b) (- a b)))
                                    (ref my-array[b])))))
                "42")