#include "rochambo.hpp"

#include "../aoc.hpp"

#include "../big_bang/datum/path.hpp"
#include "../big_bang/datum/flonum.hpp"

#include "../big_bang/physics/random.hpp"

#include <iostream>
#include <fstream>

using namespace WarGrey::STEM;
using namespace WarGrey::AoC;

/*************************************************************************************************/
static const char* guessed_strategy_desc = "猜测策略得分";
static const char* designed_strategy_desc = "精灵策略得分";
static const char* random_strategy_desc = "随机策略得分";

static const char* rule_score_fmt = "%d分";
static const char* outcome_fmt = "+%d +%d";

/*************************************************************************************************/
static inline RPSShape random_shape() {
    return static_cast<RPSShape>(random_uniform(1, 3));
}

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

static const char* r_play_names [] = { "scissor", "paper", "rock" };
static const int r_play_points  [] = { shape_score(RPSShape::Scissor), shape_score(RPSShape::Paper), shape_score(RPSShape::Rock) };
static const char* r_outcome_names [] = { "backflip", "idle-0", "duck-0" };
static const int r_outcome_points  [] = { outcome_score(RPSOutcome::Win), outcome_score(RPSOutcome::Draw), outcome_score(RPSOutcome::Lose) };

/*************************************************************************************************/
void WarGrey::AoC::RochamboPlane::construct(float width, float height) {
    this->load_strategy(digimon_path("mee/02.rps.dat"));
    this->style = make_highlight_dimension_style(answer_fontsize, 6U, 0);
}

void WarGrey::AoC::RochamboPlane::load(float width, float height) {
    this->logo = this->insert(new Sprite(digimon_path("logo", ".png")));
    this->titlet = this->insert(new Labellet(aoc_font::title, BLACK, title_fmt, 2, this->name()));
    this->guessed_score = this->insert(new Dimensionlet(this->style, "", guessed_strategy_desc));
    this->designed_score = this->insert(new Dimensionlet(this->style, "", designed_strategy_desc));
    this->random_score = this->insert(new Dimensionlet(this->style, "", random_strategy_desc));
    this->snack = this->insert(new SpriteGridSheet(digimon_path("sprite/snacks", ".png"), 3, 4, 2, 2));
    this->tux = this->insert(new Sprite(digimon_path("sprite/tux")));
    
    for (int idx = 0; idx < sizeof(this->play_icons) / sizeof(Sprite*); idx ++) {
        this->play_scores[idx] = this->insert(new Labellet(aoc_font::text, PURPLE, rule_score_fmt, r_play_points[idx]));
        this->play_icons[idx] = this->insert(new Sprite(digimon_path("sprite/rochambo")));
        this->play_icons[idx]->switch_to_custome(r_play_names[idx]);
        this->play_icons[idx]->scale(0.36F);
    }

    for (int idx = 0; idx < sizeof(this->outcome_icons) / sizeof(Sprite*); idx ++) {
        this->outcome_scores[idx] = this->insert(new Labellet(aoc_font::text, ROYALBLUE, rule_score_fmt, r_outcome_points[idx]));
        this->outcome_icons[idx] = this->insert(new Sprite(digimon_path("sprite/tux")));
        this->outcome_icons[idx]->switch_to_custome(r_outcome_names[idx]);
        this->outcome_icons[idx]->scale(0.618F);
    }

    this->change_mode(1);
    this->outcome_desc = this->insert(new Labellet(aoc_font::text, FORESTGREEN, " "));
    this->op_play = this->insert(new Sprite(digimon_path("sprite/rochambo")));
    this->sf_play = this->insert(new Sprite(digimon_path("sprite/rochambo")));
    this->op_play->scale(+0.618F, 0.618F);
    this->sf_play->scale(-0.618F, 0.618F);
    this->change_mode(0);
}

