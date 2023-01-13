#pragma once // 确保只被 include 一次

#include "../big_bang/game.hpp"

#include <vector>

namespace WarGrey::AoC {
    /******************************************* 声明游戏精灵 ******************************************/
    class Elfmon : public WarGrey::STEM::Sprite {
        public:
            Elfmon(const char* dirname, int id);

        public:
            int calorie_total();
            
        public:
            std::vector<int> food_calories;
            int id;
    };

    /******************************************* 声明游戏状态 ******************************************/
    enum class CCStatus {
        CountOff,
        FindMaximumCalorie,
        FindMaximumCalories,
        FindMaximumCaloriesViaSorting,
        TaskDone
    };
    
    /******************************************* 声明游戏世界 ******************************************/
    class CalorieCountingWorld : public WarGrey::STEM::World {
        public:
            CalorieCountingWorld() : World("Calorie Counting", 8) {}
            virtual ~CalorieCountingWorld();

        public:  // 覆盖游戏基本方法
            void construct(int argc, char* argv[]) override;
            void load(float width, float height) override;
            void reflow(float width, float height) override;
            void update(uint32_t count, uint32_t interval, uint32_t uptime) override;

        public:
            bool can_select(WarGrey::STEM::IMatter* m) override;
            void after_select(WarGrey::STEM::IMatter* m, bool yes_or_no) override;

        private:
            void on_task_start(WarGrey::AoC::CCStatus status);
            void on_task_done();

        private:
            void move_elf_to_grid(WarGrey::AoC::Elfmon* elf);
            void excite_elf(WarGrey::AoC::Elfmon* elf, float scale);
            void reflow_top_elves();
            void calm_elf_down(WarGrey::AoC::Elfmon* elf, float scale);
            void calm_top_elves_down();
            void random_walk(int start_idx);
            void swap_elves(int self_idx, int target_idx);

        private:
            void load_calories(const std::string& pathname);

        private: // 本游戏世界中的物体
            WarGrey::STEM::Labellet* title;
            WarGrey::STEM::Labellet* population;
            WarGrey::STEM::Labellet* top1_total;
            WarGrey::STEM::Labellet* topn_total;
            WarGrey::STEM::Labellet* sorted_total;
            WarGrey::STEM::Labellet* info_board;
            std::vector<WarGrey::AoC::Elfmon*> elves;
            std::vector<WarGrey::STEM::Labellet*> dims;
            
        private: // 精灵网格参数
            float grid_xoff;
            float grid_yoff;
            float cell_width;
            float cell_height;
            int row;
            int col;

        private:
            WarGrey::AoC::CCStatus status;

        private: // shared variable
            int current_elf_idx;
            int top_calorie;

        private: // for FindMaximumCalorie
            int prev_top_elf_id;

        private: // for FindMaximumCalories and FindMaximumCaloriesViaSorting
            std::vector<int> top_calories;
            std::vector<int> top_elf_indices;
            int top_count;
    };
}
