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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Serialization
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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
(test (test-interp `{}) (clos (list) (numC 6) empty-env))

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
(test/exn (test-interp `(/ 12 0)) "Division of 12 by zero")
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

(test (top-eval `(with (a = 0) 
                 (with (a! = (fn (expected) (if (eq? a expected) (a <- (+ 1 a)) (/ 1 0))))
                       (begin (+ (a! 0) (a! 1))
                              (if (begin (a! 2) true) 
                                  (a! 3)
                                  (/ 1 0))
                              (new-array (a! 4) (fn () (a! 5)))))))
      "#<array>")

(test (top-eval `(with (a = 1)
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
       '{with {a = 0}
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