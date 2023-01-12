#include "rock_paper_scissors.hpp"

#include "../aoc.hpp"

#include "../big_bang/datum/path.hpp"
#include "../big_bang/datum/flonum.hpp"
#include "../big_bang/datum/vector.hpp"

#include "../big_bang/physics/random.hpp"

#include <iostream>
#include <fstream>
#include <string>

using namespace WarGrey::STEM;
using namespace WarGrey::AoC;

/*************************************************************************************************/
static const char* title_desc = "猜拳大赛";
static const char* tournament_desc = "剩余次数";
static const char* guessed_strategy_desc = "猜测策略得分";
static const char* designed_strategy_desc = "精灵策略得分";

static const char* play_rule_fmt = "出‘石头’得%d分\n   出‘布’得%d分\n出‘剪刀’得%d分";
static const char* outcome_rule_fmt = "输了得%d分\n赢了得%d分\n平局得%d分";
static const char* outcome_fmt = "%s 得分 %d (%d + %d)";

/*************************************************************************************************/
static inline RPSShape char_to_shape(char ch) {
    switch (ch) {
        case 'A': case 'X': return RPSShape::Rock; break;
        case 'B': case 'Y': return RPSShape::Paper; break;
        case 'C': case 'Z': return RPSShape::Scissor; break;
        default: return RPSShape::_; break;
    }
}

static inline RPSOutcome char_to_outcome(char ch) {
    switch (ch) {
        case 'X': return RPSOutcome::Lose; break;
        case 'Y': return RPSOutcome::Draw; break;
        case 'Z': return RPSOutcome::Win; break;
        default: return RPSOutcome::_; break;
    }
}

static inline int shape_score(RPSShape shape) {
    return static_cast<int>(shape);
}

static inline int outcome_score(RPSOutcome outcome) {
    return static_cast<int>(outcome);
}

static inline RPSOutcome round_end(RPSShape opponent, RPSShape self) {
    RPSOutcome end = RPSOutcome::_;

    if (opponent == self) {
        end = RPSOutcome::Draw;
    } else if (self == RPSShape::Rock) {
        end = (opponent == RPSShape::Paper) ? RPSOutcome::Lose : RPSOutcome::Win;
    } else if (self == RPSShape::Paper) {
        end = (opponent == RPSShape::Scissor) ? RPSOutcome::Lose : RPSOutcome::Win;
    } else if (self == RPSShape::Scissor) {
        end = (opponent == RPSShape::Rock) ? RPSOutcome::Lose : RPSOutcome::Win;
    }

    return end;
}

static inline RPSShape smart_shape(RPSShape op_play, RPSOutcome outcome) {
    RPSShape shape = RPSShape::_;

    if (outcome == RPSOutcome::Draw) {
        shape = op_play;
    } else if (op_play == RPSShape::Rock) {
        shape = (outcome == RPSOutcome::Win) ? RPSShape::Paper : RPSShape::Scissor;
    } else if (op_play == RPSShape::Paper) {
        shape = (outcome == RPSOutcome::Win) ? RPSShape::Scissor : RPSShape::Rock;
    } else if (op_play == RPSShape::Scissor) {
        shape = (outcome == RPSOutcome::Win) ? RPSShape::Rock : RPSShape::Paper;
    }

    return shape;
}

/*************************************************************************************************/
void WarGrey::AoC::RochamboWorld::construct(int argc, char* argv[]) {
    std::string datin;

    for (int idx = 1; idx < argc; idx ++) {
        datin = std::string(argv[idx]);
    }
    
    if (datin.size() > 0) {
        this->load_strategy(datin);
    } else {
        this->load_strategy(digimon_path("mee/02.rps.dat"));
    }

    aoc_fonts_initialize();
}

WarGrey::AoC::RochamboWorld::~RochamboWorld() {
    aoc_fonts_destroy();
}

void WarGrey::AoC::RochamboWorld::load(float width, float height) {
    this->title_let = this->insert(new Labellet(aoc_font::title, BLACK, title_fmt, 2, title_desc));
    this->info_board_let = this->insert(new Labellet(aoc_font::text, GRAY, puzzle_fmt, tournament_desc, this->strategy.size()));
    this->guessed_score_let = this->insert(new Labellet(aoc_font::text, GOLDENROD, unknown_fmt, guessed_strategy_desc));
    this->designed_score_let = this->insert(new Labellet(aoc_font::text, SPRINGGREEN, unknown_fmt, designed_strategy_desc));
    this->outcome_let = this->insert(new Labellet(aoc_font::text, FORESTGREEN, " "));
    this->tux = this->insert(new Sprite(digimon_path("tux", "", "stone/sprite")));

    this->play_rules_let = this->insert(new Labellet(aoc_font::text, PURPLE, play_rule_fmt,
            shape_score(RPSShape::Rock), shape_score(RPSShape::Paper), shape_score(RPSShape::Scissor)));
    this->outcome_rules_let = this->insert(new Labellet(aoc_font::text, ROYALBLUE, outcome_rule_fmt,
            outcome_score(RPSOutcome::Lose), outcome_score(RPSOutcome::Win), outcome_score(RPSOutcome::Draw)));
    
    this->on_task_done();
}

