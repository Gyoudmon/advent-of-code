#include "aoc.hpp"

using namespace WarGrey::STEM;
using namespace WarGrey::AoC;

/*************************************************************************************************/
TTF_Font* WarGrey::AoC::aoc_font::title = nullptr;
TTF_Font* WarGrey::AoC::aoc_font::text = nullptr;
TTF_Font* WarGrey::AoC::aoc_font::dimension = nullptr;

/*************************************************************************************************/
void WarGrey::AoC::aoc_fonts_initialize() {
    if (aoc_font::title == nullptr) {
        aoc_font::title = game_create_font(font_basename(game_font::unicode), title_fontsize);
        aoc_font::text = game_create_font(font_basename(game_font::unicode), text_fontsize);
        aoc_font::dimension = game_create_font(font_basename(game_font::math), dim_fontsize);
    }
}

void WarGrey::AoC::aoc_fonts_destroy() {
    game_font_destroy(aoc_font::title);
    game_font_destroy(aoc_font::text);
    game_font_destroy(aoc_font::dimension);
}