void WarGrey::AoC::RochamboPlane::reflow(float width, float height) {
    this->move_to(this->titlet, this->logo, MatterAnchor::RC, MatterAnchor::LC);
    this->move_to(this->guessed_score, this->logo, MatterAnchor::LB, MatterAnchor::LT, 0.0F, 1.0F);
    this->move_to(this->designed_score, this->guessed_score, MatterAnchor::LB, MatterAnchor::LT, 0.0F, 1.0F);
    this->move_to(this->random_score, this->designed_score, MatterAnchor::LB, MatterAnchor::LT, 0.0F, 1.0F);
    
    this->tux_x0 = width * 0.25F;
    this->tux_y0 = height * 0.618F;
    this->tux_distance = width * 0.5F;
    
    this->move_to(this->snack, width * 0.75F, this->tux_y0, MatterAnchor::LC);
    this->move_to(this->tux, this->tux_x0, this->tux_y0, MatterAnchor::RC);
    this->tux_step(0.0F);
    
    /* reflow scores */ {
        float xdist = float(text_fontsize);
        IMatter* prev = nullptr;

        for (int idx = 0; idx < sizeof(this->play_icons) / sizeof(Sprite*); idx ++) {
            if (prev == nullptr) {
                this->move_to(this->play_scores[idx], width, 0.0F, MatterAnchor::RT);
            } else {
                this->move_to(this->play_scores[idx], prev, MatterAnchor::LT, MatterAnchor::RT, -xdist);
            }
            this->move_to(this->play_icons[idx], this->play_scores[idx], MatterAnchor::LC, MatterAnchor::RC);
            prev = this->play_icons[idx];
        }

        for (int idx = 0; idx < sizeof(this->outcome_icons) / sizeof(Sprite*); idx ++) {
            this->move_to(this->outcome_icons[idx], this->play_icons[idx], MatterAnchor::LB, MatterAnchor::LT);
            this->move_to(this->outcome_scores[idx], this->play_scores[idx], 0.5F, this->outcome_icons[idx], 0.5F, MatterAnchor::CC);
        }
    }
}

void WarGrey::AoC::RochamboPlane::on_enter(IPlane* from) {
    this->on_task_done();
    
    this->tux->play("walk");
    this->snack->set_fps(2);
    this->snack->play("");
}

void WarGrey::AoC::RochamboPlane::update(uint32_t count, uint32_t interval, uint32_t uptime) {
    switch (this->status) {
        case RPSStatus::SimulateWithGuessedStrategy: {
            if (this->current_round < this->strategy.size()) {
                std::pair<char, char> play = this->strategy[this->current_round];
                RPSShape op_play = char_to_shape(play.first);
                RPSShape sf_play = char_to_shape(play.second);
                RPSOutcome outcome = round_end(op_play, sf_play);

                this->total_score += this->round_score(op_play, sf_play, outcome);
                this->guessed_score->set_value(this->total_score);
                
                this->current_round ++;
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

                this->total_score += this->round_score(op_play, sf_play, outcome);
                this->designed_score->set_value(this->total_score);
                
                this->current_round ++;
            } else {
                this->on_task_done();
            }
        }; break;
        case RPSStatus::SimulateWithRandomStrategy: {
            if (this->current_round < this->strategy.size()) {
                std::pair<char, char> play = this->strategy[this->current_round];
                RPSShape op_play = char_to_shape(play.first);
                RPSShape sf_play = random_shape();
                RPSOutcome outcome = round_end(op_play, sf_play);

                this->total_score += this->round_score(op_play, sf_play, outcome);
                this->random_score->set_value(this->total_score);
                
                this->current_round ++;
            } else {
                this->on_task_done();
            }
        }; break;
        default: /* do nothing */;
    }
}

void WarGrey::AoC::RochamboPlane::after_select(IMatter* m, bool yes_or_no) {
    if (yes_or_no) {
        if (m == this->guessed_score) {
            this->current_round = 0;
            this->total_score = 0;
            this->on_task_start(RPSStatus::SimulateWithGuessedStrategy);
        } else if (m == this->designed_score) {
            this->current_round = 0;
            this->total_score = 0;
            this->on_task_start(RPSStatus::SimulateWithDesignedStrategy);
        } else if (m == this->random_score) {
            this->current_round = 0;
            this->total_score = 0;
            this->guessed_score->set_value(this->guessed_final_score);
            this->designed_score->set_value(this->designed_final_score);
            this->on_task_start(RPSStatus::SimulateWithRandomStrategy);
        } else if (m == this->logo) {
            this->status = RPSStatus::MissionDone;
        }
    }
}

