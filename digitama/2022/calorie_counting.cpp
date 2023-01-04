#include "calorie_counting.hpp"

#include "../big_bang/datum/path.hpp"
#include "../big_bang/datum/flonum.hpp"
#include "../big_bang/physics/random.hpp"

#include <iostream>
#include <fstream>
#include <string>

using namespace WarGrey::STEM;
using namespace WarGrey::AoC;

/*************************************************************************************************/
static const char* population_desc = "精灵数量";
static const char* puzzle1_desc = "最多卡路里";
static const char* puzzle2_desc = "前三总卡路里";

static const char* unknown_fmt = "%s: (未知)";
static const char* puzzling_fmt = "%s: %d[%s]";
static const char* puzzled_fmt = "%s: %d";

static float scale_up = 1.5F;
static float scale_back = 0.75F;

static inline const char* elf_name(int hint) {
    switch ((hint < 3) ? hint : random_uniform(0, 3)) {
    case 1: return "fairy"; break;
    case 2: return "goblin"; break;
    case 3: return "mermaid"; break;
    default: return "elf";
    }
}

/*************************************************************************************************/
// 实现 AoCWorld::construct 方法，加载输入数据
void WarGrey::AoC::AoCWorld::construct(int argc, char* argv[]) {
    if (argc > 1) {
        this->load_calories(std::string(argv[1]));
    } else {
        this->load_calories(digimon_path("2022/calorie.counting.dat"));
    }

    this->big_font = game_create_font(font_basename(game_font::unicode), 42);
    this->status = WorldStatus::Wait;
    this->set_window_size(1200, 1000);
}

// 实现 AoCWorld::load 方法，加载精灵
void WarGrey::AoC::AoCWorld::load(float width, float height) {
    this->population = this->insert(new Labellet(this->big_font, GOLDENROD, unknown_fmt, population_desc));
    this->p1_answer = this->insert(new Labellet(this->big_font, ROYALBLUE, unknown_fmt, puzzle1_desc));
    this->p2_answer = this->insert(new Labellet(this->big_font, CRIMSON, unknown_fmt, puzzle2_desc));
    
    if (this->elves.size() > 0) {
        this->begin_update_sequence();
        for (auto elf : this->elves) {
            this->insert(elf);
            elf->switch_to_custome("normal");
            elf->resize(0.20F);
        }
        this->end_update_sequence();
    }
}

// 实现 AoCWorld::reflow 方法，调整精灵位置
void WarGrey::AoC::AoCWorld::reflow(float width, float height) {
    this->move_to(this->p1_answer, this->population, MatterAnchor::LB, MatterAnchor::LT);
    this->move_to(this->p2_answer, this->p1_answer, MatterAnchor::LB, MatterAnchor::LT);
    
    if (this->elves.size() > 0) {
        float elf_width, elf_height;
        float lbl_width, lbl_height;
        
        this->elves[0]->feed_extent(0.0F, 0.0F, &elf_width, &elf_height);
        this->p2_answer->feed_extent(0.0F, 0.0F, &lbl_width, &lbl_height);
        
        this->cell_width = elf_width * 1.618F;
        this->cell_height = elf_height * 1.618F;
        this->grid_xoff = lbl_width * 1.2F;
        this->grid_yoff = lbl_height * 0.0F;

        this->col = int(flfloor((width - this->grid_xoff) / this->cell_width)) - 1;
        this->row = int(flfloor((height - this->grid_yoff) / this->cell_height));
        
        this->begin_update_sequence();
        for (auto elf : this->elves) {
            this->move_elf_to_grid(elf);
        }
        this->end_update_sequence();
    }
}

