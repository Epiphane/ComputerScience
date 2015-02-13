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
  [binop (op : (Value Value -> Value)) (l : ExprC) (r : ExprC)]
  [ifC (cond : ExprC) (success : ExprC) (fail : ExprC)]
  [appC (fun : ExprC) (args : (listof ExprC))]
  [lam (args : (listof symbol)) (body : ExprC)])

;; Represents an environment
(define-type-alias Environment (hashof symbol Value))

;; the empty environment
(define empty-env (hash (list)))

;; Store
(define-type-alias Sto number)

(define empty-sto 0)

;; Value-store type
(define-type V*S
  [v*s (v : Value) (s : number)])

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
    [(equal? type `/) (some (arith-op /))]
    [(equal? type `eq?) (some (if-op equal?))]
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
(test/exn ((get-arith-op `eq?) (num 5) (bool false)) "Not a number: false")
(test/exn ((get-arith-op `<=) (bool true) (num 6)) "Not a number: true")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Parsing
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Reserved symbols
(define reserved (list `fn `true `false `if `with `+ `- `* `/ `<= `eq? `=))

;; Converts an s-expression into a list of arguments
#;(define (fn-args [exprs : (listof s-expression)]) : (listof symbol)
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
                                    (rest (s-exp->list expr))))])])]))

;; Test cases
#;(test (parse `true) (boolC true))
#;(test (parse `false) (boolC false))
(test (parse `134) (numC 134))
(test (serialize (clos (list) (parse `(+ 4 5)) empty-env)) "#<procedure>")
(test (parse `(f 5 6)) (appC (idC 'f) (list (numC 5) (numC 6))))
#;(test (parse `abc) (idC 'abc))
#;(test (parse `{new-array (+ 2 3) 1}) (new-arrayC (binop + (numC 2) (numC 3))))
#;(test (parse `{ref (new-array 2 0)[1]}) (refC (new-array (numC 2) (numC 0)) (numC 1)))
#;(test (parse `(with {arr = (new-array 3 0)}
                    {arr[1] <- (+ {ref arr[1]} 2)}))
      (appC (lam (list 'arr) (arr-setC arr 1 (binop + (refC (idC 'arr) (numC 1)) (numC 2)))) (new-arrayC (numC 3) (numC 0))))
#;(test (parse `({fn (a b) (a <- b)} 6 4))
      (appC (lam (list 'a 'b) (setC (idC 'a) (idC 'b))) (list (numC 6) (numC 4))))
#;(test (parse `(with {a = 10}
                    (begin (a <- (/ a a))
                           (- a 2)))) 
      (appC (lam (list 'a) (beginC (list (setC (idC 'a) (binop / (idC 'a) (idC 'a)))
                                         (binop - (idC 'a) (numC 2)))))))
#;(test (parse `(if {<= a b} 19 (if {eq? c d} 20 21))) 
      (ifC (binop <=
                  (idC 'a)
                  (idC 'b))
           (numC 19)
           (ifC (binop eq? (idC 'c) (idC 'd)) (numC 20) (numC 21))))
#;(test (parse `(with (a = (+ 4 6)) (b = 12) (* a b)))
      (appC (lam (list 'a 'b)
                 (binop * 'a 'b))
            (binop + (val (num 4)) (val (num 6)))))
#;(test (parse `(fn {} {6})) (val (fun (val (num 6)))))

;; Exception testing
(test/exn (parse `(if 4 5)) "Syntax does not match EBNF spec")
;(test/exn (parse `(fn {a -} {- a -})) "Function does not match EBNF spec")
;(test/exn (parse `(fn {a a} {- a a})) "Argument identifiers are not unique")
;(test/exn (parse `(13)) "Syntax does not match EBNF spec")
;(test/exn (parse `(13 - 10)) "Syntax does not match EBNF spec")
;(test/exn (parse `(with {a = 5})) "Syntax does not match EBNF spec")
;(test/exn (parse `(fn {* a a})) "Syntax does not match EBNF spec")
;(test/exn (parse `(if (<= 5 10) 'a 'b 'c)) "Syntax does not match EBNF spec")
;(test/exn (parse `if) "Bad if form")
;(test/exn (parse `(if 1)) "Bad if form")
;(test/exn (parse `(10 10 10)) "Syntax does not match EBNF spec")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Evaluation
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (eval [e : ExprC] [env : Environment] [sto : Sto]) : V*S
  (type-case ExprC e
    [numC (n) (v*s (num n) sto)]
    [else (error 'unim "Unimplemented")]))
#|
;; Test cases
;; Environment
(define test-env (make-hash (list (values 'a*2b
                                          (interp 
                                           (parse `(fn {a b} {* a (* 2 b)}))))
                                  (values 'min
                                          (interp 
                                           (parse `(fn {a b} {if (<= a b) a b})))))))

(define (test-interp [expr : s-expression] [res : Val])
  (test (v*s-v (interp (parse expr) test-env empty-sto)) res))
(define (test-interp/exn [expr : s-expression] [err : string])
  (test/exn (interp (parse expr) test-env empty-sto) err))

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
(test-interp `{new-array (+ 2 3) 1} (array (list (num 1) (num 1) (num 1) (num 1) (num 1))))
(test-interp `{ref (new-array 2 0)[1]} (num 0))
(test-interp `(with {arr = (new-array 3 0)}
                    {arr[1] <- (+ {ref arr[1]} 2)})
             (num 2))
(test-interp `({fn (a b) (a <- b)} 6 4)
             (num 4))
(test-interp `(with {a = 10}
                    (begin (a <- (/ a a))
                           (- a 2)))
             (num -1))
(test-interp `(with (a = (+ 4 6)) (b = 12) (* a b)) (num 120))
(test-interp `(fn {} {6}) (clos (num 6) empty-env))

;; Exception testing
(test-interp/exn `(l <- 9) "Unbound Identifier")
(test-interp/exn `(ref (new-array 10 3) 15) "Array Index out of bounds")
(test-interp/exn `(with (res = (if (<= (/ 9 3) (+ 1 1)) 10 11))
                        (res)) "Application of number or boolean")
(test/exn (interp (appC (val (num 10)) (list (val (num 12)))) test-env)
          "Application of number or boolean")
(test-interp/exn `(if {<= a b} 19 (if {eq? c d} 20 21))
                 "Unbound Identifier")
(test-interp/exn `(/ 12 0) "Division by zero")
(test-interp/exn `(ab 12) "Unbound Identifier")
(test-interp/exn `(+ / 2) "Reserved word cannot be used as identifier")
(test-interp/exn `(if (+ 10 2) 'a 'b) "Non-boolean test in if statement")
(test-interp/exn `(bad-fn 2) "Unbound Identifier")
(test-interp/exn `(+ a b) "Unbound Identifier")
(test-interp/exn `(a*2b 1 2 3) "Incorrect number of arguments") |#