#include "calorie_counting.hpp"

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
static const char* title_desc = "C++ 降临节: [任务1] 计点卡路里";
static const char* population_desc = "精灵伙伴数量";
static const char* top1_desc = "个体食物最富";
static const char* cmp_alg_desc = "比较法";
static const char* srt_alg_desc = "排序法";

static const char* unknown_fmt = "%s: (未知)";
static const char* topn_unknown_fmt = "前%d热量[%s]: (未知)";
static const char* topn_fmt = "前%d热量[%s]: %d";
static const char* puzzle_fmt = "%s: %d";

static const int title_fontsize = 50;
static const int text_fontsize = 36;
static const int dim_fontsize = 24;
static const int micro_pace = 3;

static const float normal_scale_up = 1.6F;
static const float top_scale_up = 2.0F;

static const int elf_type_count = 2;

static inline const char* elf_name(int hint) {
    switch ((hint <= elf_type_count) ? hint : random_uniform(0, elf_type_count)) {
    case 1: return "fairy"; break;
    case 2: return "goblin"; break;
    case 3: return "mermaid"; break;
    default: return "elf";
    }
}

/*************************************************************************************************/
namespace {
    enum class CmdlineOps { TopCount, _ }; 
}

/*************************************************************************************************/
void WarGrey::AoC::AoCWorld::construct(int argc, char* argv[]) {
    CmdlineOps opt = CmdlineOps::_;
    std::string datin;

    for (int idx = 1; idx < argc; idx ++) {
        switch (opt) {
            case CmdlineOps::TopCount: {
                this->top_count = std::stoi(argv[idx]);
                opt = CmdlineOps::_;
             }; break;
            default: {
                if (strncmp("-n", argv[idx], 2) == 0) {
                    opt = CmdlineOps::TopCount;
                } else {
                    datin = std::string(argv[idx]);
                }
            }
        }
    }
    
    if (datin.size() > 0) {
        this->load_calories(datin);
    } else {
        this->load_calories(digimon_path("2022/01.cc.dat"));
    }

    if (this->top_count <= 0) {
        this->top_count = 3;
    } else if (this->top_count > this->elves.size()) {
        this->top_count = this->elves.size();
    }

    this->title_font = game_create_font(font_basename(game_font::unicode), title_fontsize);
    this->text_font = game_create_font(font_basename(game_font::unicode), text_fontsize);
    this->dim_font = game_create_font(font_basename(game_font::math), dim_fontsize);
}

WarGrey::AoC::AoCWorld::~AoCWorld() {
    game_font_destroy(this->text_font);
    game_font_destroy(this->title_font);
    game_font_destroy(this->dim_font);
}

void WarGrey::AoC::AoCWorld::load(float width, float height) {
    this->title = this->insert(new Labellet(this->title_font, BLACK, title_desc));
    this->info_board = this->insert(new Labellet(this->title_font, GRAY, " "));
    this->population = this->insert(new Labellet(this->text_font, GOLDENROD, unknown_fmt, population_desc));
    this->top1_total = this->insert(new Labellet(this->text_font, ROYALBLUE, unknown_fmt, top1_desc));
    this->topn_total = this->insert(new Labellet(this->text_font, FORESTGREEN, topn_unknown_fmt, this->top_count, cmp_alg_desc));
    this->sorted_total = this->insert(new Labellet(this->text_font, SEAGREEN, topn_unknown_fmt, this->top_count, srt_alg_desc));

    for (int idx = 0; idx < this->top_count; idx ++) {
        this->dims.push_back(this->insert(new Labellet(this->dim_font, SALMON, " ")));
    }
    
    if (this->elves.size() > 0) {
        int x0, xn, y0, yn;

        x0 = fl2fxi(width * 0.32F);
        xn = fl2fxi(width) - text_fontsize;
        y0 = fl2fxi(height * 0.5F);
        yn = fl2fxi(height) - text_fontsize;

        this->begin_update_sequence();
        for (auto elf : this->elves) {
            this->insert(elf);
            elf->switch_to_custome("normal");
            elf->resize(0.19F);

            this->move_to(elf, float(random_uniform(x0, xn)), float(random_uniform(y0, yn)), MatterAnchor::CC);
        }
        this->end_update_sequence();
    }

    this->on_task_done();
}

