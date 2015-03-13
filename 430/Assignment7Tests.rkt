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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Serialization
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Test cases
(test (serialize (num 32)) "32")
(test (serialize (bool true)) "true")
(test (serialize (bool false)) "false")
(test (serialize (clos (list 'a) (binop + (val (num 5)) (val (num 2))) empty-env)) "#<procedure>")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Expansion
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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

;; Basic EBNF Tests
(test-interp `(if (eq? (- 10 5) 5) 10 11) (num 10))
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