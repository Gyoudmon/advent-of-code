#!/usr/bin/env racket

#lang typed/racket

(define cat-by-line : (-> Input-Port Void)
  (lambda [port]
    (for ([line (in-port read-line port)])
      (displayln line))))

(define main : (-> Integer (Vectorof String) Integer)
  (lambda [argc argv]
    (if (> argc 0)

        (for ([fname (in-vector argv)])
          (when (file-exists? fname)
            (call-with-input-file* fname
              cat-by-line)))

        (cat-by-line (current-input-port)))
    
    0))



(module+ main
  (exit (main (vector-length (current-command-line-arguments))
              (current-command-line-arguments))))
