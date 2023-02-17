#include "rochambo.hpp"

using namespace WarGrey::STEM;
using namespace WarGrey::AoC;

/*************************************************************************************************/
static const char* population_desc = "大赛剩余轮数";
static const char* guessed_strategy_desc = "猜测策略得分";
static const char* designed_strategy_desc = "精灵策略得分";
static const char* random_strategy_desc = "随机策略得分";

static const char* rule_score_fmt = "%d分";
static const char* outcome_fmt = "+%d +%d";

/*************************************************************************************************/
static inline RPSShape random_shape() {
    return static_cast<RPSShape>(random_uniform(1, 3));
}

static inline RPSShape name_to_shape(const char* name) {
    RPSShape shape = RPSShape::_;

    if (strcmp(name, "rock") == 0) {
        shape = RPSShape::Rock;
    } else if (strcmp(name, "paper") == 0) {
        shape = RPSShape::Paper;
    } else if (strcmp(name, "scissor") == 0) {
        shape = RPSShape::Scissor;
    }

    return shape;
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

static inline RPSOutcome round_outcome(RPSShape opponent, RPSShape self) {
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
static const char* r_outcome_names [] = { "lshoot", "lthrust", "hurt" };
static const int r_outcome_points  [] = { outcome_score(RPSOutcome::Win), outcome_score(RPSOutcome::Draw), outcome_score(RPSOutcome::Lose) };

/*************************************************************************************************/
void WarGrey::AoC::RochamboPlane::construct(float width, float height) {
    this->load_strategy(digimon_path("mee/02_rps", aoc_ext));
    this->style = make_highlight_dimension_style(answer_fontsize, 6U, 0);
}

void WarGrey::AoC::RochamboPlane::load(float width, float height) {
    this->backdrop = this->insert(new GridAtlas("backdrop/beach.png"));
    this->titlet = this->insert(new Labellet(aoc_font::title, BLACK, title_fmt, 2, this->name()));
    this->population = this->insert(new Labellet(aoc_font::normal, GOLDENROD, puzzle_fmt, population_desc, this->strategy.size()));
    this->guessed_score = this->insert(new Dimensionlet(this->style, "", guessed_strategy_desc));
    this->designed_score = this->insert(new Dimensionlet(this->style, "", designed_strategy_desc));
    this->random_score = this->insert(new Dimensionlet(this->style, "", random_strategy_desc));
    this->tent = this->insert(new SpriteGridSheet("spritesheet/tents.png", 1, 4));
    this->snack = this->insert(new SpriteGridSheet("spritesheet/snacks.png", 3, 4, 2, 2));
    
    this->elves[0] = this->insert(new ElfSheet("dress"));
    this->tux = this->insert(new Tuxmon());
    this->elves[1] = this->insert(new ElfSheet("male"));
    
    this->agent = this->insert(new Linkmon());
    this->agent->scale(-1.0F, 1.0F);

    for (int idx = 0; idx < sizeof(this->play_icons) / sizeof(Sprite*); idx ++) {
        this->play_scores[idx] = this->insert(new Labellet(aoc_font::normal, PURPLE, rule_score_fmt, r_play_points[idx]));
        this->play_icons[idx] = this->insert(new Sprite("sprite/rochambo"));
        this->play_icons[idx]->switch_to_costume(r_play_names[idx]);
        this->play_icons[idx]->scale(0.36F);
    }

    for (int idx = 0; idx < sizeof(this->outcome_icons) / sizeof(Sprite*); idx ++) {
        this->outcome_scores[idx] = this->insert(new Labellet(aoc_font::normal, ROYALBLUE, rule_score_fmt, r_outcome_points[idx]));
        this->outcome_icons[idx] = this->insert(new ElfSheet("santa_claus"));
        this->outcome_icons[idx]->play(r_outcome_names[idx]);
        this->outcome_icons[idx]->scale(0.80F);
    }

    this->outcome_desc = this->insert(new Labellet(aoc_font::normal, FORESTGREEN, " "));
    this->l_play = this->insert(new Sprite("sprite/rochambo"));
    this->r_play = this->insert(new Sprite("sprite/rochambo"));

    this->tent->scale(0.80F);
    this->l_play->scale(+0.618F, 0.618F);
    this->r_play->scale(-0.618F, 0.618F);
}

void WarGrey::AoC::RochamboPlane::reflow(float width, float height) {
    this->move_to(this->titlet, this->agent, MatterAnchor::RB, MatterAnchor::LB);
    this->move_to(this->population, this->agent, MatterAnchor::LB, MatterAnchor::LT);
    this->move_to(this->guessed_score, this->population, MatterAnchor::LB, MatterAnchor::LT, 0.0F, 1.0F);
    this->move_to(this->designed_score, this->guessed_score, MatterAnchor::LB, MatterAnchor::LT, 0.0F, 1.0F);
    this->move_to(this->random_score, this->designed_score, MatterAnchor::LB, MatterAnchor::LT, 0.0F, 1.0F);
    
    this->backdrop->resize(width, 0.0F);
    this->move_to(this->backdrop, 0.0, height, MatterAnchor::LB);
    
    this->round_x0 = width * 0.64F;
    this->round_y0 = height * 0.36F;

    this->race_x0 = width * 0.25F;
    this->race_y0 = height * 0.88F;
    this->race_distance = width * 0.5F;
    
    this->move_to(this->snack, this->race_x0 + this->race_distance, this->race_y0, MatterAnchor::LC);
    this->move_to(this->tent, 0.0, height, MatterAnchor::LB);
    this->move_to(this->tux, this->tent, 0.75F, 0.7F, MatterAnchor::CC);
    
    /* reflow scores */ {
        float xdist = float(normal_fontsize);
        IMatter* prev = nullptr;

        for (int idx = 0; idx < sizeof(this->play_icons) / sizeof(Sprite*); idx ++) {
            if (prev == nullptr) {
                this->move_to(this->play_scores[idx], width, 0.0F, MatterAnchor::RT, -8.0F, 8.0F);
            } else {
                this->move_to(this->play_scores[idx], prev, MatterAnchor::LC, MatterAnchor::RC, -xdist);
            }
            this->move_to(this->play_icons[idx], this->play_scores[idx], MatterAnchor::LC, MatterAnchor::RC);
            prev = this->play_icons[idx];
        }

        for (int idx = 0; idx < sizeof(this->outcome_icons) / sizeof(Sprite*); idx ++) {
            this->move_to(this->outcome_icons[idx], this->play_icons[idx], MatterAnchor::CB, MatterAnchor::CT);
            this->move_to(this->outcome_scores[idx], this->play_scores[idx], 0.5F, this->outcome_icons[idx], 0.5F, MatterAnchor::CC);
        }
    }
}

void WarGrey::AoC::RochamboPlane::on_enter(IPlane* from) {
    this->on_task_done();
    
    this->agent->play("Greeting", 1);
    this->snack->play();

    this->set_matter_fps(this->tux, 10);
    this->set_matter_fps(this->snack, 2);
}

void WarGrey::AoC::RochamboPlane::update(uint32_t count, uint32_t interval, uint32_t uptime) {
    switch (this->status) {
        case RPSStatus::SimulateTheTournament: {
            if (this->current_round < this->strategy.size()) {
                std::pair<char, char> play = this->strategy[this->current_round];
                RPSShape op_play = char_to_shape(play.first);
                RPSShape sf_play_g = char_to_shape(play.second);
                RPSOutcome outcome_g = round_outcome(op_play, sf_play_g);
                RPSOutcome outcome_d = char_to_outcome(play.second);
                RPSShape sf_play_d = smart_shape(op_play, outcome_d);
                RPSShape sf_play_r = random_shape();
                RPSOutcome outcome_r = round_outcome(op_play, sf_play_r);

                this->switch_play_costume(this->l_play, op_play);
                this->switch_play_costume(this->r_play, sf_play_g);

                this->guessed_total_score += this->round_score(this->tux, op_play, sf_play_g, outcome_g);
                this->guessed_score->set_value(float(this->guessed_total_score));

                this->designed_total_score += this->round_score(this->elves[0], op_play, sf_play_d, outcome_d);
                this->designed_score->set_value(float(this->designed_total_score));

                this->random_total_score += this->round_score(this->elves[1], op_play, sf_play_r, outcome_r);
                this->random_score->set_value(float(this->random_total_score));
                
                this->current_round ++;
                this->population->set_text(puzzle_fmt, population_desc, this->strategy.size() - this->current_round);
            } else {
                this->population->set_text(puzzle_fmt, population_desc, this->strategy.size());
                this->on_task_done();
            }
        }; break;
        default: {
            if (this->round_self == nullptr) {
                if (count % 2 == 0) {
                    this->l_play->switch_to_random_costume();
                    this->r_play->switch_to_random_costume();
                }
            }
        };
    }
}

void WarGrey::AoC::RochamboPlane::after_select(IMatter* m, bool yes_or_no) {
    if (yes_or_no) {
        if ((m == this->guessed_score) || (m == this->designed_score) || (m == this->random_score) || (m == this->tux)) {
            this->current_round = 0;
            this->guessed_total_score = 0;
            this->designed_total_score = 0;
            this->random_total_score = 0;
            this->on_task_start(RPSStatus::SimulateTheTournament);
        } else if ((m == this->elves[0]) || (m == this->elves[1])) {
            if (this->round_self == nullptr) {
                if (m == this->elves[0]) {
                    this->round_self = this->l_play;
                    this->move_to(this->outcome_desc, this->elves[0], MatterAnchor::LT, MatterAnchor::RT);
                    this->round_score(this->r_play, this->l_play);
                } else if (m == this->elves[1]) {
                    this->round_self = this->r_play;
                    this->move_to(this->outcome_desc, this->elves[1], MatterAnchor::RT, MatterAnchor::LT);
                    this->round_score(this->l_play, this->r_play);
                }
            } else {
                this->round_self = nullptr;
                this->on_task_done();
            }
        } else if (m == this->agent) {
            this->agent->play("GoodBye", 1);
            this->status = RPSStatus::MissionDone;
        }
    } else {
        if ((m == this->elves[0]) || (m == this->elves[1])) {
            this->round_self = nullptr;
            this->on_task_done();
        }
    }
}

bool WarGrey::AoC::RochamboPlane::can_select(IMatter* m) {
    return (this->status == RPSStatus::TaskDone);
}

bool WarGrey::AoC::RochamboPlane::has_mission_completed() {
    return (this->status == RPSStatus::MissionDone) && (!this->agent->in_playing());
}

void WarGrey::AoC::RochamboPlane::on_task_start(RPSStatus status) {
    this->status = status;

    this->round_self = nullptr;

    this->elves[0]->scale_to(1.0F);
    this->elves[1]->scale_to(1.0F);
    
    this->move_to(this->tux, this->race_x0, this->race_y0, MatterAnchor::CC);
    this->move_to(elves[0], this->tux, 0.5F, 0.3F, MatterAnchor::CB);
    this->move_to(elves[1], this->tux, 0.5F, 0.7F, MatterAnchor::CT);

    this->move_to(this->l_play, this->snack, MatterAnchor::LT, MatterAnchor::LB);
    this->move_to(this->r_play, this->snack, MatterAnchor::RT, MatterAnchor::RB);

    this->elves[0]->play("rwalk");
    this->elves[1]->play("rwalk");
    this->tux->play("walk");

    this->agent->play("Processing");
}

void WarGrey::AoC::RochamboPlane::on_task_done() {
    this->status = RPSStatus::TaskDone;

    this->elves[0]->scale_to(1.618F);
    this->elves[1]->scale_to(1.618F);

    this->move_to(this->elves[0], this->round_x0, this->round_y0, MatterAnchor::RC);
    this->move_to(this->elves[1], this->elves[0], MatterAnchor::RC, MatterAnchor::LC);
    
    this->move_to(this->l_play, this->elves[0], MatterAnchor::CT, MatterAnchor::CB);
    this->move_to(this->r_play, this->elves[1], MatterAnchor::CT, MatterAnchor::CB);
    
    this->agent->stop(1);
    this->tux->stop();
    this->tux->switch_to_costume("idle-0");
    this->elves[0]->play("rslash");
    this->elves[1]->play("lslash");
    this->outcome_desc->set_text("");
}

int WarGrey::AoC::RochamboPlane::round_score(ISprite* racer, RPSShape op_shape, RPSShape sf_shape, RPSOutcome outcome) {
    int shpscore = shape_score(sf_shape);
    int outscore = outcome_score(outcome);
    int score = shpscore + outscore;

    this->move(racer, float(score) / this->distance_score * this->race_distance, 0.0F);

    return score;
}

int WarGrey::AoC::RochamboPlane::round_score(Sprite* opponent, Sprite* self) {
    RPSShape op_play = name_to_shape(opponent->current_costume_name());
    RPSShape sf_play = name_to_shape(self->current_costume_name());
    RPSOutcome outcome = round_outcome(op_play, sf_play);
    int shpscore = shape_score(sf_play);
    int outscore = outcome_score(outcome);
    int score = shpscore + outscore;
    
    switch (outcome) {
        case RPSOutcome::Lose: {
            if (this->round_self == this->l_play) {
                this->elves[0]->play("hurt");
                this->elves[1]->play("lshoot");
            } else {
                this->elves[0]->play("rshoot");
                this->elves[1]->play("hurt");
            }

            this->outcome_desc->set_text_color(CRIMSON);
            this->outcome_desc->set_text(MatterAnchor::CC, outcome_fmt, shpscore, outscore);
        }; break;
        case RPSOutcome::Win: {
            if (this->round_self == this->l_play) {
                this->elves[0]->play("rshoot");
                this->elves[1]->play("hurt");
            } else {
                this->elves[0]->play("hurt");
                this->elves[1]->play("lshoot");
            }

            this->outcome_desc->set_text_color(FORESTGREEN);
            this->outcome_desc->set_text(MatterAnchor::CC, outcome_fmt, shpscore, outscore);
        }; break;
        case RPSOutcome::Draw: {
            this->elves[0]->play("rthrust");
            this->elves[1]->play("lthrust");

            this->outcome_desc->set_text_color(DARKORANGE);
            this->outcome_desc->set_text(MatterAnchor::CC, outcome_fmt, shpscore, outscore);
        }; break;
        default: ;
    }

    return score;
}

void WarGrey::AoC::RochamboPlane::switch_play_costume(Sprite* target, RPSShape play) {
    switch (play) {
        case RPSShape::Rock: target->switch_to_costume("rock"); break;
        case RPSShape::Paper: target->switch_to_costume("paper"); break;
        case RPSShape::Scissor: target->switch_to_costume("scissor"); break;
        default: ;
    }
}

/*************************************************************************************************/
void WarGrey::AoC::RochamboPlane::load_strategy(const std::string& pathname) {
    std::ifstream datin(pathname);
    
    this->strategy.clear();
    this->guessed_final_score = 0;
    this->designed_final_score = 0;
    
    if (datin) {
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
                
                    this->guessed_final_score += shape_score(sf_play) + outcome_score(round_outcome(op_play, sf_play));
                    this->designed_final_score += shape_score(smart_shape(op_play, outcome)) + outcome_score(outcome);
                }
            }
        }

        this->distance_score = flmax(float(this->guessed_final_score), float(this->designed_final_score), 1.0F);
        datin.close();
    }
}
