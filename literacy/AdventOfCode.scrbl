#lang scribble/sigplan @nocopyright

@require{literacy.rkt}
@require{graphviz.rkt}

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-title/pkg-desc[]

@include-abstract{abstract.scrbl}

@handbook-statistics[git-size-ring-chart git-loc-time-series
                     #:lang-delta-only? #true #:recursive? #false #:date-delta (* 3600 24 7)
                     #:gitstat-width 460 #:gitstat-radius 80 #:subgroups null
                     #:altcolors '(["Racket" . Green]
                                   ["Python" . Khaki])]

@handbook-smart-table[]

@include-section{magical.energy.expedition.scrbl}

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-appendix[#:index-section? #true #:numbered? #false
 (book-bib-entry #:date "2011" #:edition "2nd"
                 "CM" "Concrete Mathematics: A Foundation for Computer Science"
                 (list "Ronald L. Graham" "Donald E. Knuth" "Oren Oatashnik")
                 "China Machine Press")
 (book-bib-entry #:date "2011" #:edition "1st"
                 "LoL" "Land of Lisp: Learn to Program in Lisp, One Game at a Time!"
                 "Conrad Barski, MD" "No Starch Press")
 (book-bib-entry #:date "2013" #:edition "1st"
                 "RoR" "Realm of Racket: Learn to Program, One Game at a Time!"
                 (list "Forrest Bice" "Rose DeMaio" "Spencer Florence" "Feng-Yun Mimi Lin" "Scott Lindeman"
                       "Nicole Nussbaum" "Eric Peterson" "Ryan Plessner" "David Van Horn" "Matthias Felleisen"
                       "Conrad Barski, MD")
                 "No Starch Press")
 (book-bib-entry #:date "2009"
                 "TCC" "冒号课堂：编程范式与OOP思想"
                 "郑辉" "电子工业出版社")]
