#pragma once

#include "big_bang/graphics/font.hpp"
#include "big_bang/physics/random.hpp"

#include "big_bang/datum/path.hpp"
#include "big_bang/datum/flonum.hpp"

#include "sprite/ulpc.hpp"

#include <iostream>
#include <fstream>

namespace WarGrey::AoC {
    static const char* aoc_ext = ".aoc";
    
    /* Constants */
    static const char* title0_fmt = "C++ 降临节: %s";
    static const char* title_fmt = "C++ 降临节: [第%d天] %s";
    static const char* puzzle_fmt = "%s: %d";
    static const char* unknown_fmt = "%s: (未知)";
    
    /* Fonts */
    static const int title_fontsize = 42; /* <-- already hit the ceiling */
    static const int answer_fontsize = 28;
    static const int normal_fontsize = 24;
    static const int tiny_fontsize = 20;

    class aoc_font {
    public:
        static TTF_Font* title;
        static TTF_Font* normal;
        static TTF_Font* tiny;
        static TTF_Font* vertical;
        static TTF_Font* mono;
    };

    void aoc_fonts_initialize();
    void aoc_fonts_destroy();

    std::string day_to_string(int day);
    std::string string_to_vertical_name(const char* s);
}
