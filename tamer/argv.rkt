#!/usr/bin/env racket

;;; The 1st line above is called SheBang in Unix
;;; It tells the Unix system to employ `racket` to run this script
;;;   starting from the second line, even if it is a blank line

#lang racket

;;; The 7th line above tells `racket` this script is written in the default 'racket' language

;;;
; The entry function, but the name 'main' is not required, here it is used bacuse of convention in contrast to C/C++
;
; `argc` is short for 'argument count'
; `argv` is short for 'argument vector', and contains all commandline arguments

(define (main argc argv)
  ;;; WARNING
  ; Unlike in C++ or Python, the name of the executable is not contained in 'argv'
  ; But the `find-system-path` can be used to get it.
  (printf "Running: ~a~n" (find-system-path 'run-file))
  (printf "  received ~a argument~a from user~n" argc (if (= argc 1) "" "s"))
  
  ; display all commandline arguments, one per line
  (for ([i (in-range 0 argc)])
    (printf "    argv[~a]: ~a~n" i (vector-ref argv i)))
  
  0)



(module+ main
  ;;; the submodule with the required name 'main' can be used to launch the application
  ;;; just similar to what C and C++ applications would do with the 'main' function

  (exit (main (vector-length (current-command-line-arguments))
              (current-command-line-arguments))))

