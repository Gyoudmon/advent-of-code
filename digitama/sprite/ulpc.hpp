#pragma once

#include "../big_bang/matter/sprite/sheet.hpp"

/* For the Universe LPC Spritesheet Generator */
/* https://sanderfrenken.github.io/Universal-LPC-Spritesheet-Character-Generator/index.html */
/* https://opengameart.org/content/lpc-santa */
/* https://lpc.opengameart.org/static/LPC-Style-Guide/build/index.html */

namespace WarGrey::AoC {
    static const size_t santa_elf_type_count = 5;
    
    class SpriteULPCSheet : public WarGrey::STEM::SpriteGridSheet {
    public:
        SpriteULPCSheet(const std::string& pathname);

    protected:
        const char* costume_grid_to_name(int r, int c) override;
    };

    class ElfSheet : public WarGrey::AoC::SpriteULPCSheet {
    public:
        ElfSheet(const char* name);
        ElfSheet(int name_hint = 256);
    };
}
