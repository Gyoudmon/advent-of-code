#lang typed/racket

(define (update-calories [calories : (Listof Natural)] [self:cal : Natural]) : (Listof Natural)
  (define idx (index-where calories (λ [[top:cal : Natural]] (< top:cal self:cal))))
  (cond [(not idx) calories]
        [else (let ([replaced:cal (list-ref calories idx)])
                (update-calories (list-set calories idx self:cal) replaced:cal))]))

(define find-top-calories : (-> Input-Port Positive-Integer Natural)
  (lambda [/dev/datin n]
    (let rpul ([self:cal : Natural 0]
               [calories ((inst make-list Natural) n 0)])
      (define line (read-line /dev/datin))
      (if (string? line)
          (let ([c (string->number line 10)])
            (if (exact-positive-integer? c)
                (rpul (+ self:cal c) calories)
                (rpul 0 (update-calories calories self:cal))))
          (apply + (update-calories calories self:cal))))))

(define main : (-> Integer (Vectorof String) Integer)
  (lambda [argc argv]
    (displayln
     (if (> argc 0)
         
         (let ([fname (vector-ref argv 0)]
               [n (and (> argc 1) (string->number (vector-ref argv 1)))])
           (when (file-exists? fname)
             (call-with-input-file* fname
               (λ [[/dev/stdin : Input-Port]]
                 (find-top-calories /dev/stdin
                                    (cond [(exact-positive-integer? n) n]
                                          [else 3]))))))
         
         (find-top-calories (current-input-port) 3)))
    
    0))



(module+ main
  (exit (main (vector-length (current-command-line-arguments))
              (current-command-line-arguments))))

