#pragma once // 确保只被 include 一次

#include "../big_bang/game.hpp"
#include "../sprite/mario.hpp"
#include "../aoc.hpp"

#include <vector>
#include <map>

namespace WarGrey::PLT {
    /******************************************* 声明游戏状态 ******************************************/
    enum class PStatus {
        FindMisplacedItems,
        FindBadgeItems,
        CreatePackHash,
        TaskDone,
        MissionDone,
    };

    class PortBackDrop : public WarGrey::AoC::MarioWorldAtlas {
    public:
        PortBackDrop(int row, int col, float tile_size = 0.0F)
            : MarioWorldAtlas(row, col, tile_size) {}
    };
    
    /******************************************* 声明游戏世界 ******************************************/
    class PortPlane : public WarGrey::STEM::Plane {
    public:
        PortPlane() : Plane("#流") {}
        virtual ~PortPlane() {}

    public:  // 覆盖游戏基本方法
        void construct(float width, float height) override;
        void load(float width, float height) override;
        void reflow(float width, float height) override;
        void update(uint32_t count, uint32_t interval, uint32_t uptime) override;

    public:
        bool can_select(WarGrey::STEM::IMatter* m) override;
        void after_select(WarGrey::STEM::IMatter* m, bool yes_or_no) override;

    public:
        bool has_mission_completed() override;
        void on_enter(WarGrey::STEM::IPlane* from) override;

    private:
        void on_task_start(WarGrey::PLT::PStatus status);
        void on_task_done();

    private: // 本游戏世界中的物体
        WarGrey::STEM::Linkmon* agent;
        WarGrey::STEM::Labellet* title;
        WarGrey::PLT::PortBackDrop* backdrop;

    private:
        WarGrey::PLT::PStatus status;
    };
}
