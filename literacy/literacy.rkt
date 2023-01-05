#lang racket/base

(provide (all-defined-out))
(provide (all-from-out digimon/tamer))

(require digimon/tamer)
(require digimon/collection)

(require scribble/core)
(require scribble/manual)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Just in case for README.md
(enter-digimon-zone!)

;(tamer-indexed-block-hide-chapter-index #true)

(tamer-default-figure-label "图")
(tamer-default-figure-label-separator #false)
(tamer-default-figure-label-tail ". ")
(tamer-default-figure-label-style 'bold)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define-syntax (define-aoc-bib stx)
  (syntax-case stx []
    [(_ id title day)
     (syntax/loc stx
       (define-bib id
         #:title  (format "Day ~a: ~a" day title)
         #:author "Eric Wastl"
         #:date   2022
         #:url    (format "https://adventofcode.com/2022/day/~a" day)))]))

(define with-aoc-data-from
  (lambda [path #:do f . argv]
    (call-with-input-file* (digimon-path 'stone path)
      (lambda [/dev/stdin]
        (apply f /dev/stdin argv)))))

(define question
  (lambda argv
    (racketresultfont argv)))
