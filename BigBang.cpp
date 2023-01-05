#include "digitama/2022/rock_paper_scissors.hpp"

using namespace WarGrey::AoC;

int main(int argc, char* args[]) {
    RockPaperScissorsWorld universe;

    universe.construct(argc, args);
    universe.set_window_size(1400, 1200);
    universe.big_bang();

    return 0;
}
