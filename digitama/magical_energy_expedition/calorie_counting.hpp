#pragma once // 确保只被 include 一次

#include "../big_bang/game.hpp"
#include "../sprite/ulpc.hpp"

#include <vector>

namespace WarGrey::AoC {
    /******************************************* 声明游戏精灵 ******************************************/
    class Elfmon : public WarGrey::AoC::ElfSheet {
    public:
        Elfmon(int id);

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
        TaskDone,
        MissionDone,
    };
    
    /******************************************* 声明游戏世界 ******************************************/
    class CalorieCountingPlane : public WarGrey::STEM::Plane {
    public:
        CalorieCountingPlane(int n = 3) : Plane("计点卡路里"), top_count(n) {}
        virtual ~CalorieCountingPlane() {}

    public:  // 覆盖游戏基本方法
        void construct(float width, float height) override;
        void load(float width, float height) override;
        void reflow(float width, float height) override;
        void update(uint32_t count, uint32_t interval, uint32_t uptime) override;

    public:
        bool can_select(WarGrey::STEM::IMatter* m) override;
        void after_select(WarGrey::STEM::IMatter* m, bool yes_or_no) override;

    public:
        bool has_mission_completed() override { return this->status == CCStatus::MissionDone; }
        void on_enter(WarGrey::STEM::IPlane* from) override;

    private:
        void on_task_start(WarGrey::AoC::CCStatus status);
        void on_task_done();

    private:
        void move_elf_to_grid(WarGrey::AoC::Elfmon* elf);
        void excite_elf(WarGrey::AoC::Elfmon* elf, float scale);
        void reflow_top_elves();
        void calm_elf_down(WarGrey::AoC::Elfmon* elf);
        void calm_top_elves_down();
        void random_walk(int start_idx);
        void swap_elves(int self_idx, int target_idx);

    private:
        void load_calories(const std::string& pathname);

    private: // 本游戏世界中的物体
        WarGrey::STEM::Sprite* logo;
        WarGrey::STEM::Labellet* title;
        WarGrey::STEM::Labellet* population;
        WarGrey::STEM::Dimensionlet* top1_total;
        WarGrey::STEM::Dimensionlet* topn_total;
        WarGrey::STEM::Dimensionlet* sorted_total;
        WarGrey::STEM::Labellet* info_board;
        WarGrey::STEM::SpriteGridSheet* snack;
        std::vector<WarGrey::AoC::Elfmon*> elves;
        std::vector<WarGrey::STEM::Labellet*> dims;

    private:
        WarGrey::AoC::CCStatus status;
        WarGrey::STEM::DimensionStyle style;

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
