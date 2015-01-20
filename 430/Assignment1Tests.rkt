;; 2.3.3 Total profit for a movie theater, given the number of attendees
;; $5 per ticket, Cost: $20 + $0.50 per attendee
;; No implementation...

;; Test cases
(test (total-profit 10) 25)
(test (total-profit 20) 70)

;; ----------------------------------------------------------------------------
;; 3.3.5 Computes the height a rocket reaches in a given amount of time.
;; No implementation...

;; Test cases
(test (height 10) 490)
(test (height 1) 4.9)

;; ----------------------------------------------------------------------------
;; 2.2 Writing implements
;; Represent a writing implement (provided in assignment)
(define-type Writer
  ; ink volume in ml, how-full a number in the range 0.0 to 1.0
  [pen (ink-volume : number) (how-full : number)]
  ; length in cm
  [pencil (length : number)])

;; Given a writing instrument, output how far it can write
;; No implementation...

;; how-far-to-write test cases
(test (how-far-to-write (pen 10 0.3)) 450)
(test (how-far-to-write (pencil 2)) 112)

;; ----------------------------------------------------------------------------
;; 2.3 Low-degree Polynomials
;; Representation for a polynomial
(define-type Polynomial
  [linear (a : number) (b : number)]
  [quadratic (a : number) (b : number) (c : number)])

;; Evaluates a polynomial
;; No implementation...

;; Test cases
(test (eval (linear 2 4) 3) 10)
(test (eval (quadratic 4 1 9) 3) 48)
(test (eval (linear 3 2) (eval (quadratic 1 2 3) 2)) 35)

;; ----------------------------------------------------------------------------
;; 2.4 Derivative
;; Accepts a polynomial and returns a polynomial representing its derivative
;; No implementation...

;; Test cases
(test (derivative (linear 4 3)) (linear 0 4))
(test (derivative (quadratic 3 2 1)) (linear 6 2))
(test (derivative (quadratic 0 5 3)) (linear 0 5))
(test (derivative (derivative (quadratic 5 4 3))) (linear 0 10))

;; ----------------------------------------------------------------------------
;; 2.5 Binary Tree
;; Represents binary tree objects
(define-type BTree
  [leaf (value : symbol)]
  [node (left : BTree) (right : BTree)])

(define tree1 (leaf 'abc))
(define tree2 (node (leaf 'abc) 
                    (node (leaf 'def)
                          (leaf 'ghi))))
(define tree3 (node tree2
                    (node tree2
                          tree1))

;; ----------------------------------------------------------------------------
;; 2.6 Mirror
;; Given a binary tree, produce a new BTree that is a mirror image (left-right)
;; No implementation...

;; Test cases
(test (mirror tree1) (leaf 'abc))
(test (mirror tree2) (node (node (leaf 'ghi)
                                 (leaf 'def))
                           (leaf 'abc)))
(test (mirror tree3) (node (node (leaf 'abc)
                                 (node (node (leaf 'ghi)
                                             (leaf 'def))
                                       (leaf 'abc))
                           (node (node (leaf 'ghi)
                                       (leaf 'def))
                                 (leaf 'abc))))
(test (mirror (mirror tree3)) tree3)

;; ----------------------------------------------------------------------------
;; 2.7 Traversal
;; Given a binary tree, produce a list representing an in-order traversal of it
;; No implementation...

;; Test cases
(test (in-order tree1) (list 'abc))
(test (in-order tree2) (list 'ghi 'def 'abc))
(test (in-order tree3) (list 'abc 'ghi 'def 'abc 'ghi 'def 'abc))
(test (in-order (mirror tree2)) (list 'abc 'def 'ghi))
(test (in-order (mirror tree3)) (list 'abc 'def 'ghi 'abc 'def 'ghi 'abc))