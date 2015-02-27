#lang plai-typed

(print-only-errors #t)
(require plai-typed/s-exp-match)

;; Gucci Lang v4
;; EBNF Specification:
;; ClassDef = {class id extends id {fields id ...} {method id {id ...} GUCI5} ...}
;; GUCI4 = num
;;       | true
;;       | false
;;       | string
;;       | this
;;       | id
;;       | {array GUCI5 GUCI5}
;;       | {ref GUCI5[GUCI5]}
;;       | {GUCI5[GUCI5] <- GUCI5}
;;       | {id <- GUCI5}
;;       | {begin GUCI5 GUCI5 ...}
;;       | {if GUCI5 GUCI5 GUCI5}
;;       | {with {id = GUCI5} ... GUCI5}
;;       | {fn {id ...} GUCI5}
;;       | {operator GUCI5 GUCI5}
;;       | {rec id GUCI5 ...}
;;       | {new id GUCI5 ...}
;;       | {send GUCI5 id GUCI5 ...}
;;       | {GUCI5 GUCI5 ...}
;; operator = +, -, *, /, eq?, <=

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Types
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Represents a value
(define-type Value
  [str (s : string)]
  [num (n : number)]
  [bool (b : boolean)]
  [array (start : number) (l : number)]
  [clos (args : (listof symbol)) (body : ExprC) (env : Environment)])

;; Represents an expression
(define-type ExprC
  [numC (n : number)]
  [strC (s : string)]
  [idC (i : symbol)]
  [boolC (b : boolean)]
  [arrayC (len : ExprC) (val : ExprC)]
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

(define empty-sto : Sto
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
(define-type A*S
  [a*s (v : Value) (s : Sto)])

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Serialization
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (serialize [val : Value])
  (type-case Value val
    [str (s) s]
    [num (n) (to-string n)]
    [bool (b) (cond
                [b "true"]
                [else "false"])]
    [clos (a b e) "#<procedure>"]
    [array (s l) "#<array>"]))

;; Test cases
(test (serialize (str "Hello world")) "Hello world")
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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Parsing
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Reserved symbols
(define reserved (list `fn `true `false `if `with `array `=
                       `begin `rec `new `send `class `extends
                       `fields `method `+ `- `* `/ `eq? `<=))

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

(define-type Class
  [class (fields : (listof symbol)) (constructor : ExprC)])
(define-type-alias ClassMap (hashof symbol Class))

;; Object class
(define basic-classes : ClassMap
  (hash 
   (list (values 'Object
                 (class empty
                   (lam empty (lam (list 'methodname) (idC 'method-doesnt-exist))))))))

;; Parse class methods
;; Method structure: {method id {id ...} GUCI5}
(define (class-methods [methods : (listof s-expression)]) : ExprC
  (cond
    [(empty? methods) (appC (idC 'parent) (list (idC 'methodname) (idC 'this)))]
    [else (ifC (binop (get-arith-op `eq?) (idC 'methodname) (strC (symbol->string (s-exp->symbol (second (s-exp->list (first methods)))))))
               (lam 
                (map s-exp->symbol (s-exp->list (third (s-exp->list (first methods)))))
                (parse (fourth (s-exp->list (first methods)))))
               (class-methods (rest methods)))]))

;; Combines a list of fields
(define (list-combine [l1 : (listof symbol)] [l2 : (listof symbol)]) : (listof symbol)
  (cond
    [(empty? l1) l2]
    [else (cons (first l1) (list-combine (rest l1) l2))]))

