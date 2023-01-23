#pragma once

#include "big_bang/graphics/font.hpp"

namespace WarGrey::AoC {
    /* Constants */
    static const char* title0_fmt = "C++ 降临节: %s";
    static const char* title_fmt = "C++ 降临节: [第%d天] %s";
    static const char* puzzle_fmt = "%s: %d";
    static const char* unknown_fmt = "%s: (未知)";

    /* Fonts */
    static const int title_fontsize = 50;
    static const int answer_fontsize = 36;
    static const int text_fontsize = 32;
    static const int dim_fontsize = 24;
    static const int vertical_fontsize = 18;

    class aoc_font {
    public:
        static TTF_Font* title;
        static TTF_Font* text;
        static TTF_Font* dimension;
        static TTF_Font* vertical;
        static TTF_Font* mono;
    };

    void aoc_fonts_initialize();
    void aoc_fonts_destroy();

    std::string day_to_string(int day);
    std::string string_to_vertical_name(const char* s);
}