void WarGrey::AoC::RochamboWorld::reflow(float width, float height) {
    this->move_to(this->info_board_let, width, 0.0F, MatterAnchor::RT);
    this->move_to(this->guessed_score_let, this->title_let, MatterAnchor::LB, MatterAnchor::LT);
    this->move_to(this->designed_score_let, this->guessed_score_let, MatterAnchor::LB, MatterAnchor::LT);
    this->move_to(this->play_rules_let, this->info_board_let, MatterAnchor::RB, MatterAnchor::RT);
    this->move_to(this->outcome_rules_let, this->play_rules_let, MatterAnchor::RB, MatterAnchor::RT);
    this->move_to(this->outcome_let, width * 0.5F, height * 0.25F, MatterAnchor::CC);
    this->move_to(this->tux, width * 0.5F, height * 0.5F, MatterAnchor::CC);
    this->tux->play("walk");
}

void WarGrey::AoC::RochamboWorld::update(uint32_t interval, uint32_t count, uint32_t uptime) {
    switch (this->status) {
        case RPSStatus::SimulateWithGuessedStrategy: {
            if (this->current_round < this->strategy.size()) {
                std::pair<char, char> play = this->strategy[this->current_round];
                RPSShape op_play = char_to_shape(play.first);
                RPSShape sf_play = char_to_shape(play.second);
                RPSOutcome outcome = round_end(op_play, sf_play);

                this->total_score += this->round_score(sf_play, outcome);
                this->guessed_score_let->set_text(puzzle_fmt, guessed_strategy_desc, this->total_score);
                
                this->current_round ++;
                this->info_board_let->set_text(MatterAnchor::RC, puzzle_fmt, tournament_desc, this->strategy.size() - this->current_round);
            } else {
                this->on_task_done();
            }
        }; break;
        case RPSStatus::SimulateWithDesignedStrategy: {
            if (this->current_round < this->strategy.size()) {
                std::pair<char, char> play = this->strategy[this->current_round];
                RPSShape op_play = char_to_shape(play.first);
                RPSOutcome outcome = char_to_outcome(play.second);
                RPSShape sf_play = smart_shape(op_play, outcome);

                this->total_score += this->round_score(sf_play, outcome);
                this->designed_score_let->set_text(puzzle_fmt, designed_strategy_desc, this->total_score);
                
                this->current_round ++;
                this->info_board_let->set_text(MatterAnchor::RC, puzzle_fmt, tournament_desc, this->strategy.size() - this->current_round);
            } else {
                this->on_task_done();
            }
        }; break;
        default: /* do nothing */;
    }
}

void WarGrey::AoC::RochamboWorld::after_select(IMatter* m, bool yes_or_no) {
    if (yes_or_no) {
        if (m == this->guessed_score_let) {
            this->current_round = 0;
            this->total_score = 0;
            this->on_task_start(RPSStatus::SimulateWithGuessedStrategy);
        } else if (m == this->designed_score_let) {
            this->current_round = 0;
            this->total_score = 0;
            this->on_task_start(RPSStatus::SimulateWithDesignedStrategy);
        }
    }
}

bool WarGrey::AoC::RochamboWorld::can_select(IMatter* m) {
    return (this->status == RPSStatus::TaskDone);
}

void WarGrey::AoC::RochamboWorld::on_task_start(RPSStatus status) {
    this->status = status;
}

void WarGrey::AoC::RochamboWorld::on_task_done() {
    this->status = RPSStatus::TaskDone;
}

int WarGrey::AoC::RochamboWorld::round_score(RPSShape self_shape, RPSOutcome outcome) {
    int shpscore = shape_score(self_shape);
    int outscore = outcome_score(outcome);
    int score = shpscore + outscore;
    
    switch (outcome) {
        case RPSOutcome::Lose: {
            this->outcome_let->set_text_color(CRIMSON);
            this->outcome_let->set_text(MatterAnchor::CC, outcome_fmt, "输了", score, shpscore, outscore);
        }; break;
        case RPSOutcome::Win: {
            this->outcome_let->set_text_color(FORESTGREEN);
            this->outcome_let->set_text(MatterAnchor::CC, outcome_fmt, "赢了", score, shpscore, outscore);
        }; break;
        case RPSOutcome::Draw: {
            this->outcome_let->set_text_color(DARKORANGE);
            this->outcome_let->set_text(MatterAnchor::CC, outcome_fmt, "平局", score, shpscore, outscore);
        }; break;
    }

    return score;
}

/*************************************************************************************************/
void WarGrey::AoC::RochamboWorld::load_strategy(const std::string& pathname) {
    std::ifstream datin(pathname);
    
    this->strategy.clear();

    if (datin.is_open()) {    
        std::string line;

        while (std::getline(datin, line)) {
            if (line.size() >= 3) {
                char op_char = line[0];
                char sf_char = line[2];
                RPSShape op_play = char_to_shape(op_char);
                RPSShape sf_play = char_to_shape(sf_char);
            
                if ((op_play != RPSShape::_) && (sf_play != RPSShape::_)) {
                    this->strategy.push_back(std::pair<char, char>(op_char, sf_char));
                }
            }
        }

        datin.close();
    }
}
