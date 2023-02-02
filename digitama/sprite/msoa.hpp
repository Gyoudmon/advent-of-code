#pragma once

#include "../big_bang/matter/sprite/sheet.hpp"

/* MS Office Assistant, Link (the cat) */
/* https://github.com/jaseg/clippy */

namespace WarGrey::AoC {
    class MSOfficeSpriteSheet : public WarGrey::STEM::SpriteGridSheet {
    public:
        MSOfficeSpriteSheet(const std::string& pathname, int row, int col);

    protected:
        int get_initial_costume_index() override { return -1; }
        const char* costume_index_to_name(size_t idx) override { return "[THIS FUNCTION IS USELESS]"; }
    };

    class OALinkmon : public WarGrey::AoC::MSOfficeSpriteSheet {
    public:
        OALinkmon();
        
    public:
        int preferred_local_fps() override { return 0; }

    protected:
        int submit_action_frames(std::vector<std::pair<int, int>>& frame_refs, const std::string& action) override;
        int update_action_frames(std::vector<std::pair<int, int>>& frame_refs, const std::string& action) override;
    };
}
