#include "calorie_counting.hpp"

#include "../big_bang/datum/vector.hpp"

using namespace WarGrey::STEM;
using namespace WarGrey::AoC;

/*************************************************************************************************/
static const char* population_desc = "精灵伙伴数量";
static const char* top1_desc = "个体食物最多";
static const char* cmp_alg_desc = "列表法";
static const char* srt_alg_desc = "排序法";
static const char* topn_unknown_fmt = "前%d热量[%s]";

static const float normal_scale_up = 1.6F;
static const float top_scale_up = 2.0F;
static const float elf_size = 45.0F;

static const int grid_column = 36;
static const int micro_pace = 3;

static const float moving_duration_when_exciting = 0.2F;
static const float moving_duration_when_calming_down = 1.5F;

/*************************************************************************************************/
void WarGrey::AoC::CalorieCountingPlane::construct(float width, float height) {
    this->load_calories(digimon_path("mee/01_cc", aoc_ext));
    
    if (this->top_count <= 0) {
        this->top_count = 3;
    } else if (this->top_count > this->elves.size()) {
        this->top_count = int(this->elves.size());
    }

    this->current_elf_idx = -1; // indicator of first run
    this->style = make_highlight_dimension_style(answer_fontsize, 7U, 0);
}

void WarGrey::AoC::CalorieCountingPlane::load(float width, float height) {
    this->backdrop = this->insert(new GridAtlas("backdrop/deck.png"));
    this->title = this->insert(new Labellet(aoc_font::title, BLACK, title_fmt, 1, this->name()));
    this->info_board = this->insert(new Labellet(aoc_font::large, GRAY, ""));
    this->population = this->insert(new Labellet(aoc_font::normal, GOLDENROD, unknown_fmt, population_desc));
    this->top1_total = this->insert(new Dimensionlet(this->style, "cal", top1_desc));
    this->topn_total = this->insert(new Dimensionlet(this->style, "cal", topn_unknown_fmt, this->top_count, cmp_alg_desc));
    this->sorted_total = this->insert(new Dimensionlet(this->style, "cal", topn_unknown_fmt, this->top_count, srt_alg_desc));
    
    this->snack = this->insert(new SpriteGridSheet("spritesheet/snacks.png", 3, 4, 2, 2));
    this->snack->scale(0.32F);
    this->set_matter_fps(this->snack, 2);

    this->agent = this->insert(new Linkmon());
    this->agent->scale(-1.0F, 1.0F);

    this->set_sentry_sprite(this->agent);
    
    for (int idx = 0; idx < this->top_count; idx ++) {
        this->dims.push_back(this->insert(new Labellet(aoc_font::math, SALMON, " ")));
    }
    
    if (this->elves.size() > 0) {
        int x0, xn, y0, yn;

        x0 = normal_fontsize;
        xn = fl2fxi(width) - x0;
        y0 = fl2fxi(height * 0.78F);
        yn = fl2fxi(height * 0.95F);

        for (auto elf : this->elves) {
            this->insert(elf, float(random_uniform(x0, xn)), float(random_uniform(y0, yn)), MatterAnchor::CC);
            this->set_matter_fps(elf, 8);
            elf->resize(elf_size);
            elf->play("lwalk");
        }
    }
}

