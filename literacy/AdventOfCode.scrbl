#lang scribble/sigplan @nocopyright

@require{literacy.rkt}
@require{graphviz.rkt}

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-title/pkg-desc[]

@include-abstract{abstract.scrbl}

@handbook-statistics[git-size-ring-chart git-loc-time-series
                     #:lang-delta-only? #true #:recursive? #false #:date-delta (* 3600 24)
                     #:gitstat-width 460 #:gitstat-radius 80 #:subgroups null
                     #:altcolors '(["Racket" . Green]
                                   ["Python" . Khaki])]

@handbook-smart-table[]

@include-section{magical.energy.expedition.scrbl}

@texbook-appendix{先修知识}
@include-section{appendix.scrbl}

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-appendix[#:index-section? #true #:numbered? #false]
