#include "digitama/2022/calorie_counting.hpp"

using namespace WarGrey::AoC;

int main(int argc, char* args[]) {
    AoCWorld universe;

    universe.construct(argc, args);
    universe.set_window_size(1200, 1200);
    universe.big_bang();

    return 0;
}
