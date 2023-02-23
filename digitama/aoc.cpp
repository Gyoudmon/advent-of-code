#include "aoc.hpp"

using namespace WarGrey::STEM;
using namespace WarGrey::AoC;

/*************************************************************************************************/
TTF_Font* WarGrey::AoC::aoc_font::title = nullptr;
TTF_Font* WarGrey::AoC::aoc_font::large = nullptr;
TTF_Font* WarGrey::AoC::aoc_font::normal = nullptr;
TTF_Font* WarGrey::AoC::aoc_font::tiny = nullptr;
TTF_Font* WarGrey::AoC::aoc_font::math = nullptr;
TTF_Font* WarGrey::AoC::aoc_font::vertical = nullptr;
TTF_Font* WarGrey::AoC::aoc_font::mono = nullptr;

/*************************************************************************************************/
void WarGrey::AoC::aoc_fonts_initialize() {
    if (aoc_font::title == nullptr) {
        aoc_font::title = game_create_font(font_basename(game_font::title), title_fontsize);
        aoc_font::large = game_create_font(font_basename(game_font::unicode), title_fontsize);
        aoc_font::normal = game_create_font(font_basename(game_font::unicode), normal_fontsize);
        aoc_font::tiny = game_create_font(font_basename(game_font::unicode), tiny_fontsize);
        aoc_font::math = game_create_font(font_basename(game_font::math), tiny_fontsize);
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
