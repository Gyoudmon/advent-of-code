#pragma once // 确保只被 include 一次

#include "../big_bang/game.hpp"

#include <vector>

namespace WarGrey::AoC {
    /******************************************* 声明游戏世界 ******************************************/
    enum class WorldStatus { CountOff, FindMaximumCalorie, Wait };
    
    /******************************************* 声明游戏世界 ******************************************/
    class Elfmon : public WarGrey::STEM::Sprite {
        public:
            Elfmon(const char* dirname, int idx);

        public:
            int calorie_total();

        public:
            std::vector<int> calories;
            int idx;
    };

    /******************************************* 声明游戏世界 ******************************************/
    class AoCWorld : public WarGrey::STEM::World {
        public:
            AoCWorld() : World("清点卡路里", 8) {}
            virtual ~AoCWorld() { WarGrey::STEM::game_font_destroy(this->big_font); }

        public:    // 覆盖游戏基本方法
            void construct(int argc, char* argv[]) override;
            void load(float width, float height) override;
            void reflow(float width, float height) override;
            void update(uint32_t interval, uint32_t count, uint32_t uptime) override;

        public:
            bool can_select(WarGrey::STEM::IMatter* m) override;
            void after_select(WarGrey::STEM::IMatter* m, bool yes_or_no) override;

        private:
            void load_calories(const std::string& pathname);
            int find_maximum_calories(int n);

        private:
            void move_elf_to_grid(WarGrey::AoC::Elfmon* elf);
            void excite_elf(WarGrey::AoC::Elfmon* elf);
            void calm_elf_down(WarGrey::AoC::Elfmon* elf);

        private:   // 本游戏世界中的物体和数据
            WarGrey::STEM::Labellet* population;
            WarGrey::STEM::Labellet* p1_answer;
            WarGrey::STEM::Labellet* p2_answer;
            WarGrey::STEM::Labellet* leader;
            std::vector<WarGrey::AoC::Elfmon*> elves;

        private:
            float grid_xoff;
            float grid_yoff;
            float cell_width;
            float cell_height;
            int row;
            int col;

        private:
            TTF_Font* big_font;
            WarGrey::AoC::WorldStatus status;
            int current_elf_idx;
            int last_elf_idx;
            int max_cal;
    };
}