;; Parse a class definition
(define (parse-class [classes : ClassMap] [classdef : s-expression]) : ClassMap
  (if (s-exp-match? `{class SYMBOL extends SYMBOL
                       {fields SYMBOL ...}
                       {method SYMBOL {SYMBOL ...} ANY} ...} classdef)
      (let ([pieces (s-exp->list classdef)])
        (type-case (optionof Class) (hash-ref classes (s-exp->symbol (fourth pieces)))
          [some (c)
                (type-case Class c
                  [class (parent-fields parent-constructor)
                    (let ([combined-fields (list-combine parent-fields (map s-exp->symbol (rest (s-exp->list (first (rest (rest (rest (rest pieces)))))))))])
                      (hash-set classes (s-exp->symbol (second pieces))
                                (class combined-fields
                                  (lam combined-fields
                                       (appC (lam (list 'parent)
                                                  (lam 
                                                   (list 'methodname 'this)
                                                   (class-methods (rest (rest (rest (rest (rest pieces))))))))
                                             (list (appC (idC (s-exp->symbol (fourth pieces)))
                                                         (map (lambda (sym) (idC sym)) parent-fields))))))))])]
          [none () (error 'class-ref (string-append (symbol->string (s-exp->symbol (fourth pieces)))
                                                    " not declared"))]))
      (error 'parse "Function does not match classdef specification")))

;; Parse an s-expression
(define (parse [expr : s-expression])
  (local [(define (recur e) (parse e))]
    (cond
      [(s-exp-match? `NUMBER expr) (numC (s-exp->number expr))]
      [(s-exp-match? `STRING expr) (strC (s-exp->string expr))]
      [(s-exp-match? `true expr) (boolC true)]
      [(s-exp-match? `false expr) (boolC  false)]
      [(s-exp-match? `SYMBOL expr) 
       (if (member expr reserved)
           (error 'parse "Reserved word cannot be used as identifier")
           (idC (s-exp->symbol expr)))]
      ; new array
      [(s-exp-match? `(new-array ANY ANY) expr)
       (arrayC (recur (second (s-exp->list expr)))
                   (recur (third (s-exp->list expr))))]
      ; ref
      [(s-exp-match? `(ref ANY[ANY]) expr)
       (refC (recur (second (s-exp->list expr)))
             (recur (first (s-exp->list (third (s-exp->list expr))))))]
      ; array set
      [(s-exp-match? `(ANY[ANY] <- ANY) expr)
       (arr-setC (recur (first (s-exp->list expr)))
                 (recur (first (s-exp->list (second (s-exp->list expr)))))
                 (recur (fourth (s-exp->list expr))))]
      ; set
      [(s-exp-match? `(SYMBOL <- ANY) expr)
       (setC (s-exp->symbol (first (s-exp->list expr)))
             (recur (third (s-exp->list expr))))]
      ; with
      [(s-exp-match? `(with {SYMBOL = ANY} ... ANY) expr)
       (appC
        (lam (fn-args (map (lambda (arg) (first (s-exp->list arg)))
                           (reverse (rest (reverse (rest (s-exp->list expr)))))))
             (recur (first (reverse (s-exp->list expr)))))
        (map (lambda (arg) (recur (third (s-exp->list arg))))
             (reverse (rest (reverse (rest (s-exp->list expr)))))))]
      ; rec
      [(s-exp-match? `(rec {SYMBOL = ANY} ANY) expr)
       (let [(fn-name (s-exp->symbol (first (s-exp->list (second (s-exp->list expr))))))]
         (if (member (symbol->s-exp fn-name) reserved)
             (error 'argument "Reserved word cannot be used as identifier")
             (appC
              (lam (list (s-exp->symbol (first (s-exp->list (second (s-exp->list expr))))))
                   (recur (third (s-exp->list expr))))
              (list (appC (lam (list fn-name)
                               (setC fn-name (recur (third (s-exp->list (second (s-exp->list expr)))))))
                          (list (numC 0)))))))]
      ; if
      [(s-exp-match? `(if ANY ANY ANY) expr)
       (ifC (recur (second (s-exp->list expr)))
            (recur (third (s-exp->list expr)))
            (recur (fourth (s-exp->list expr))))]
      ; fn
      [(s-exp-match? `(fn {SYMBOL ...} ANY) expr)
       (lam (fn-args (s-exp->list (second (s-exp->list expr))))
            (parse (third (s-exp->list expr))))]
      ; begin
      [(s-exp-match? `(begin ANY ...) expr)
       (beginC (map (lambda (exp) (recur exp)) (rest (s-exp->list expr))))]
      ; send
      [(s-exp-match? `(send ANY SYMBOL ANY ...) expr)
       (appC (appC (recur (second (s-exp->list expr))) 
                   (list (strC (symbol->string (s-exp->symbol (third (s-exp->list expr)))))
                         (recur (second (s-exp->list expr)))))
             (map recur (rest (rest (rest (s-exp->list expr))))))]
      ; new
      [(s-exp-match? `(new SYMBOL ANY ...) expr)
       (appC (idC (s-exp->symbol 
                   (second (s-exp->list expr))))
             (map recur (rest (rest (s-exp->list expr)))))]
      ; other function
      [(s-exp-match? `(SYMBOL ANY ANY) expr)
       (type-case (optionof (Value Value -> Value)) (binary-op (first (s-exp->list expr)))
         [some (fun) (binop fun
                            (recur (second (s-exp->list expr)))
                            (recur (third (s-exp->list expr))))]
         [none () 
               (cond [(member (first (s-exp->list expr)) reserved)
                      (error 'recur "Syntax does not match EBNF spec")]
                     [else (appC (idC (s-exp->symbol 
                                       (first (s-exp->list expr))))
                                 (map (lambda (arg) (recur arg))
                                      (rest (s-exp->list expr))))])])]
      [(s-exp-match? `(SYMBOL ANY ...) expr)
       (if (member (first (s-exp->list expr)) reserved)
           (error 'parse (string-append "Reserved word " (string-append (symbol->string (s-exp->symbol (first (s-exp->list expr)))) " cannot be used as identifier")))
           (appC (idC (s-exp->symbol (first (s-exp->list expr))))
                 (map (lambda (arg) (recur arg))
                      (rest (s-exp->list expr)))))]
      [(s-exp-match? `(ANY ...) expr)
       (appC (recur (first (s-exp->list expr)))
             (map (lambda (arg) (recur arg))
                  (rest (s-exp->list expr))))])))

;; Inject class definitions into expr
(define (inject-and-parse [classnames : (listof symbol)] [classes : ClassMap] [expr : s-expression]) : ExprC
  (cond
    [(empty? classnames) (parse expr)]
    [else
     (appC (lam (list (first classnames))
                     (inject-and-parse (rest classnames) classes expr))
                (list
                 (type-case (optionof Class) (hash-ref classes (first classnames))
                   [some (s) (appC (lam (list (first classnames))
                                        (setC (first classnames) (class-constructor s)))
                                   (list (numC 0)))]
                   [none () (error 'inject "Class not found")])))]))
 
;; Not a use case - just for test coverage
(test/exn (inject-and-parse (list 'dummy) basic-classes `true) "Class not found")

;; Parse a program with class definitions
(define (parse-prog [classdefs : (listof s-expression)] [expr : s-expression]) : ExprC
  (if (empty? classdefs)
      (parse expr)
      (local [(define (parse-classes classes defs)
                (cond
                  [(empty? defs) classes]
                  [else (parse-classes (parse-class classes (first defs)) (rest defs))]))]
        (inject-and-parse 
         (cons 'Object (map (lambda (def) (s-exp->symbol (second (s-exp->list def)))) classdefs))
         (parse-classes basic-classes classdefs) 
         expr))))

;; Test cases
(test (parse-prog empty `true) (boolC true))
(test (parse-prog empty `false) (boolC false))
(test (parse-prog empty `134) (numC 134))
(test (parse-prog empty `abc) (idC 'abc))
(test (parse-prog empty `"abc") (strC "abc"))
(test (parse-prog empty `(f 5 6)) (appC (idC 'f) (list (numC 5) (numC 6))))
(test (parse-prog empty `{new-array 5 1}) (arrayC (numC 5) (numC 1)))
(test (parse-prog empty `{ref (new-array 2 0)[1]}) (refC (arrayC (numC 2) (numC 0)) (numC 1)))
(test (parse-prog empty `{(new-array 2 0)[1] <- 10})
      (arr-setC (arrayC (numC 2) (numC 0)) (numC 1) (numC 10)))
(test (parse-prog empty `(with {arr = (new-array 3 0)}
                    {arr[1] <- {ref arr[1]}}))
      (appC (lam (list 'arr) (arr-setC (idC 'arr) 
                                       (numC 1) 
                                       (refC (idC 'arr) (numC 1)))) (list (arrayC (numC 3) (numC 0)))))
(test (parse-prog empty `({fn (a b) (a <- b)} 6 4))
      (appC (lam (list 'a 'b) (setC 'a (idC 'b))) (list (numC 6) (numC 4))))
(test (parse-prog empty `(with {a = 10}
                    (begin (a <- 2)
                           (a <- 4)
                           (a <- 6)))) 
      (appC (lam (list 'a) (beginC (list (setC 'a (numC 2))
                                         (setC 'a (numC 4))
                                         (setC 'a (numC 6)))))
            (list (numC 10))))
(test (parse-prog empty `(fn {} 6)) (lam (list) (numC 6)))

;; Exception testing
(test/exn (parse-prog (list `(class fake)) `(new fake)) "Function does not match classdef specification")
(test/exn (parse-prog (list `(class fake extends Nothing {fields a})) `(new fake)) "Nothing not declared")
(test/exn (parse-prog empty `(if 4 5)) "Syntax does not match EBNF spec")
(test/exn (parse-prog empty `(rec (with = 34) 3)) "Reserved word cannot be used as identifier")
(test/exn (parse-prog empty `(fn {a -} {- a -})) "Reserved word cannot be used as identifier")
(test/exn (parse-prog empty `(fn {a a} {- a a})) "Argument identifiers are not unique")
(test/exn (parse-prog empty `(13 - 10)) "Reserved word cannot be used as identifier")
(test/exn (parse-prog empty `(with {a = 5})) "Reserved word cannot be used as identifier")
(test/exn (parse-prog empty `(fn {* a a})) "Reserved word fn cannot be used as identifier")
(test/exn (parse-prog empty `(if (<= 5 10) 'a 'b 'c)) "Reserved word if cannot be used as identifier")
(test/exn (parse-prog empty `if) "Reserved word cannot be used as identifier")
(test/exn (parse-prog empty `(if 1)) "Reserved word if cannot be used as identifier")

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
     (type-case A*S (interp (first evals) env sto)
       [a*s (val sto1)
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
(define (interp-exprs [exprs : (listof ExprC)] [env : Environment] [sto : Sto]) : A*S
  (cond
    [(empty? exprs) (error 'interp  "No expressions to execute")]
    [(equal? (length exprs) 1) (interp (first exprs) env sto)]
    [else (type-case A*S (interp (first exprs) env sto)
            [a*s (v sto1) (interp-exprs (rest exprs) env sto1)])]))

;; Interpret an ExprC
(define (interp [e : ExprC] [env : Environment] [sto : Sto]) : A*S
  (type-case ExprC e
    [strC (s) (a*s (str s) sto)]
    [numC (n) (a*s (num n) sto)]
    [boolC (b) (a*s (bool b) sto)]
    [idC (i) (a*s (find-symbol i env sto) sto)]
    [binop (op a b) (type-case A*S (interp a env sto)
                      [a*s (l sto1) (type-case A*S (interp b env sto1)
                                      [a*s (r sto2) (a*s (op l r) sto2)])])]
    [ifC (test success fail)
         (type-case A*S (interp test env sto)
           [a*s (v sto1)
                (type-case Value v
                  [bool (b) (if b (interp success env sto1) (interp fail env sto1))]
                  [else (not-a "boolean" v)])])]
    [arrayC (l v) (type-case A*S (interp l env sto)
                        [a*s (len sto1) (type-case A*S (interp v env sto1)
                                          [a*s (val sto2) 
                                               (let ([res (allocate sto2 (get-num len) val)])
                                                 (a*s (array (fst res) (get-num len)) (snd res)))])])]
    [refC (a n) (type-case A*S (interp a env sto)
                  [a*s (arr sto1) 
                       (type-case Value arr
                         [array (start length)
                                (type-case A*S (interp n env sto1)
                                  [a*s (ndx sto2)
                                       (type-case Value ndx
                                         [num (ndx)
                                              (if (< ndx length)
                                                  (a*s (fetch (+ start ndx) sto2) sto2)
                                                  (error 'bounds (string-append
                                                                  "Array index out of bounds: "
                                                                  (serialize (num ndx)))))]
                                         [else (not-a "number" ndx)])])]
                         [else (not-a "array" arr)])])]
    [arr-setC (a n v) (type-case A*S (interp a env sto)
                        [a*s (arr sto1) 
                             (type-case Value arr
                               [array (start length)
                                      (type-case A*S (interp n env sto1)
                                        [a*s (ndx sto2)
                                             (type-case Value ndx
                                               [num (ndx)
                                                    (if (< ndx length)
                                                        (type-case A*S (interp v env sto2)
                                                          [a*s (val sto3)
                                                               (a*s val
                                                                    (override-store (cell (+ start ndx) val) sto3))])
                                                        (error 'bounds (string-append
                                                                        "Array index out of bounds: "
                                                                        (serialize (num ndx)))))]
                                               [else (not-a "number" ndx)])])]
                               [else (not-a "array" arr)])])]
    [appC (f a) (type-case A*S (interp f env sto)
                  [a*s (fun sto1)
                       (type-case Value fun
                         [clos (args body clos-env)
                               (let [(ena*sto (env-vars args a env sto1 clos-env))]
                                 (interp body (fst ena*sto) (snd ena*sto)))]
                         [else (not-a "function" fun)])])]
    [lam (args body) (a*s (clos args body env) sto)]
    [setC (var val) (type-case A*S (interp val env sto)
                        [a*s (value sto1) 
                             (type-case (optionof Location) (hash-ref env var)
                               [some (l) (a*s value (override-store (cell l value) sto1))]
                               [else (error 'unbound-identifier (string-append "Unbound Identifier "
                                                                               (symbol->string var)))])])]
    [beginC (exprs) (interp-exprs exprs env sto)]))
;; Test cases
(define (test-interp [classes : (listof s-expression)] [body : s-expression]) : Value
  (a*s-v (interp (parse-prog classes body) empty-env empty-sto)))

;; Top Eval
(define (top-eval [classes : (listof s-expression)] [body : s-expression]) : string
  (serialize (a*s-v (interp (parse-prog classes body) empty-env empty-sto))))

;; Basic EBNF Tests
(test (test-interp empty `false) (bool false))
(test (test-interp empty `(+ 4 7)) (num 11))
(test (test-interp empty `(/ 3 1)) (num 3))
(test (test-interp empty `(if (eq? (- 10 5) 5) 10 11)) (num 10))
(test (test-interp empty `(with (val = 10)
                                (fun = (fn (a b) (* a b)))
                                (fun val 7))) (num 70))
(test (test-interp empty `(with (val = (if (<= (/ 9 3) (+ 1 1)) 10 11))
                                val)) (num 11))
(test (test-interp empty `((fn (seven) (seven))
                           ((fn (minus) (fn () (minus (+ 3 10) (* 2 3))))
                            (fn (x y) (+ x (* -1 y)))))) (num 7))
(test (test-interp empty `((fn (a) 
                               ((fn (b)
                                    ((fn (c) (a (b c))) 10))
                                (fn (num) (a num))))
                           (fn (num) (* num num)))) (num 10000))
(test (test-interp empty `{new-array (+ 2 3) 1}) (array 0 5))
(test (test-interp empty `{ref (new-array 2 0)[0]}) (num 0))
(test (test-interp empty `{ref (new-array 2 0)[1]}) (num 0))
(test (test-interp empty `{(new-array 3 0)[1] <- 2})
      (num 2))
(test (test-interp empty `(with {arr = (new-array 3 0)}
                                {arr[1] <- (+ {ref arr[1]} 2)}))
      (num 2))
(test (test-interp empty `({fn (a b) (a <- b)} 6 4))
      (num 4))
(test (test-interp empty `(with {a = 10}
                                (begin (a <- (/ a a))
                                       (+ a 4)
                                       (- a 2))))
      (num -1))
(test (test-interp empty `(with (a = (+ 4 6)) (b = 12) (* a b))) (num 120))
(test (test-interp empty `(fn {} 6)) (clos (list) (numC 6) empty-env))

;; Exception testing
(test/exn (test-interp empty `(l <- 12)) "Unbound Identifier")
(test/exn (test-interp empty `(ref (new-array 10 3)[15])) "Array index out of bounds: 15")
(test/exn (test-interp empty `((new-array 10 3)[15] <- 9)) "Array index out of bounds: 15")
(test/exn (test-interp empty `(ref 10[15])) "Not a array: 10")
(test/exn (test-interp empty `(10[15] <- 9)) "Not a array: 10")
(test/exn (test-interp empty `(ref (new-array 10 3)[true])) "Not a number: true")
(test/exn (test-interp empty `((new-array 10 3)[true] <- 9)) "Not a number: true")
(test/exn (test-interp empty `(with (res = (if (<= (/ 9 3) (+ 1 1)) 10 11))
                        (res))) "Not a function: 11")
(test/exn (interp (appC (numC 10) (list (numC 12))) empty-env empty-sto)
          "Not a function: 10")
(test/exn (test-interp empty `(if {<= a b} 19 (if {eq? c d} 20 21)))
                 "Unbound Identifier")
(test/exn (test-interp empty `(new-array true 10)) "Not a number: true")
(test/exn (test-interp empty `(/ true 0)) "Not a number: true")
(test/exn (test-interp empty `(if 10 1 0)) "Not a boolean: 10")
(test/exn (test-interp empty `(/ 12 0)) "Division of 12 by zero")
(test/exn (test-interp empty `ab) "Unbound Identifier")
(test/exn (test-interp empty `(ab 12)) "Unbound Identifier")
(test/exn (test-interp empty `(+ / 2)) "Reserved word cannot be used as identifier")
(test/exn (test-interp empty `(if (+ 10 2) 'a 'b)) "Not a boolean: 12")
(test/exn (test-interp empty `(bad-fn 2)) "Unbound Identifier")
(test/exn (test-interp empty `(+ a b)) "Unbound Identifier")
(test/exn (test-interp empty `((fn () 6) 1 2 3)) "Incorrect number of arguments")
(test/exn (test-interp empty `(begin)) "No expressions to execute")

(test (top-eval empty `(eq? (new-array 2 2) (new-array 2 3))) "false")
(test (top-eval empty `(with 
                  (make-incr = (fn (x) (fn () (begin (x <- (+ x 1)) x)))) 
                  (with (incr = (make-incr 23)) 
                        (begin (incr) 
                               (incr) 
                               (incr))))) "26")
(test (top-eval
       empty `{with {m1 = {fn {f n} {if {<= n 0} -1 {+ 1 {f f {- n 1}}}}}}
                    {m1 m1 500}})
      "499")

(test (top-eval empty `(with (a = 0) 
                             (with (a! = (fn (expected) (if (eq? a expected) (a <- (+ 1 a)) (/ 1 0))))
                                   (begin (+ (a! 0) (a! 1))
                                          (if (begin (a! 2) true) 
                                              (a! 3)
                                              (/ 1 0))
                                          (new-array (a! 4) (fn () (a! 5)))))))
      "#<array>")

(test (top-eval empty `(with (a = 1)
                             (b = 23)
                             (array-length = 30)
                             (array-initVal = false)
                             (with (my-array = (new-array array-length array-initVal))
                                   (diff = (fn (x y) (- x y)))
                                   (begin (if (eq? (diff (+ 3 4) (* 2 3)) 1) 
                                              (a <- 19) 
                                              (b <- 19))
                                          (my-array[b] <- (if (<= a b) (+ a b) (- a b)))
                                          (ref my-array[b])))))
      "42")
(test (top-eval 
       empty `{with {a = 0}
                    {with {a! = {fn {expected}
                                    {if {eq? a expected}
                                        {a <- {+ 1 a}}
                                        {/ a 0}}}}
                          {begin {+ {a! 0} {a! 1}}
                                 {if {begin {a! 2} true}
                                     {a! 3}
                                     {/ 1 0}}
                                 {new-array {begin {a! 4} 34}
                                            {begin {a! 5} false}}
                                 {{begin {a! 6} {new-array 3 false}}
                                  [{begin {a! 7} 2}]
                                  <- {begin {a! 8} 98723}}
                                 {with {p = 9}
                                       {p <- {a! 9}}}
                                 {{begin {a! 10} {fn {x y} {begin {a! 13} {+ x y}}}}
                                  {begin {a! 11} 3}
                                  {begin {a! 12} 4}}
                                 14}}})
      "14")

;; New tests with Assignment 5
(test (top-eval (list `{class A extends Object
                         {fields x y}
                         {method mult-x {m} {* m x}}
                         {method get-y {} y}}
                      `{class B extends A
                         {fields z}
                         {method get-xyz {} {* x {* y z}}}})
      `{with {obj = {new B 1 2 3}}
             {obj2 = {new A 4 5}}
             {begin {if {eq? {send obj2 get-y} 5} true fail1}
                    {if {eq? {send obj2 mult-x 2} 8} true fail2}
                    {if {eq? {send obj get-y} 2} true fail3}
                    {if {eq? {send obj get-xyz} 6} true fail4}}})
      "true")
(test (top-eval empty 
                `(rec {foo = {fn {i} {if {<= i 1} 1 {* i {foo {- i 1}}}}}}
                   {foo 4}))
      "24") 