void WarGrey::AoC::CalorieCountingPlane::reflow(float width, float height) {
    this->move_to(this->title, this->agent, MatterAnchor::RB, MatterAnchor::LB);
    this->move_to(this->snack, width, 0.0F, MatterAnchor::RT);
    this->move_to(this->info_board, this->snack, MatterAnchor::LC, MatterAnchor::RC);
    this->move_to(this->population, this->agent, MatterAnchor::LB, MatterAnchor::LT);
    this->move_to(this->top1_total, this->population, MatterAnchor::LB, MatterAnchor::LT, 0.0F, 1.0F);
    this->move_to(this->topn_total, this->top1_total, MatterAnchor::LB, MatterAnchor::LT, 0.0F, 1.0F);
    this->move_to(this->sorted_total, this->topn_total, MatterAnchor::LB, MatterAnchor::LT, 0.0F, 1.0F);
    
    this->backdrop->resize(width, height);
    this->move_to(this->backdrop, 0.0F, height, MatterAnchor::LB);
    
    if (this->elves.size() > 0) {
        float gxoff = float(normal_fontsize);
        float gyoff = height * 0.76F;

        this->create_grid(grid_column, gxoff, gyoff, width - gxoff - float(normal_fontsize));

        /* reflow top elves labels */ {
            int top_elf_col = 4;
            float xoff = width * 0.5F;
            float yoff = float(normal_fontsize) * 2.4F;
            float cwidth = (width - xoff) / float(top_elf_col);
            float cheight = (height * 0.64F - yoff) / 4.0F;

            for (int idx = 0; idx < this->dims.size(); idx ++) {
                int c = idx % top_elf_col;
                int r = idx / top_elf_col;

                this->move_to(this->dims[idx], (c + 0.4F) * cwidth + xoff, (r + 1.0F) * cheight + yoff, MatterAnchor::CB);
            }
        }
    }
}

void WarGrey::AoC::CalorieCountingPlane::update(uint32_t count, uint32_t interval, uint32_t uptime) {
    switch (this->status) {
        case CCStatus::CountOff: {
            if (this->current_elf_idx > 0) {
                this->calm_elf_down(this->elves[this->current_elf_idx - 1]);
            }
                
            if (this->current_elf_idx < this->elves.size()) {
                this->excite_elf(this->elves[this->current_elf_idx], normal_scale_up);
                this->move_elf_to_grid(this->elves[this->current_elf_idx]);
                this->info_board->set_text(std::to_string(this->elves[this->current_elf_idx]->calorie_total()), MatterAnchor::RC);
                this->population->set_text(puzzle_fmt, population_desc, ++ this->current_elf_idx);
                
                this->random_walk(this->current_elf_idx);
            } else {
                this->population->set_text(puzzle_fmt, population_desc, this->elves.size());
                this->on_task_done();
            }
        }; break;
        case CCStatus::FindMaximumCalorie: {
            if (this->current_elf_idx < this->elves.size()) {
                int self_cal = this->elves[this->current_elf_idx]->calorie_total();

                this->info_board->set_text(std::to_string(self_cal), MatterAnchor::RC);
                    
                if (self_cal > this->top_calorie) {
                    this->top_calorie = self_cal;
                    this->top1_total->set_value(float(self_cal));
                    this->excite_elf(this->elves[this->current_elf_idx], top_scale_up);
                    this->glide_to(moving_duration_when_exciting,
                        this->elves[this->current_elf_idx], this->top1_total,
                        MatterAnchor::RB, MatterAnchor::LB);
                    
                    if (this->prev_top_elf_id >= 0) {
                        this->calm_elf_down(this->elves[this->prev_top_elf_id]);
                    }

                    this->prev_top_elf_id = this->current_elf_idx;
                } else {
                    this->move_elf_to_grid(this->elves[this->current_elf_idx]);
                }

                this->current_elf_idx ++;
                this->random_walk(this->current_elf_idx);
            } else {
                if (this->prev_top_elf_id >= 0) {
                    this->calm_elf_down(this->elves[this->prev_top_elf_id]);
                }

                this->on_task_done();
            }
        }; break;
        case CCStatus::FindMaximumCalories: {
            if (this->current_elf_idx < this->elves.size()) {
                int maybe_replaced_cal;
                int self_cal = this->elves[this->current_elf_idx]->calorie_total();
                int replaced_idx = vector_replace_sorted_maximum(this->top_calories, self_cal, &maybe_replaced_cal);
                
                this->info_board->set_text(std::to_string(self_cal), MatterAnchor::RC);

                if (replaced_idx >= 0) {
                    int replaced_elf_idx = this->top_elf_indices[replaced_idx];

                    if (replaced_idx < this->top_count - 1) {
                        for (int idx = this->top_count - 1; idx > replaced_idx + 1; idx --) {
                            this->top_calories[idx] = this->top_calories[idx - 1];
                        }
                        this->top_calories[replaced_idx + 1] = maybe_replaced_cal; 
                    }

                    if (replaced_elf_idx >= 0) {
                        this->calm_elf_down(this->elves[replaced_elf_idx]);
                    }

                    this->top_elf_indices[replaced_idx] = this->current_elf_idx;
                    this->excite_elf(this->elves[this->current_elf_idx], top_scale_up);
                    this->topn_total->set_value(float(vector_sum(this->top_calories)));
                } else {
                    this->move_elf_to_grid(this->elves[this->current_elf_idx]);
                }

                this->reflow_top_elves();
                this->current_elf_idx ++;
                this->random_walk(this->current_elf_idx);
            } else {
                this->topn_total->set_value(float(vector_sum(this->top_calories)));
                this->calm_top_elves_down();
                this->on_task_done();
            }
        }; break;
        case CCStatus::FindMaximumCaloriesViaSorting: {
            if ((this->top_calories.size() < this->top_count) || (this->current_elf_idx > 0)) {
                if (this->current_elf_idx == 0) {
                    this->current_elf_idx = int(this->top_calories.size()) + 1;
                    this->top_elf_indices.push_back(this->current_elf_idx - 1);
                    this->top_calories.push_back(this->elves[this->current_elf_idx - 1]->calorie_total());
                } else if (this->current_elf_idx < this->elves.size()) {
                    int top_cal_in_this_round = this->top_calories.back();
                    int self_cal = this->elves[this->current_elf_idx]->calorie_total();

                    this->info_board->set_text(std::to_string(self_cal), MatterAnchor::RC);

                    if (top_cal_in_this_round < self_cal) {
                        int target_elf_idx = this->top_elf_indices.back();

                        this->swap_elves(this->current_elf_idx, target_elf_idx);
                        this->excite_elf(this->elves[target_elf_idx], top_scale_up);
                        this->calm_elf_down(this->elves[this->current_elf_idx]);
                        this->top_calories[this->top_calories.size() - 1] = self_cal;
                        this->sorted_total->set_value(float(vector_sum(this->top_calories)));
                    } else {
                        this->move_elf_to_grid(this->elves[this->current_elf_idx]);
                    }

                    this->reflow_top_elves();
                    this->current_elf_idx ++;
                    this->random_walk(this->current_elf_idx);
                } else {
                    this->current_elf_idx = 0;
                }
            } else {
                this->sorted_total->set_value(float(vector_sum(this->top_calories)));
                this->calm_top_elves_down();
                this->on_task_done();
            }
        }; break;
        default: {
            if (this->current_elf_idx < 0) {
                if (count % 4 == 0) {
                    this->random_walk(0);
                }
            }
        };
    }
}