void WarGrey::AoC::AoCWorld::reflow(float width, float height) {
    this->move_to(this->info_board, width, 0.0F, MatterAnchor::RT);
    this->move_to(this->population, this->title, MatterAnchor::LB, MatterAnchor::LT);
    this->move_to(this->top1_total, this->population, MatterAnchor::LB, MatterAnchor::LT);
    this->move_to(this->topn_total, this->top1_total, MatterAnchor::LB, MatterAnchor::LT);
    this->move_to(this->sorted_total, this->topn_total, MatterAnchor::LB, MatterAnchor::LT);
    
    if (this->elves.size() > 0) {
        float elf_width, elf_height;
        float lbl_width, lbl_height, title_height;
        
        this->elves[0]->feed_extent(0.0F, 0.0F, &elf_width, &elf_height);
        this->title->feed_extent(0.0F, 0.0F, nullptr, &title_height);
        this->topn_total->feed_extent(0.0F, 0.0F, &lbl_width, &lbl_height);
        
        this->cell_width = elf_width * 1.618F;
        this->cell_height = elf_height * 1.618F;
        this->grid_xoff = lbl_width + float(text_fontsize);
        this->grid_yoff = title_height + lbl_height * 0.0F;

        this->col = int(flfloor((width - this->grid_xoff) / this->cell_width)) - 1;
        this->row = int(flfloor((height - this->grid_yoff) / this->cell_height));

        /* reflow top elves labels */ {
            float yoff, cwidth, cheight;
            int top_elf_col = 3;
        
            this->feed_matter_location(this->sorted_total, &lbl_width, &yoff, MatterAnchor::RB);
            cwidth = lbl_width / float(top_elf_col);
            cheight = cwidth + float(dim_fontsize);

            for (int idx = 0; idx < this->dims.size(); idx ++) {
                int c = idx % top_elf_col;
                int r = idx / top_elf_col;

                this->move_to(this->dims[idx], (c + 0.4F) * cwidth, (r + 1.0F) * cheight + yoff, MatterAnchor::CB);
            }
        }
    }
}