// 实现 AoCWorld::update 方法
void WarGrey::AoC::AoCWorld::update(uint32_t interval, uint32_t count, uint32_t uptime) {
    switch (this->status) {
        case WorldStatus::CountOff: {
            if (this->current_elf_idx > 0) {
                this->calm_elf_down(this->elves[this->current_elf_idx - 1]);
            }
                
            if (this->current_elf_idx < this->elves.size()) {
                this->excite_elf(this->elves[this->current_elf_idx]);
                this->population->set_text(puzzling_fmt, population_desc, ++ this->current_elf_idx, "报数中");
            } else {
                this->population->set_text(puzzled_fmt, population_desc, ++ this->current_elf_idx);
                this->status = WorldStatus::Wait;
            }
        }; break;
        case WorldStatus::FindMaximumCalorie: {
            if (this->current_elf_idx < this->elves.size()) {
                int self_cal = this->elves[this->current_elf_idx]->calorie_total();

                if (self_cal > this->max_cal) {
                    this->max_cal = self_cal;
                    this->p1_answer->set_text(puzzling_fmt, puzzle1_desc, this->max_cal, "冒泡中");
                    this->excite_elf(this->elves[this->current_elf_idx]);
                    
                    if (this->last_elf_idx >= 0) {
                        this->calm_elf_down(this->elves[this->last_elf_idx]);
                    }

                    this->last_elf_idx = this->current_elf_idx;
                }

                this->current_elf_idx ++;
            } else {
                if (this->last_elf_idx >= 0) {
                    this->calm_elf_down(this->elves[this->last_elf_idx]);
                    this->last_elf_idx = -1;
                }

                this->p1_answer->set_text(puzzled_fmt, puzzle1_desc, this->max_cal);
                this->status = WorldStatus::Wait;
            }
        }; break;
        default: /* do nothing */;
    }
}

bool WarGrey::AoC::AoCWorld::can_select(IMatter* m) {
    return (this->status == WorldStatus::Wait);
}

void WarGrey::AoC::AoCWorld::after_select(IMatter* m, bool yes_or_no) {
    if (yes_or_no) {
        if (m == this->population) {
            this->status = WorldStatus::CountOff;
            this->current_elf_idx = 0;
        } else if (m == this->p1_answer) {
            this->status = WorldStatus::FindMaximumCalorie;
            this->current_elf_idx = 0;
            this->last_elf_idx = -1;
            this->max_cal = 0;
        }
    }
}

void WarGrey::AoC::AoCWorld::move_elf_to_grid(Elfmon* elf) {
    int c = elf->idx % this->col;
    int r = elf->idx / this->col;

    this->move_to(elf,
        (c + 0.5F) * this->cell_width + this->grid_xoff,
        (r + 1.0F) * this->cell_height + this->grid_yoff,
        MatterAnchor::CB);
}

void WarGrey::AoC::AoCWorld::excite_elf(Elfmon* elf) {
    elf->switch_to_custome("greeting");
    elf->resize(scale_up);
}

void WarGrey::AoC::AoCWorld::calm_elf_down(Elfmon* elf) {
    elf->switch_to_custome("normal");
    elf->resize(scale_back);
}

/*************************************************************************************************/
// 实现 AoCWorld::load_calories 方法，加载输入数据
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

                    elf = new Elfmon(elf_name(idx), idx);
                    elf->enable_resizing(true, MatterAnchor::CB);
                }

                elf->calories.push_back(std::stoi(line));
            }
        }

        if (elf != nullptr) {
            this->elves.push_back(elf);
        }

        datin.close();
    }
}

// 实现 AoCWorld::find_maximum_calories 方法，计算携带最多卡路里的 n 个精灵所带卡路里的总和
int WarGrey::AoC::AoCWorld::find_maximum_calories(int n) {
    std::vector<int> calories(n);
    int total = 0;

    for (int eidx = 0; eidx < this->elves.size(); eidx ++) {
        int cal = this->elves[eidx]->calorie_total();

        for (int cidx = 0; cidx < n; cidx ++) {
            if (cal > calories[cidx]) {
                calories[cidx] = cal;
                break;
            }
        }
    }

    for (int idx = 0; idx < n; idx ++) {
        total += calories[idx];
    }

    return total;
}

/*************************************************************************************************/
WarGrey::AoC::Elfmon::Elfmon(const char* dirname, int idx)
    : Sprite(digimon_path(dirname, "", "stone/sprite")), idx(idx) {}

int WarGrey::AoC::Elfmon::calorie_total() {
    int total = 0;

    for (auto cal : this->calories) {
        total += cal;        
    }

    return total;  
}
