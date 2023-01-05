#pragma once

#include "big_bang/graphics/font.hpp"

namespace WarGrey::AoC {
    /* Constants */
    static const char* title_fmt = "C++ 降临节: [任务%d] %s";
    static const char* puzzle_fmt = "%s: %d";
    static const char* unknown_fmt = "%s: (未知)";

    /* Fonts */
    static const int title_fontsize = 50;
    static const int text_fontsize = 36;
    static const int dim_fontsize = 24;

    class aoc_font {
        public:
            static TTF_Font* title;
            static TTF_Font* text;
            static TTF_Font* dimension;
    };

    void aoc_fonts_initialize();
    void aoc_fonts_destroy();
}
