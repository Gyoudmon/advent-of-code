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
static const char* title_desc = "石头 剪刀 布大赛";
static const char* tournament_desc = "大赛总局数";
static const char* guessed_strategy_desc = "精灵策略得分";

static const char* selection_rule_fmt = "出‘石头’得%d分\n   出‘布’得%d分\n出‘剪刀’得%d分";
static const char* outcome_rule_fmt = "输了得%d分\n赢了得%d分\n平局得%d分";
static const char* outcome_fmt = "%s 得分 %d (%d + %d)";

static const int rock_point = 1;
static const int paper_point = 2;
static const int scissor_point = 3;

static const int win_point = 6;
static const int lose_point = 0;
static const int draw_point = 3;

/*************************************************************************************************/
static inline RPSSelection char_to_selection(char ch) {
    switch (ch) {
        case 'A': case 'X': return RPSSelection::Rock; break;
        case 'B': case 'Y': return RPSSelection::Paper; break;
        case 'C': case 'Z': return RPSSelection::Scissor; break;
        default: return RPSSelection::_; break;
    }
}

static inline int selection_score(RPSSelection selection) {
    int score = 0;

    switch (selection) {
        case RPSSelection::Rock: score = rock_point; break;
        case RPSSelection::Paper: score = paper_point; break;
        case RPSSelection::Scissor: score = scissor_point; break;
        default: /**/;
    }

    return score;
}

static inline int round_score(RPSSelection opponent, RPSSelection self) {
    int score = 0;

    if (opponent == self) {
        score = draw_point;
    } else if (self == RPSSelection::Rock) {
        score = (opponent == RPSSelection::Paper) ? lose_point : win_point;
    } else if (self == RPSSelection::Paper) {
        score = (opponent == RPSSelection::Scissor) ? lose_point : win_point;
    } else if (self == RPSSelection::Scissor) {
        score = (opponent == RPSSelection::Rock) ? lose_point : win_point;
    }

    return score;
}

/*************************************************************************************************/
void WarGrey::AoC::RockPaperScissorsWorld::construct(int argc, char* argv[]) {
    std::string datin;

    for (int idx = 1; idx < argc; idx ++) {
        datin = std::string(argv[idx]);
    }
    
    if (datin.size() > 0) {
        this->load_strategy(datin);
    } else {
        this->load_strategy(digimon_path("2022/02.rps.dat"));
    }

    aoc_fonts_initialize();
}

WarGrey::AoC::RockPaperScissorsWorld::~RockPaperScissorsWorld() {
    aoc_fonts_destroy();
}

void WarGrey::AoC::RockPaperScissorsWorld::load(float width, float height) {
    this->title_let = this->insert(new Labellet(aoc_font::title, BLACK, title_fmt, 2, title_desc));
    this->info_board_let = this->insert(new Labellet(aoc_font::text, GRAY, puzzle_fmt, tournament_desc, this->strategy.size()));
    this->selection_rules_let = this->insert(new Labellet(aoc_font::text, PURPLE, selection_rule_fmt, rock_point, paper_point, scissor_point));
    this->outcome_rules_let = this->insert(new Labellet(aoc_font::text, ROYALBLUE, outcome_rule_fmt, lose_point, win_point, draw_point));
    this->guessed_score_let = this->insert(new Labellet(aoc_font::text, GOLDENROD, unknown_fmt, guessed_strategy_desc));
    this->outcome_let = this->insert(new Labellet(aoc_font::text, FORESTGREEN, " "));

    this->on_task_done();
}

void WarGrey::AoC::RockPaperScissorsWorld::reflow(float width, float height) {
    this->move_to(this->info_board_let, width, 0.0F, MatterAnchor::RT);
    this->move_to(this->guessed_score_let, this->title_let, MatterAnchor::LB, MatterAnchor::LT);
    this->move_to(this->selection_rules_let, this->info_board_let, MatterAnchor::RB, MatterAnchor::RT);
    this->move_to(this->outcome_rules_let, this->selection_rules_let, MatterAnchor::RB, MatterAnchor::RT);
    this->move_to(this->outcome_let, width * 0.5F, height * 0.25F, MatterAnchor::CC);
}

void WarGrey::AoC::RockPaperScissorsWorld::update(uint32_t interval, uint32_t count, uint32_t uptime) {
    switch (this->status) {
        case RPSStatus::PlayFollowStrategy: {
            if (this->current_round < this->strategy.size()) {
                std::pair<RPSSelection, char> play = this->strategy[this->current_round];
                RPSSelection self = char_to_selection(play.second);
                int selscore = selection_score(self);
                int outscore = round_score(play.first, self);

                this->total_score += (selscore + outscore);
                this->guessed_score_let->set_text(puzzle_fmt, guessed_strategy_desc, this->total_score);
                this->display_outcome(selscore, outscore);
                
                this->current_round ++;
            } else {
                this->guessed_score_let->set_text(puzzle_fmt, guessed_strategy_desc, this->total_score);
                this->on_task_done();
            }
        }; break;
        default: /* do nothing */;
    }
}

void WarGrey::AoC::RockPaperScissorsWorld::after_select(IMatter* m, bool yes_or_no) {
    if (yes_or_no) {
        if (m == this->guessed_score_let) {
            this->current_round = 0;
            this->total_score = 0;
            this->on_task_start(RPSStatus::PlayFollowStrategy);
        }
    }
}

bool WarGrey::AoC::RockPaperScissorsWorld::can_select(IMatter* m) {
    return (this->status == RPSStatus::TaskDone);
}

void WarGrey::AoC::RockPaperScissorsWorld::on_task_start(RPSStatus status) {
    this->status = status;
}

void WarGrey::AoC::RockPaperScissorsWorld::on_task_done() {
    this->status = RPSStatus::TaskDone;
}

void WarGrey::AoC::RockPaperScissorsWorld::display_outcome(int selection_score, int outcome_score) {
    int round_score = selection_score + outcome_score;
    
    switch (outcome_score) {
        case lose_point: {
            this->outcome_let->set_text_color(CRIMSON);
            this->outcome_let->set_text(MatterAnchor::CC, outcome_fmt, "输了", round_score, selection_score, outcome_score);
        }; break;
        case win_point: {
            this->outcome_let->set_text_color(FORESTGREEN);
            this->outcome_let->set_text(MatterAnchor::CC, outcome_fmt, "赢了", round_score, selection_score, outcome_score);
        }; break;
        case draw_point: {
            this->outcome_let->set_text_color(DARKORANGE);
            this->outcome_let->set_text(MatterAnchor::CC, outcome_fmt, "平局", round_score, selection_score, outcome_score);
        }; break;
    }
}

/*************************************************************************************************/
void WarGrey::AoC::RockPaperScissorsWorld::load_strategy(const std::string& pathname) {
    std::ifstream datin(pathname);
    
    this->strategy.clear();

    if (datin.is_open()) {    
        std::string line;

        while (std::getline(datin, line)) {
            if (line.size() >= 3) {
                RPSSelection opponent = char_to_selection(line[0]);
                char self_char = line[2];
                RPSSelection self = char_to_selection(self_char);

                if ((opponent != RPSSelection::_) && (self != RPSSelection::_)) {
                    this->strategy.push_back(std::pair<RPSSelection, char>(opponent, self_char));
                }
            }
        }

        datin.close();
    }
}
