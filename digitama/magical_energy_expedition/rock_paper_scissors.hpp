#pragma once // 确保只被 include 一次

#include "../big_bang/game.hpp"

#include <vector>

namespace WarGrey::AoC {
    /******************************************* 声明游戏状态 ******************************************/
    enum class RPSStatus {
        SimulateWithGuessedStrategy,
        SimulateWithDesignedStrategy,
        TaskDone
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
    class RochamboWorld : public WarGrey::STEM::World {
        public:
            RochamboWorld() : World("Rock Paper Scissors", 60) {}
            virtual ~RochamboWorld();

        public:  // 覆盖游戏基本方法
            void construct(int argc, char* argv[]) override;
            void load(float width, float height) override;
            void reflow(float width, float height) override;
            void update(uint32_t interval, uint32_t count, uint32_t uptime) override;

        public:
            bool can_select(WarGrey::STEM::IMatter* m) override;
            void after_select(WarGrey::STEM::IMatter* m, bool yes_or_no) override;

        private:
            void on_task_start(WarGrey::AoC::RPSStatus status);
            void on_task_done();

        private:
            int round_score(WarGrey::AoC::RPSShape self_shape, WarGrey::AoC::RPSOutcome outcome);

        private:
            void load_strategy(const std::string& pathname);

        private: // 本游戏世界中的物体
            WarGrey::STEM::Labellet* title_let;
            WarGrey::STEM::Labellet* info_board_let;
            WarGrey::STEM::Labellet* play_rules_let;
            WarGrey::STEM::Labellet* outcome_rules_let;
            WarGrey::STEM::Labellet* guessed_score_let;
            WarGrey::STEM::Labellet* designed_score_let;
            WarGrey::STEM::Labellet* outcome_let;
            WarGrey::STEM::Sprite* tux;
            
        private:
            WarGrey::AoC::RPSStatus status;
            std::vector<std::pair<char, char>> strategy;

        private: // Shared Variables
            int current_round = 0;
            int total_score = 0;
    };
}
