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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Serialization
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Test cases
(test (serialize (num 32)) "32")
(test (serialize (id 'abc)) "abc")
(test (serialize (bool true)) "true")
(test (serialize (bool false)) "false")
(test (serialize (clos (list 'a) (binop + (val (num 5)) (val (num 2))) empty-env)) "#<procedure>")
(test (serialize (array (list 1 2 3))) "#<array>")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Parsing
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Test cases
(test (parse `true) (boolC true))
(test (parse `false) (boolC false))
(test (parse `134) (numC 134))
(test (parse `abc) (idC 'abc))
(test (parse `{new-array (+ 2 3) 1}) (new-arrayC (binop + (numC 2) (numC 3))))
(test (parse `{ref (new-array 2 0)[1]}) (refC (new-array (numC 2) (numC 0)) (numC 1)))
(test (parse `(with {arr = (new-array 3 0)}
                    {arr[1] <- (+ {ref arr[1]} 2)}))
      (appC (lam (list 'arr) (arr-setC arr 1 (binop + (refC (idC 'arr) (numC 1)) (numC 2)))) (new-arrayC (numC 3) (numC 0))))
(test (parse `({fn (a b) (a <- b)} 6 4))
      (appC (lam (list 'a 'b) (setC (idC 'a) (idC 'b))) (list (numC 6) (numC 4))))
(test (parse `(with {a = 10}
                    (begin (a <- (/ a a))
                           (- a 2)))) 
      (appC (lam (list 'a) (beginC (list (setC (idC 'a) (binop / (idC 'a) (idC 'a)))
                                         (binop - (idC 'a) (numC 2)))))))
(test (parse `(if {<= a b} 19 (if {eq? c d} 20 21))) 
      (ifC (binop <=
                  (idC 'a)
                  (idC 'b))
           (numC 19)
           (ifC (binop eq? (idC 'c) (idC 'd)) (numC 20) (numC 21))))
(test (parse `(with (a = (+ 4 6)) (b = 12) (* a b)))
      (appC (lam (list 'a 'b)
                 (binop * 'a 'b))
            (binop + (val (num 4)) (val (num 6)))))
(test (parse `(fn {} {6})) (val (fun (val (num 6)))))

;; Exception testing
(test/exn (parse `(fn {a -} {- a -})) "Function does not match EBNF spec")
(test/exn (parse `(fn {a a} {- a a})) "Argument identifiers are not unique")
(test/exn (parse `(13)) "Syntax does not match EBNF spec")
(test/exn (parse `(13 - 10)) "Syntax does not match EBNF spec")
(test/exn (parse `(with {a = 5})) "Syntax does not match EBNF spec")
(test/exn (parse `(fn {* a a})) "Syntax does not match EBNF spec")
(test/exn (parse `(if (<= 5 10) 'a 'b 'c)) "Syntax does not match EBNF spec")
(test/exn (parse `if) "Bad if form")
(test/exn (parse `(if 1)) "Bad if form")
(test/exn (parse `(10 10 10)) "Syntax does not match EBNF spec")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Evaluation
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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
(test-interp/exn `(a*2b 1 2 3) "Incorrect number of arguments")