#pragma once // 确保只被 include 一次

#include "../big_bang/game.hpp"

#include <vector>

namespace WarGrey::AoC {
    /******************************************* 声明游戏状态 ******************************************/
    enum class RPSStatus {
        SimulateWithGuessedStrategy,
        SimulateWithDesignedStrategy,
        TaskDone,
        MissionDone
    };

    enum class RPSShape {
        Rock = 1,
        Paper = 2,
        Scissor = 3,
        _ = 0
    };

    enum class RPSOutcome {
        Win = 6,
        Draw = 3,
        Lose = 0,

        // this is dangerous unless you really want to
        //   treat all invalid inputs as loses.  
        _ = 0
    };

    /******************************************* 声明游戏世界 ******************************************/
    class RochamboPlane : public WarGrey::STEM::Plane {
    public:
        RochamboPlane() : Plane("猜拳大赛") {}
        virtual ~RochamboPlane() {}

    public:  // 覆盖游戏基本方法
        void construct(float width, float height) override;
        void load(float width, float height) override;
        void reflow(float width, float height) override;
        void update(uint32_t count, uint32_t interval, uint32_t uptime) override;

    public:
        bool can_select(WarGrey::STEM::IMatter* m) override;
        void after_select(WarGrey::STEM::IMatter* m, bool yes_or_no) override;

    public:
        bool has_mission_completed() override { return this->status == RPSStatus::MissionDone; }
        void on_transfer(IPlane* from, IPlane* to) override;

    private:
        void on_task_start(WarGrey::AoC::RPSStatus status);
        void on_task_done();

    private:
        int round_score(WarGrey::AoC::RPSShape self_shape, WarGrey::AoC::RPSOutcome outcome);

    private:
        void load_strategy(const std::string& pathname);

    private: // 本游戏世界中的物体
        WarGrey::STEM::Sprite* logo;
        WarGrey::STEM::Labellet* title_let;
        WarGrey::STEM::Labellet* info_board_let;
        WarGrey::STEM::Labellet* play_rules_let;
        WarGrey::STEM::Labellet* outcome_rules_let;
        WarGrey::STEM::Dimensionlet* guessed_score_let;
        WarGrey::STEM::Dimensionlet* designed_score_let;
        WarGrey::STEM::Labellet* outcome_let;
        WarGrey::STEM::Sprite* tux;
        
    private:
        WarGrey::STEM::DimensionStyle style;
        WarGrey::AoC::RPSStatus status;
        std::vector<std::pair<char, char>> strategy;
        
    private: // Shared Variables
        int current_round = 0;
        int total_score = 0;
    };
}
