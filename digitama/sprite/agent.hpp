#pragma once

#include "../big_bang/matter/sprite/sheet.hpp"

/* MS Office Assistant, Link (the cat) */
/* https://github.com/jaseg/clippy */

namespace WarGrey::AoC {
    class AgentSpriteSheet : public WarGrey::STEM::SpriteGridSheet {
    public:
        AgentSpriteSheet(const std::string& pathname, int row, int col);

    public:
        int preferred_local_fps() override { return 0; }

    protected:
        int get_initial_costume_index() override { return -1; }
        const char* costume_index_to_name(size_t idx) override { return "[THIS FUNCTION IS USELESS]"; }
    };

    class AgentLink : public WarGrey::AoC::AgentSpriteSheet {
    public:
        AgentLink();

    protected:
        int submit_idle_frames(std::vector<std::pair<int, int>>& frame_refs, int& times) override;
        int submit_action_frames(std::vector<std::pair<int, int>>& frame_refs, const std::string& action) override;
        int update_action_frames(std::vector<std::pair<int, int>>& frame_refs, int next_branch) override;

    private:
        const std::string& find_agent_frames_by_index(int frame_idx, int* agent_idx);
        int push_action_frames(std::vector<std::pair<int, int>>& frame_refs, const std::string& action, int idx0 = 0);
    };
}