void WarGrey::AoC::CalorieCountingPlane::after_select(IMatter* m, bool yes_or_no) {
    if (yes_or_no) {
        if (m == this->population) {
            this->current_elf_idx = 0;
            this->on_task_start(CCStatus::CountOff);
        } else if (m == this->top1_total) {
            this->current_elf_idx = 0;
            this->prev_top_elf_id = -1;
            this->top_calorie = 0;
            this->on_task_start(CCStatus::FindMaximumCalorie);
        } else if (m == this->topn_total) {
            this->current_elf_idx = 0;
            vector_reset(this->top_calories, this->top_count, 0);
            vector_reset(this->top_elf_indices, this->top_count, -1);
            this->on_task_start(CCStatus::FindMaximumCalories);
        } else if (m == this->sorted_total) {
            this->current_elf_idx = 0;
            this->top_calories.clear();
            this->top_elf_indices.clear();
            this->on_task_start(CCStatus::FindMaximumCaloriesViaSorting);
        } else {
            Elfmon* maybe_elf = dynamic_cast<Elfmon*>(m);

            if (maybe_elf != nullptr) {
                this->info_board->set_text(std::to_string(maybe_elf->calorie_total()), MatterAnchor::RC);
            }
        }

        this->no_selected();
    }
}

bool WarGrey::AoC::CalorieCountingPlane::can_select(IMatter* m) {
    return (this->status == CCStatus::TaskDone);
}

