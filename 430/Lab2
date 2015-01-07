#lang plai-typed

;; Lab 2 Excersizes
;; 1 Define a desk with width, height, depth
;; 3 Include bookshelves with depth, # shelves, shelf width
(define-type Furniture
  [desk (width : number) (height : number) (depth : number)]
  [bookshelf (depth : number) (shelves : number) (shelf-width : number)])

(define my-desk (desk 10 7 5))
(define my-bookshelf (bookshelf 10 12 3))

;; 2/4 Determine how much floor space a desk takes up
(define (furniture-footprint [furniture : Furniture]) : number
  (type-case Furniture furniture
    [desk (width height depth) (* width depth)]
    [bookshelf (depth shelves shelf-width) (* shelf-width depth)]))

;; Test cases
;; My desk (10 x 7 x 5) = 50 ft of floor space
(test (furniture-footprint my-desk) 50)

;; My Bookshelf (10 x 3, 12 shelves) = 30 ft
(test (furniture-footprint my-bookshelf) 30)
