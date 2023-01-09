#include "digitama/magical_energy_expedition/rock_paper_scissors.hpp"

using namespace WarGrey::AoC;

int main(int argc, char* args[]) {
    RochamboWorld universe;

    universe.construct(argc, args);
    universe.set_window_size(1200, 1200);
    universe.big_bang();

    return 0;
}