void WarGrey::AoC::CalorieCountingPlane::move_elf_to_grid(Elfmon* elf) {
    this->glide_to_grid(moving_duration_when_calming_down, elf, elf->id, MatterAnchor::CB);
}

void WarGrey::AoC::CalorieCountingPlane::reflow_top_elves() {
    for (int idx = 0; idx < this->top_elf_indices.size(); idx ++) {
        int top_elf_idx = this->top_elf_indices[idx];

        if (top_elf_idx >= 0) {
            this->dims[idx]->set_text("%d", this->elves[top_elf_idx]->calorie_total());
            this->glide_to(moving_duration_when_exciting,
                this->elves[top_elf_idx], this->dims[idx],
                MatterAnchor::CT, MatterAnchor::CB);
        } else {
            break;
        }
    }
}

void WarGrey::AoC::CalorieCountingPlane::calm_top_elves_down() {
    for (int idx = 0; idx < this->top_elf_indices.size(); idx ++) {
        int top_elf_idx = this->top_elf_indices[idx];

        if (top_elf_idx >= 0) {
            this->calm_elf_down(this->elves[top_elf_idx]);
            this->dims[idx]->set_text(" ");
        } else {
            break;
        }
    }
}

void WarGrey::AoC::CalorieCountingPlane::excite_elf(Elfmon* elf, float scale) {
    elf->play("dspellcast");
    elf->scale(scale);
}

void WarGrey::AoC::CalorieCountingPlane::calm_elf_down(Elfmon* elf) {
    elf->play("lwalk");
    elf->resize(elf_size);
    this->move_elf_to_grid(elf);
}

void WarGrey::AoC::CalorieCountingPlane::random_walk(int start_idx) {
    for (int idx = start_idx; idx < this->elves.size(); idx ++) {
        this->move(this->elves[idx], float(random_uniform(-micro_pace, micro_pace)), float(random_uniform(-micro_pace, micro_pace)));
    }
}

void WarGrey::AoC::CalorieCountingPlane::on_mission_start(float width, float height) {
    this->on_task_done();
    this->snack->play();
}

void WarGrey::AoC::CalorieCountingPlane::on_task_start(CCStatus status) {
    this->status = status;
    this->agent->play("Processing");
}

void WarGrey::AoC::CalorieCountingPlane::on_task_done() {
    this->status = CCStatus::TaskDone;
    this->info_board->set_text(MatterAnchor::RC, "");
    this->agent->stop(1);
}

void WarGrey::AoC::CalorieCountingPlane::swap_elves(int self_idx, int target_idx) {
    Elfmon* self = this->elves[self_idx];
    Elfmon* target = this->elves[target_idx];
    int self_id = self->id;

    self->id = target->id;
    target->id = self_id;

    this->elves[self_idx] = target;
    this->elves[target_idx] = self;
}

/*************************************************************************************************/
void WarGrey::AoC::CalorieCountingPlane::load_calories(const std::string& pathname) {
    std::ifstream datin(pathname);
    Elfmon* elf = nullptr;

    this->elves.clear();

    if (datin) {
        std::string line;
        
        while (std::getline(datin, line)) {
            if (line.size() == 0) {
                this->elves.push_back(elf);
                elf = nullptr;
            } else {
                if (elf == nullptr) {
                    elf = new Elfmon(int(this->elves.size()));
                }

                elf->food_calories.push_back(std::stoi(line));
            }
        }

        if (elf != nullptr) {
            this->elves.push_back(elf);
        }

        datin.close();
    }
}

/*************************************************************************************************/
WarGrey::AoC::Elfmon::Elfmon(int id) : ElfSheet(id), id(id) {}

int WarGrey::AoC::Elfmon::calorie_total() {
    return vector_sum(this->food_calories);
}