bool WarGrey::AoC::RochamboPlane::can_select(IMatter* m) {
    return (this->status == RPSStatus::TaskDone);
}

void WarGrey::AoC::RochamboPlane::on_task_start(RPSStatus status) {
    this->status = status;
    this->change_mode(1);
    this->move_to(this->tux, this->tux_x0, this->tux_y0, MatterAnchor::CC);
    this->tux_step(0.0F);
}

void WarGrey::AoC::RochamboPlane::on_task_done() {
    this->status = RPSStatus::TaskDone;
    this->change_mode(0);
}

int WarGrey::AoC::RochamboPlane::round_score(RPSShape op_shape, RPSShape sf_shape, RPSOutcome outcome) {
    int shpscore = shape_score(sf_shape);
    int outscore = outcome_score(outcome);
    int score = shpscore + outscore;
    
    switch (outcome) {
        case RPSOutcome::Lose: {
            this->outcome_desc->set_text_color(CRIMSON);
            this->tux->switch_to_custome(r_outcome_names[2]);
            this->outcome_desc->set_text(MatterAnchor::CC, outcome_fmt, shpscore, outscore);
        }; break;
        case RPSOutcome::Win: {
            this->outcome_desc->set_text_color(FORESTGREEN);
            this->tux->switch_to_custome(r_outcome_names[0]);
            this->outcome_desc->set_text(MatterAnchor::CC, outcome_fmt, shpscore, outscore);
        }; break;
        case RPSOutcome::Draw: {
            this->outcome_desc->set_text_color(DARKORANGE);
            this->tux->switch_to_custome(r_outcome_names[1]);
            this->outcome_desc->set_text(MatterAnchor::CC, outcome_fmt, shpscore, outscore);
        }; break;
    }

    this->switch_play_custome(this->op_play, op_shape);
    this->switch_play_custome(this->sf_play, sf_shape);
    this->tux_step(float(score) / this->distance_score * this->tux_distance);

    return score;
}

void WarGrey::AoC::RochamboPlane::tux_step(float dx) {
    this->move(this->tux, dx, 0.0F);
    this->move_to(this->op_play, this->tux, MatterAnchor::LT, MatterAnchor::RB);
    this->move_to(this->sf_play, this->tux, MatterAnchor::RT, MatterAnchor::LB);
    this->move_to(this->outcome_desc, this->tux, MatterAnchor::CB, MatterAnchor::CT);
}

void WarGrey::AoC::RochamboPlane::switch_play_custome(Sprite* target, RPSShape play) {
    switch (play) {
        case RPSShape::Rock: target->switch_to_custome("rock"); break;
        case RPSShape::Paper: target->switch_to_custome("paper"); break;
        case RPSShape::Scissor: target->switch_to_custome("scissor"); break;
        default: ;
    }
}

/*************************************************************************************************/
void WarGrey::AoC::RochamboPlane::load_strategy(const std::string& pathname) {
    std::ifstream datin(pathname);
    
    this->strategy.clear();
    this->guessed_final_score = 0;
    this->designed_final_score = 0;
    
    if (datin.is_open()) {
        std::string line;
        
        while (std::getline(datin, line)) {
            if (line.size() >= 3) {
                char op_char = line[0];
                char sf_char = line[2];
                RPSShape op_play = char_to_shape(op_char);
                RPSShape sf_play = char_to_shape(sf_char);
                RPSOutcome outcome = char_to_outcome(sf_char);
            
                if ((op_play != RPSShape::_) && (sf_play != RPSShape::_)) {
                    this->strategy.push_back(std::pair<char, char>(op_char, sf_char));
                
                    this->guessed_final_score += shape_score(sf_play) + outcome_score(round_end(op_play, sf_play));
                    this->designed_final_score += shape_score(smart_shape(op_play, outcome)) + outcome_score(outcome);
                }
            }
        }

        datin.close();
    }

    this->distance_score = flmax(float(this->guessed_final_score), float(this->designed_final_score), 1.0F);
}