void WarGrey::AoC::AoCWorld::update(uint32_t interval, uint32_t count, uint32_t uptime) {
    switch (this->status) {
        case AoCStatus::CountOff: {
            if (this->current_elf_idx > 0) {
                this->calm_elf_down(this->elves[this->current_elf_idx - 1], normal_scale_up);
            }
                
            if (this->current_elf_idx < this->elves.size()) {
                this->excite_elf(this->elves[this->current_elf_idx], normal_scale_up);
                this->move_elf_to_grid(this->elves[this->current_elf_idx]);
                this->info_board->set_text(std::to_string(this->elves[this->current_elf_idx]->calorie_total()), MatterAnchor::RT);
                this->population->set_text(puzzle_fmt, population_desc, ++ this->current_elf_idx);
                
                this->random_walk(this->current_elf_idx);
            } else {
                this->population->set_text(puzzle_fmt, population_desc, this->elves.size());
                this->on_task_done();
            }
        }; break;
        case AoCStatus::FindMaximumCalorie: {
            if (this->current_elf_idx < this->elves.size()) {
                int self_cal = this->elves[this->current_elf_idx]->calorie_total();

                this->info_board->set_text(std::to_string(self_cal), MatterAnchor::RT);
                    
                if (self_cal > this->top_calorie) {
                    this->top_calorie = self_cal;
                    this->top1_total->set_text(puzzle_fmt, top1_desc, self_cal);
                    this->excite_elf(this->elves[this->current_elf_idx], top_scale_up);
                    this->move_to(this->elves[this->current_elf_idx],
                        this->top1_total, MatterAnchor::RB,
                        MatterAnchor::LB, float(dim_fontsize) * 0.618F);
                    
                    if (this->prev_top_elf_id >= 0) {
                        this->calm_elf_down(this->elves[this->prev_top_elf_id], top_scale_up);
                    }

                    this->prev_top_elf_id = this->current_elf_idx;
                } else {
                    this->move_elf_to_grid(this->elves[this->current_elf_idx]);

                    if (this->prev_top_elf_id >= 0) {
                        this->elves[this->prev_top_elf_id]->switch_to_next_custome();
                    }
                }

                this->current_elf_idx ++;
                this->random_walk(this->current_elf_idx);
            } else {
                if (this->prev_top_elf_id >= 0) {
                    this->calm_elf_down(this->elves[this->prev_top_elf_id], top_scale_up);
                }

                this->on_task_done();
            }
        }; break;
        case AoCStatus::FindMaximumCalories: {
            if (this->current_elf_idx < this->elves.size()) {
                int self_cal = this->elves[this->current_elf_idx]->calorie_total();
                int replaced_idx = vector_replace_sorted_maximum(this->top_calories, self_cal);
                
                this->info_board->set_text(std::to_string(self_cal), MatterAnchor::RT);

                if (replaced_idx >= 0) {
                    int replaced_elf_idx = this->top_elf_indices[replaced_idx];

                    if (replaced_elf_idx >= 0) {
                        this->calm_elf_down(this->elves[replaced_elf_idx], top_scale_up);
                    }

                    this->top_elf_indices[replaced_idx] = this->current_elf_idx;
                    this->excite_elf(this->elves[this->current_elf_idx], top_scale_up);
                    this->topn_total->set_text(topn_fmt, this->top_count, cmp_alg_desc, vector_sum(this->top_calories));
                } else {
                    this->move_elf_to_grid(this->elves[this->current_elf_idx]);
                }

                this->reflow_top_elves();
                this->current_elf_idx ++;
                this->random_walk(this->current_elf_idx);
            } else {
                this->topn_total->set_text(topn_fmt, this->top_count, cmp_alg_desc, vector_sum(this->top_calories));
                this->calm_top_elves_down();
                this->on_task_done();
            }
        }; break;
        case AoCStatus::FindMaximumCaloriesViaSorting: {
            if ((this->top_calories.size() < this->top_count) || (this->current_elf_idx > 0)) {
                if (this->current_elf_idx == 0) {
                    this->current_elf_idx = this->top_calories.size() + 1;
                    this->top_elf_indices.push_back(this->current_elf_idx - 1);
                    this->top_calories.push_back(this->elves[this->current_elf_idx - 1]->calorie_total());
                } else if (this->current_elf_idx < this->elves.size()) {
                    int top_cal_in_this_round = this->top_calories.back();
                    int self_cal = this->elves[this->current_elf_idx]->calorie_total();

                    this->info_board->set_text(std::to_string(self_cal), MatterAnchor::RT);

                    if (top_cal_in_this_round < self_cal) {
                        int target_elf_idx = this->top_elf_indices.back();

                        this->swap_elves(this->current_elf_idx, target_elf_idx);
                        this->excite_elf(this->elves[target_elf_idx], top_scale_up);
                        this->calm_elf_down(this->elves[this->current_elf_idx], top_scale_up);
                        this->sorted_total->set_text(topn_fmt, this->top_count, srt_alg_desc, vector_sum(this->top_calories));
                        this->top_calories[this->top_calories.size() - 1] = self_cal;
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
                this->sorted_total->set_text(topn_fmt, this->top_count, srt_alg_desc, vector_sum(this->top_calories));
                this->calm_top_elves_down();
                this->on_task_done();
            }
        }; break;
        default: /* do nothing */;
    }
}

void WarGrey::AoC::AoCWorld::after_select(IMatter* m, bool yes_or_no) {
    if (yes_or_no) {
        if (m == this->population) {
            this->current_elf_idx = 0;
            this->on_task_start(AoCStatus::CountOff);
        } else if (m == this->top1_total) {
            this->current_elf_idx = 0;
            this->prev_top_elf_id = -1;
            this->top_calorie = 0;
            this->on_task_start(AoCStatus::FindMaximumCalorie);
        } else if (m == this->topn_total) {
            this->current_elf_idx = 0;
            vector_reset(this->top_calories, this->top_count, 0);
            vector_reset(this->top_elf_indices, this->top_count, -1);
            this->on_task_start(AoCStatus::FindMaximumCalories);
        } else if (m == this->sorted_total) {
            this->current_elf_idx = 0;
            this->top_calories.clear();
            this->top_elf_indices.clear();
            this->on_task_start(AoCStatus::FindMaximumCaloriesViaSorting);
        } else {
            Elfmon* maybe_elf = dynamic_cast<Elfmon*>(m);

            if (maybe_elf != nullptr) {
                this->info_board->set_text(std::to_string(maybe_elf->calorie_total()), MatterAnchor::RT);
            }
        }
    }
}

bool WarGrey::AoC::AoCWorld::can_select(IMatter* m) {
    return (this->status == AoCStatus::TaskDone);
}

void WarGrey::AoC::AoCWorld::move_elf_to_grid(Elfmon* elf) {
    if (elf->id > 0) {
        int idx = elf->id - 1;
        int c = idx % this->col;
        int r = idx / this->col;

        this->move_to(elf,
            (c + 0.5F) * this->cell_width + this->grid_xoff,
            (r + 1.0F) * this->cell_height + this->grid_yoff,
            MatterAnchor::CB);
    }
}

void WarGrey::AoC::AoCWorld::reflow_top_elves() {
    for (int idx = 0; idx < this->top_elf_indices.size(); idx ++) {
        int top_elf_idx = this->top_elf_indices[idx];

        if (top_elf_idx >= 0) {
            this->elves[top_elf_idx]->switch_to_next_custome();
            this->move_to(this->elves[top_elf_idx], this->dims[idx], MatterAnchor::CT, MatterAnchor::CB);
            this->dims[idx]->set_text("%d", this->elves[top_elf_idx]->calorie_total());
        } else {
            break;
        }
    }
}

void WarGrey::AoC::AoCWorld::calm_top_elves_down() {
    for (int idx = 0; idx < this->top_elf_indices.size(); idx ++) {
        int top_elf_idx = this->top_elf_indices[idx];

        if (top_elf_idx >= 0) {
            this->calm_elf_down(this->elves[top_elf_idx], top_scale_up);
            this->dims[idx]->set_text(" ");
        } else {
            break;
        }
    }
}

void WarGrey::AoC::AoCWorld::excite_elf(Elfmon* elf, float scale) {
    elf->switch_to_custome("greeting");
    elf->resize(scale);
}

void WarGrey::AoC::AoCWorld::calm_elf_down(Elfmon* elf, float scale) {
    elf->switch_to_custome("normal");
    elf->resize(1.0F / scale);
    this->move_elf_to_grid(elf);
}

void WarGrey::AoC::AoCWorld::random_walk(int start_idx) {
    for (int idx = start_idx; idx < this->elves.size(); idx ++) {
        this->move(this->elves[idx], random_uniform(-micro_pace, micro_pace), random_uniform(-micro_pace, micro_pace));
    }
}

void WarGrey::AoC::AoCWorld::on_task_start(AoCStatus status) {
    this->status = status;
}

void WarGrey::AoC::AoCWorld::on_task_done() {
    this->status = AoCStatus::TaskDone;
    this->info_board->set_text(MatterAnchor::RT, " ");
}

void WarGrey::AoC::AoCWorld::swap_elves(int self_idx, int target_idx) {
    Elfmon* self = this->elves[self_idx];
    Elfmon* target = this->elves[target_idx];
    int temp_id = self->id;

    self->id = target->id;
    target->id = temp_id;

    this->elves[self_idx] = target;
    this->elves[target_idx] = self;
}

/*************************************************************************************************/
void WarGrey::AoC::AoCWorld::load_calories(const std::string& pathname) {
    std::ifstream datin(pathname);
    Elfmon* elf = nullptr;

    this->elves.clear();

    if (datin.is_open()) {    
        std::string line;

        while (std::getline(datin, line)) {
            if (line.size() == 0) {
                this->elves.push_back(elf);
                elf = nullptr;
            } else {
                if (elf == nullptr) {
                    int idx = this->elves.size();

                    elf = new Elfmon(elf_name(idx), idx + 1);
                    elf->enable_resizing(true, MatterAnchor::CB);
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
WarGrey::AoC::Elfmon::Elfmon(const char* dirname, int id)
    : Sprite(digimon_path(dirname, "", "stone/sprite")), id(id) {}

int WarGrey::AoC::Elfmon::calorie_total() {
    return vector_sum(this->food_calories);
}
