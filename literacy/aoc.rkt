#lang racket/base

(provide (all-defined-out))
(provide (all-from-out racket/base))

(require digimon/digitama/system)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define with-aoc-data-from
  (lambda [path #:do f . argv]
    (call-with-input-file* (digimon-path 'stone path)
      (lambda [/dev/stdin]
        (apply f /dev/stdin argv)))))

(define read-aoc-data
  (lambda [#:from path #:with [read-datum read-single-line] #:for-each-do f . argv]
    (call-with-input-file* (digimon-path 'stone path)
      (lambda [/dev/stdin]
        (for/list ([datum (in-port read-datum /dev/stdin)])
          (apply f datum argv))))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define read-single-line
  (lambda [/dev/stdin]
    (read-line /dev/stdin 'any)))
