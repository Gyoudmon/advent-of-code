#pragma once

#include "../big_bang/matter/atlas.hpp"

namespace WarGrey::AoC {
    class MarioWorldAtlas : public WarGrey::STEM::GridAtlas {
    public:
        MarioWorldAtlas(int row, int col, float tile_size);
    };
}
