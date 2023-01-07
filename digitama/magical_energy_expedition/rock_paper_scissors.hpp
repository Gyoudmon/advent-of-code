#pragma once // 确保只被 include 一次

#include "../big_bang/game.hpp"

#include <vector>

namespace WarGrey::AoC {
    /******************************************* 声明游戏状态 ******************************************/
    enum class RPSStatus {
        SimulateWithGuessedStrategy,
        TaskDone
    };

    enum class RPSShape {
        Rock,
        Paper,
        Scissor,
        _
    };

    /******************************************* 声明游戏世界 ******************************************/
    class RockPaperScissorsWorld : public WarGrey::STEM::World {
        public:
            RockPaperScissorsWorld() : World("Rock Paper Scissors", 60) {}
            virtual ~RockPaperScissorsWorld();

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
            void display_outcome(int selection_score, int outcome_score);

        private:
            void load_strategy(const std::string& pathname);

        private: // 本游戏世界中的物体
            WarGrey::STEM::Labellet* title_let;
            WarGrey::STEM::Labellet* info_board_let;
            WarGrey::STEM::Labellet* selection_rules_let;
            WarGrey::STEM::Labellet* outcome_rules_let;
            WarGrey::STEM::Labellet* guessed_score_let;
            //WarGrey::STEM::Labellet* elf_score_let;
            WarGrey::STEM::Labellet* outcome_let;
            std::vector<WarGrey::STEM::Sprite*> elves;
            
        private:
            WarGrey::AoC::RPSStatus status;
            std::vector<std::pair<char, char>> strategy;

        private: // Shared Variables
            int current_round = 0;
            int total_score = 0;
    };
}
