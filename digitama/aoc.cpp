#include "aoc.hpp"

using namespace WarGrey::STEM;
using namespace WarGrey::AoC;

/*************************************************************************************************/
TTF_Font* WarGrey::AoC::aoc_font::title = nullptr;
TTF_Font* WarGrey::AoC::aoc_font::normal = nullptr;
TTF_Font* WarGrey::AoC::aoc_font::tiny = nullptr;
TTF_Font* WarGrey::AoC::aoc_font::vertical = nullptr;
TTF_Font* WarGrey::AoC::aoc_font::mono = nullptr;

/*************************************************************************************************/
void WarGrey::AoC::aoc_fonts_initialize() {
    if (aoc_font::title == nullptr) {
        aoc_font::title = game_create_font(font_basename(game_font::unicode), title_fontsize);
        aoc_font::normal = game_create_font(font_basename(game_font::unicode), normal_fontsize);
        aoc_font::tiny = game_create_font(font_basename(game_font::math), tiny_fontsize);
        aoc_font::vertical = game_create_font(font_basename(game_font::unicode), normal_fontsize);
        aoc_font::mono = game_create_font(font_basename(game_font::monospace), normal_fontsize);
    }
}

void WarGrey::AoC::aoc_fonts_destroy() {
    game_font_destroy(aoc_font::title);
    game_font_destroy(aoc_font::normal);
    game_font_destroy(aoc_font::tiny);
    game_font_destroy(aoc_font::vertical);
    game_font_destroy(aoc_font::mono);
}

/*************************************************************************************************/
std::string WarGrey::AoC::day_to_string(int day) {
    std::string dstr = std::to_string(day);

    if (day < 10) {
        dstr.insert(dstr.begin(), '0');
    }

    return dstr;
}

std::string WarGrey::AoC::string_to_vertical_name(const char* s) {
    std::string vstr;
    int idx = 0;

    while (s[idx] != '\0') {
        unsigned char c = static_cast<unsigned char>(s[idx]);

        if (idx > 0) {
            vstr.push_back('\n');
        }

        /**
         * UTF-8 encodes characters in 1 to 4 bytes, and their binary forms are:
         *   0xxx xxxx
         *   110x xxxx  10xx xxxx
         *   1110 xxxx  10xx xxxx  10xx xxxx
         *   1111 xxxx  10xx xxxx  10xx xxxx  10xx xxxx
         */
        
        if (c >= 0b11110000U) {
            vstr.append(s, idx, 4);
            idx += 4;
        } else if (c >= 0b11100000U) {
            vstr.append(s, idx, 3);
            idx += 3;
        } else if (c >= 0b11000000U) {
            vstr.append(s, idx, 2);
            idx += 2;
        } else {
            vstr.push_back(s[idx]);
            idx ++;
        }
    }

    return vstr;
}
