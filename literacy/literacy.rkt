#lang racket/base

(provide (all-defined-out))
(provide (all-from-out digimon/tamer))

(require digimon/tamer)
(require digimon/collection)
(require digimon/syntax)

(require scribble/core)
(require scribble/manual)

(require (for-syntax racket/symbol))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Just in case for README.md
(enter-digimon-zone!)

;(tamer-indexed-block-hide-chapter-index #true)

(tamer-default-figure-label "图")
(tamer-default-figure-label-separator #false)
(tamer-default-figure-label-tail ". ")
(tamer-default-figure-label-style 'bold)

(tamer-story-submodule-name 'advent)

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

(define-syntax ($argv stx)
  (syntax-case stx []
    [(_ [aname desc ...] [aname:rest desc:rest ...] ...)
     (with-syntax* ([name (datum->syntax #'aname (symbol->immutable-string (syntax-e #'aname)))]
                    [(name:rest ...) (for/list ([<name> (in-list (syntax->list #'(aname:rest ...)))])
                                       (datum->syntax <name> (symbol->immutable-string (syntax-e <name>))))]
                    [maxlength (apply max (map string-length (map syntax-e (syntax->list #'(name name:rest ...)))))])
       (syntax/loc stx
         (list (elem (hspace (- maxlength (string-length name)))
                     (racketvarfont name) ~ desc ...)
               (elem (linebreak) "; " (hspace (- maxlength (string-length name:rest)))
                     (racketvarfont name:rest) ~ desc:rest ...) ...)))]))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define story
  (lambda argv
    (inset-flow (racketplainfont argv))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; output, result
(define variable
  (lambda argv
    (racketvarfont argv)))

(define const
  (lambda argv
    (apply litchar argv)))

(define id
  (lambda argv
    (racketidfont argv)))

(define idea
  (lambda argv
    (racketvalfont argv)))

(define focus
  (lambda argv
    (racketmetafont argv)))

(define note
  (lambda argv
    (racketcommentfont argv)))

(define fallacy
  (lambda argv
    (racketerror argv)))

(define question
  (lambda argv
    (racketparenfont argv)))
