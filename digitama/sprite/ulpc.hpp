#pragma once

#include "../big_bang/matter/sprite/sheet.hpp"
#include "../big_bang/physics/motion/map2d.hpp"

/* For the Universe LPC Spritesheet Generator */
/* https://sanderfrenken.github.io/Universal-LPC-Spritesheet-Character-Generator/index.html */
/* https://opengameart.org/content/lpc-santa */
/* https://lpc.opengameart.org/static/LPC-Style-Guide/build/index.html */

namespace WarGrey::AoC {
    static const size_t santa_elf_type_count = 5;
    
    class SpriteULPCSheet : public WarGrey::STEM::SpriteGridSheet, protected WarGrey::STEM::I4WayMotion {
    public:
        SpriteULPCSheet(const std::string& pathname);

    protected:
        void on_heading_changed(double theta_rad, double vx, double vy, double prev_vr) override;
        void on_nward(double theta_rad, double vx, double vy) override;
        void on_eward(double theta_rad, double vx, double vy) override;
        void on_sward(double theta_rad, double vx, double vy) override;
        void on_wward(double theta_rad, double vx, double vy) override;

    protected:
        const char* costume_index_to_name(size_t idx) override;
    };

    class ElfSheet : public WarGrey::AoC::SpriteULPCSheet {
    public:
        ElfSheet(const char* name);
        ElfSheet(int name_hint = 256);
    
    public:
        int preferred_local_fps() override { return 10; }
    };
}
