#include "mario.hpp"

using namespace WarGrey::STEM;
using namespace WarGrey::AoC;

/*************************************************************************************************/
WarGrey::AoC::MarioWorldAtlas::MarioWorldAtlas(int row, int col, float tile_size) : GridAtlas("spritesheet/mario.png", 58, 60) {
        this->map_row = row;
        this->map_col = col;
        this->map_tile_width = tile_size;
        this->map_tile_height = tile_size;
}
