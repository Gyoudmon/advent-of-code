#lang info

(define collection "Advent of Code")
(define pkg-desc "代码降临节")

(define deps '("digimon" "pict"))
(define build-deps '("digimon" "scribble-lib" "racket-doc"))

(define pkg-authors '("WarGrey Gyoudmon Ju"))
(define test-omit-paths 'all)

(define typesettings '(["literacy/AdventOfCode.scrbl" xelatex #:always-make]))

(define native-launcher-names '(["MagicalEnergyExpedition.cpp" console ;desktop
                                 ((config SDL2)
                                  (include [windows "C:\\opt\\vcpkg\\installed\\x64-windows\\include"])
                                  (libpath [windows "C:\\opt\\vcpkg\\installed\\x64-windows\\lib"]))]))
