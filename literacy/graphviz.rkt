#lang racket

(provide (all-defined-out))

(require (except-in scribble/core table))
(require scribble/manual)

(require digimon/format)
(require digimon/git)
(require digimon/digitama/git/langstat)

(require racket/date)
(require racket/draw)
(require pict)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define file-color (make-style 'tt (list (make-color-property (list #x58 #x60 #x69)))))
(define insertion-color (make-style 'tt (list (make-color-property (list #x28 #xA7 #x45)))))
(define deletion-color (make-style 'tt (list (make-color-property (list #xCB #x24 #x31)))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define pie-chart
  (let ([no-pen (make-pen #:style 'transparent)]
        [legend-pen (make-pen #:color (make-color #xbb #xbb #xbb))]
        [legend-brush (make-brush #:color (make-color #xFF #xFF #xFF 0.618))])
    (lambda [#:radian0 [r0 0.0] #:bytes-fx [fx 0.5] #:bytes-fy [fy 0.5]
             #:legend-font [legend-font (make-font #:family 'modern #:weight 'bold #:size 9)]
             #:label-color [label-color "black"] #:%-color [%-color (make-color #x6a #x72 #x8f)] #:total-color [total-color (make-color 0 0 0 0.2)]
             flradius datasource altcolors]
      (define flwidth (* flradius 2.0))
      (define flheight (* flradius 2.0))
      
      (dc (λ [dc dx dy]
            (define saved-font (send dc get-font))
            (define saved-color (send dc get-text-foreground))
            (define saved-pen (send dc get-pen))
            (define saved-brush (send dc get-brush))

            (send dc set-smoothing 'aligned)
            (send dc set-font legend-font)
            (send dc set-pen no-pen)
      
            (when (pair? datasource)
              (define 1ch (send dc get-char-width))
              (define 1em (send dc get-char-height))
              (define lineheight (* 1em 1.2))
              (define ring-width (* 1em pi))
              (define legend-diameter (* 1em 0.618))
              (define legend-center-off (* (- 1em legend-diameter) 0.5))
              (define ring-diameter (- (min flwidth flheight) 1.0))
              (define hollow-diameter (- ring-diameter ring-width))
              (define hollow-off (/ (- ring-diameter hollow-diameter) 2))
              (define-values (ring-x ring-y) (values (+ dx (- flwidth ring-diameter)) (+ dy (* (- flheight ring-diameter) 0.5))))
              (define-values (hollow-x hollow-y) (values (+ ring-x hollow-off) (+ ring-y hollow-off)))
              (define total (for/sum ([ds (in-list datasource)]) (git-language-content ds)))

              (when (positive? total)
                (define ring (make-object region% #false))
                (define hollow (make-object region% #false))
                (define nonfirst% (- 1.0 (/ (git-language-content (car datasource)) total)))
                (send ring set-rectangle dx dy flwidth flheight)
                (send hollow set-ellipse hollow-x hollow-y hollow-diameter hollow-diameter)
                (send ring subtract hollow)
                (send dc set-clipping-region ring)
                
                (define-values (legend-label-width legend-%width legends)
                  (let draw-ring ([rest datasource]
                                  [radian0 (if (flonum? r0) r0 (* nonfirst% pi))]
                                  [max-lwidth 0]
                                  [max-pwidth 0]
                                  [legends null])
                    (cond [(null? rest) (values max-lwidth max-pwidth (reverse legends))]
                          [else (let ([datum (car rest)])
                                  (define datum% (/ (git-language-content datum) total))
                                  (define brush (make-brush #:color (language-rgba datum altcolors)))
                                  (define radiann (+ radian0 (* datum% pi 2)))
                                  (send dc set-brush brush)
                                  (send dc draw-arc ring-x ring-y ring-diameter ring-diameter radian0 radiann)
                                  
                                  (define label (git-language-name datum))
                                  (define percentage (~% datum% #:precision '(= 2)))
                                  
                                  (define-values (lwidth _lh ldistance _ls) (send dc get-text-extent label legend-font #true))
                                  (define-values (pwidth _ph _pd _ps) (send dc get-text-extent percentage legend-font #true))
                                  (draw-ring (cdr rest) radiann (max lwidth max-lwidth) (max pwidth max-pwidth)
                                             (cons (vector label percentage pwidth brush ldistance) legends)))])))
                
                (send dc set-clipping-region #false)
                (define legend-box-x (+ dx legend-center-off))
                (define legend-box-y (+ dy legend-center-off))
                (define legend-box-width (+ 1em legend-label-width 1ch legend-%width legend-center-off legend-center-off))
                (define legend-box-height (+ legend-center-off (* lineheight (length datasource))))
                (define-values (legend-x0 legend-y0) (values (+ legend-box-x legend-center-off) (+ legend-box-y legend-center-off)))
                (send dc set-pen legend-pen)
                (send dc set-brush legend-brush)
                (send dc draw-rectangle legend-box-y legend-box-y legend-box-width legend-box-height)
                (send dc set-pen no-pen)
                
                (let ([bytes (~size total)])
                  (define-values (total-width total-height _d _s) (send dc get-text-extent bytes legend-font #true))
                  (define tx (+ ring-x (* (- ring-diameter total-width)  fx)))
                  (define ty (+ ring-y (* (- ring-diameter total-height) fy)))
                  (send dc set-text-foreground total-color)
                  (send dc draw-text bytes tx ty #true))
                
                (for ([lgd (in-list legends)]
                      [idx (in-naturals)])
                  (define label (vector-ref lgd 0))
                  (define p% (vector-ref lgd 1))
                  (define legend-%x (+ legend-x0 1em legend-label-width (- 1ch legend-center-off) (- legend-%width (vector-ref lgd 2))))
                  (define legend-y (+ legend-y0 (* lineheight idx)))
                  (send dc set-brush (vector-ref lgd 3))
                  (send dc draw-ellipse (+ legend-x0 legend-center-off) (+ legend-y legend-center-off) legend-diameter legend-diameter)
                  (send dc set-text-foreground label-color)
                  (send dc draw-text label (+ legend-x0 1em) (+ legend-y (* (vector-ref lgd 4) 0.5)) #true)
                  (send dc set-text-foreground %-color)
                  (send dc draw-text p% legend-%x (+ legend-y (* (vector-ref lgd 4) 0.5)) #true))))
            
            (send* dc
              (set-font saved-font)
              (set-text-foreground saved-color)
              (set-pen saved-pen)
              (set-brush saved-brush)))
          flwidth flheight))))

(define git-loc-series
  (lambda [#:date0 [date-start #false] #:daten [date-end #false]
           #:line0 [line-start #false] #:linen [line-end #false]
           #:line-axis-count [axis-count #false] #:line-peak-ahead-factor [peak-factor 1000]
           #:mark-font [mark-font (make-font #:family 'system #:size 10)]
           #:mark-color [mark-color (make-color #x6A #x73 #x7D)]
           #:axis-color [axis-color (make-color #x00 #x00 #x00 0.3)]
           flwidth flheight datasource altcolors date-delta]
    (dc (λ [dc dx dy]
          (define saved-font (send dc get-font))
          (define saved-color (send dc get-text-foreground))
          (define saved-pen (send dc get-pen))
          (define saved-brush (send dc get-brush))

          (send dc set-smoothing 'aligned)
          (send dc set-font mark-font)
        
          (define 1ch (send dc get-char-width))
          (define 1em (send dc get-char-height))
          (define 1ex (* 1em 1/2))
          
          (define-values (locsource peak x0 xn)
            (for/fold ([src null] [all-peak 0] [x0 +inf.0] [xn 0])
                      ([lang-src (in-list datasource)])
              (define lang (git-language-name lang-src))
              (define pen (make-pen #:color (language-rgba lang-src altcolors)))
              (define stats (git-language-content lang-src))
              (define-values (date0 daten)
                (cond [(null? stats) (values x0 xn)]
                      [else (values (caar stats) (car (last stats)))]))
              (define-values (LoCs total peak)
                (for/fold ([LoCs null] [total 0] [peak 0])
                          ([stat (in-list stats)])
                  (define-values (adds dels) (git-numstats->additions+deletions* (list stat)))
                  (define total++ (+ total (- adds dels)))
                  (values (cons (cons (car stat) total++) LoCs)
                          total++
                          (max peak total++))))
              (values (cons (vector lang pen (reverse LoCs) total) src)
                      (max all-peak peak)
                      (min x0 date0)
                      (max xn daten))))
          
          (define-values (sec0 secn line0 linen)
            (values (or date-start x0)
                    (or date-end xn)
                    (or line-start 0)
                    (or line-end (* (max (exact-ceiling (/ peak peak-factor)) 1) peak-factor))))
          
          (define-values (mark-max-width _h _d _s) (send dc get-text-extent (~integer linen) mark-font #true))
          (define x-start (+ dx mark-max-width 1ch))
          (define x-range (- (+ dx flwidth) x-start mark-max-width))
          (define y-start (- (+ dy flheight) 1ex 1em))
          (define y-range (- y-start dy 1ex))
          (define date-interval (- secn sec0))
          (define line-interval (- linen line0))
          (define date-fraction (/ x-range date-interval))
          (define line-fraction (/ y-range line-interval))

          (send dc set-pen axis-color 1 'solid)
          (send dc set-text-foreground mark-color)
          
          (let draw-x-axis ([this-endx 0.0]
                            [this-sec sec0]
                            [last-month 0])
            (if (< this-sec secn)
                (let ([the-date (seconds->date this-sec)])
                  (define-values (year month) (values (date-year the-date) (date-month the-date)))
                  (define-values (x-axis x-mark year?)
                    (cond [(and (= month 1) (not (= month last-month))) (values this-sec (number->string year) #true)]
                          [(= this-sec sec0) (values this-sec (~day (date-day the-date)) #false)]
                          [(= last-month month) (values this-sec (~day (date-day the-date)) #false)]
                          [else (values this-sec (~month month) #false)]))

                  (when (and year?)
                    (let ([self-x (+ x-start (* (- x-axis sec0) date-fraction))])
                      (send dc set-pen axis-color 1 'short-dash)
                      (send dc draw-line self-x y-start self-x (- y-start y-range))
                      (send dc set-pen axis-color 1 'solid)))
                  
                  (draw-x-axis (draw-x dc (- x-axis sec0) x-mark this-endx
                                       mark-font x-start y-start
                                       date-fraction 1ex)
                               (+ this-sec date-delta)
                               month))
                (draw-x dc (- secn sec0)
                        (~day (date-day (seconds->date secn)))
                        this-endx mark-font x-start y-start
                        date-fraction 1ex)))
          
          (define adjust-count
            (cond [(and (integer? axis-count) (> axis-count 1)) axis-count]
                  [else (exact-ceiling (/ y-range (* 1em 2.0)))]))

          (if (<= adjust-count 1)
              (send* dc
                (draw-text "0" (- x-start 1ch 1ch) (- y-start 1ex) #true)
                (draw-line x-start y-start (+ x-start x-range) y-start))
              (for ([y-axis (in-range line0 (+ linen 1) (/ line-interval (- adjust-count 1)))])
                (define y (- y-start (* (- y-axis line0) line-fraction)))
                (define y-mark (if (zero? y-axis) "0" (if (exact-integer? y-axis) (~integer y-axis) (number->string y-axis))))
                (define-values (y-width _w _d _s) (send dc get-text-extent y-mark mark-font #true))
                (send dc draw-text y-mark (- x-start 1ch y-width) (- y 1ex) #true)
                (send dc draw-line x-start y (+ x-start x-range) y)))
          
          (for ([loc-src (in-list locsource)])
            (send dc set-pen (vector-ref loc-src 1))
            (send dc set-text-foreground (send (vector-ref loc-src 1) get-color))
            (define this-y-axis (vector-ref loc-src 3))
            (define y (- y-start (* (- this-y-axis line0) line-fraction)))
            (send dc draw-text (number->string this-y-axis) (+ x-start x-range 1ch) (- y 1ex) #true)
            (send dc draw-lines
                  (for/list ([date.LoC (in-list (vector-ref loc-src 2))])
                    (define-values (x-axis y-axis) (values (car date.LoC) (cdr date.LoC)))
                    (cons (+ x-start (* (- x-axis sec0) date-fraction))
                          (- y-start (* (- y-axis line0) line-fraction))))))
        
          (send* dc
            (set-font saved-font)
            (set-text-foreground saved-color)
            (set-pen saved-pen)
            (set-brush saved-brush)))
        flwidth flheight)))

(define handbook-statistics
  (lambda [#:gitstat-width [git-width #false] #:gitstat-radius [git-radius #false] #:recursive? [recursive? #true]
           #:ignore [exclude-submodules null] #:filter [filter null] #:subgroups [subgroups git-default-subgroups]
           #:altcolors [altcolors null] #:since [since #false] #:date-delta [date-delta (* 3600 24 31)]]
    (define all-files (git-list-tree #:recursive? recursive? #:ignore-submodule exclude-submodules #:filter filter))
    (define all-numstats (git-numstat #:recursive? recursive? #:ignore-submodule exclude-submodules #:since since #:filter filter))
    (define lang-files (git-files->langfiles all-files null subgroups))
    (define lang-sizes (git-files->langsizes all-files null subgroups))
    (define lang-stats (git-numstats->langstats all-numstats null subgroups))
    
    (define src-file (for/fold ([count 0]) ([lf (in-hash-values lang-files)]) (+ count (length (git-language-content lf)))))
    (define-values (insertions deletions) (git-numstats->additions+deletions* all-numstats))
    
    (define sorted-langfiles (sort (hash-values lang-sizes) >= #:key git-language-content))
    (define langstats (for/list ([(id lang) (in-hash lang-stats)] #:when (hash-has-key? lang-sizes id)) lang))
    
    (nested (filebox (elem #:style file-color (~integer src-file) (subscript "files")
                           ~ (elem #:style insertion-color (~integer insertions) (subscript "++"))
                           ~ (elem #:style deletion-color (~integer deletions) (subscript (literal "--"))))
                     (tabular #:sep (hspace 1) #:column-properties '(left right)
                              (list (let* ([pie-radius (or git-radius 75)]
                                           [series-height (* (or git-radius pie-radius) 2)]
                                           [series-width (or git-width 380)])
                                      (list (pie-chart #:bytes-fy 0.618 #:radian0 (* pi 0.5) pie-radius sorted-langfiles altcolors)
                                            (git-loc-series series-width series-height langstats altcolors date-delta)))))))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define (draw-x dc x-axis x-mark last-mark-endx mark-font x-start y-start date-fraction 1ex)
  (define x (+ x-start (* x-axis date-fraction)))
  (define-values (x-width x-height _d _s) (send dc get-text-extent x-mark mark-font #true))
  (let ([mark-x (- x (/ x-width 2))])
    (cond [(> mark-x last-mark-endx)
           (send dc draw-line x y-start x (+ y-start 1ex))
           (send dc draw-text x-mark mark-x (+ y-start 1ex) #true)
           (+ mark-x x-width)]
          [else mark-x])))

(define language-rgba
  (lambda [lang altcolors]
    (define maybe-color (or (assoc (git-language-name lang) altcolors) (git-language-color lang)))
    
    (cond [(not maybe-color) (make-color 0 0 0 1.0)]
          [(pair? maybe-color) (cdr maybe-color)]
          [(keyword? maybe-color)
           (let ([rgb (string->number (keyword->string maybe-color) 16)])
             (make-color (bitwise-and (arithmetic-shift rgb -16) #xFF)
                         (bitwise-and (arithmetic-shift rgb -8) #xFF)
                         (bitwise-and rgb #xFF)
                         1.0))]
          [else (make-color 0 0 0 1.0)])))

(define ~month
  (lambda [m]
    (cond [(< m 10) (string-append "0" (number->string m))]
          [else (number->string m)])))

(define ~day
  (lambda [d]
    (format "~~~a" d)))
