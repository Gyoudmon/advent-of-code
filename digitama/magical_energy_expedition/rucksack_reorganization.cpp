#include "rucksack_reorganization.hpp"

#include "../aoc.hpp"

#include "../big_bang/datum/path.hpp"
#include "../big_bang/datum/flonum.hpp"

#include "../big_bang/physics/random.hpp"

#include <iostream>
#include <fstream>
#include <string>

using namespace WarGrey::STEM;
using namespace WarGrey::AoC;

/*************************************************************************************************/
static const char* rucksack_styles [] = { "yellow", "green", "khaki", "white", "gold" };

static const char* population_desc = "背包数量";
static const char* misplaced_desc = "错放物品优先级总和";
static const char* badge_desc = "队徽物品优先级总和";

static const float rucksack_size = 36.0F;
static const float rucksack_grid_ratio = 1.15F;

static inline const char* random_rucksack_style(int hint) {
    return rucksack_styles[random_uniform(0, sizeof(rucksack_styles) / sizeof(char*) - 1)];
}

static inline const char* random_rucksack_gender(int hint) {
    return (random_uniform(1, 100) % 2 == 0) ? "male" : "female";
}

/*************************************************************************************************/
void WarGrey::AoC::RucksackReorganizationPlane::construct(float width, float height) {
    this->load_item_list(digimon_path("mee/03.rr.dat"));
    
    if (this->group_size <= 0) {
        this->group_size = 3;
    } else if (this->group_size > this->rucksacks.size()) {
        this->group_size = this->rucksacks.size();
    }

    this->style = make_highlight_dimension_style(answer_fontsize, 5U, 0);
}

void WarGrey::AoC::RucksackReorganizationPlane::load(float width, float height) {
    this->logo = this->insert(new Sprite("logo.png"));
    this->title = this->insert(new Labellet(aoc_font::title, BLACK, title_fmt, 3, this->name()));
    this->population = this->insert(new Labellet(aoc_font::text, GOLDENROD, puzzle_fmt, population_desc, this->rucksacks.size()));
    this->info_board = this->insert(new Labellet(aoc_font::mono, GRAY, ""));
    this->misplaced_sum = this->insert(new Dimensionlet(this->style, "", misplaced_desc));
    this->badge_sum = this->insert(new Dimensionlet(this->style, "", badge_desc));
    
    if (this->rucksacks.size() > 0) {
        int x0, xn, y0, yn;

        x0 = fl2fxi(width * 0.32F);
        xn = fl2fxi(width) - text_fontsize;
        y0 = fl2fxi(height * 0.5F);
        yn = fl2fxi(height) - text_fontsize;

        for (auto rucksack : this->rucksacks) {
            this->insert(rucksack);
            rucksack->resize(rucksack_size);
        }
    }
}

void WarGrey::AoC::RucksackReorganizationPlane::reflow(float width, float height) {
    this->move_to(this->title, this->logo, MatterAnchor::RC, MatterAnchor::LC);
    this->move_to(this->population, this->logo, MatterAnchor::LB, MatterAnchor::LT);
    this->move_to(this->misplaced_sum, this->population, MatterAnchor::LB, MatterAnchor::LT, 0.0F, 1.0F);
    this->move_to(this->badge_sum, this->misplaced_sum, MatterAnchor::LB, MatterAnchor::LT, 0.0F, 1.0F);
    this->move_to(this->info_board, this->badge_sum, MatterAnchor::LB, MatterAnchor::LT);
    
    if (this->rucksacks.size() > 0) {
        float elf_width, elf_height;
        float lbl_width, lbl_height, title_height;
        
        this->rucksacks[0]->feed_extent(0.0F, 0.0F, &elf_width, &elf_height);
        this->title->feed_extent(0.0F, 0.0F, nullptr, &title_height);
        this->badge_sum->feed_extent(0.0F, 0.0F, &lbl_width, &lbl_height);
        
        this->cell_width = elf_width * rucksack_grid_ratio;
        this->cell_height = elf_height * rucksack_grid_ratio;
        this->grid_xoff = lbl_width + float(text_fontsize);
        this->grid_yoff = title_height + lbl_height * 0.0F;

        this->col = int(flfloor((width - this->grid_xoff) / this->cell_width)) - 1;
        this->row = int(flfloor((height - this->grid_yoff) / this->cell_height));

        for (int idx = 0; idx < this->rucksacks.size(); idx ++) {
            this->move_rucksack_to_grid(this->rucksacks[idx]);
        }
    }
}

void WarGrey::AoC::RucksackReorganizationPlane::on_enter(IPlane* from) {
    this->on_task_done();
}

void WarGrey::AoC::RucksackReorganizationPlane::update(uint32_t count, uint32_t interval, uint32_t uptime) {
    switch (this->status) {
        case RRStatus::CountOff: {
            if (this->current_rucksack_idx < this->rucksacks.size()) {
                this->rucksacks[this->current_rucksack_idx]->switch_to_next_costume();
                this->display_items(this->rucksacks[this->current_rucksack_idx]);
                this->population->set_text(puzzle_fmt, population_desc, ++ this->current_rucksack_idx);
            } else {
                this->population->set_text(puzzle_fmt, population_desc, this->rucksacks.size());
                this->on_task_done();
            }
        }; break;
        default: /* do nothing */;
    }
}

void WarGrey::AoC::RucksackReorganizationPlane::after_select(IMatter* m, bool yes_or_no) {
    if (yes_or_no) {
        if (m == this->population) {
            this->current_rucksack_idx = 0;
            this->on_task_start(RRStatus::CountOff);
        } else if (m == this->logo) {
            this->status = RRStatus::MissionDone;
        } else {
            Rucksack* self = dynamic_cast<Rucksack*>(m);

            if (self != nullptr) {
                self->switch_to_prev_costume();
                this->display_items(self);
            }
        }
    }
}

bool WarGrey::AoC::RucksackReorganizationPlane::can_select(IMatter* m) {
    return (this->status == RRStatus::TaskDone);
}

void WarGrey::AoC::RucksackReorganizationPlane::move_rucksack_to_grid(Rucksack* rs) {
    if (rs->id > 0) {
        int idx = rs->id - 1;
        int c = idx % this->col;
        int r = idx / this->col;

        this->move_to(rs,
            (c + 0.5F) * this->cell_width + this->grid_xoff,
            (r + 1.0F) * this->cell_height + this->grid_yoff,
            MatterAnchor::CB);
    }
}

void WarGrey::AoC::RucksackReorganizationPlane::on_task_start(RRStatus status) {
    this->status = status;
}

void WarGrey::AoC::RucksackReorganizationPlane::on_task_done() {
    this->status = RRStatus::TaskDone;
    this->info_board->set_text("");
}

void WarGrey::AoC::RucksackReorganizationPlane::display_items(Rucksack* self) {
    size_t size = self->items.size();
    std::string items = self->items.substr(0, size / 2);
                
    items.push_back('\n');
    items.append(self->items.substr(size / 2));

    this->info_board->set_text(items);
}

/*************************************************************************************************/
void WarGrey::AoC::RucksackReorganizationPlane::load_item_list(const std::string& pathname) {
    std::ifstream datin(pathname);
    
    this->rucksacks.clear();

    if (datin.is_open()) {    
        std::string line;

        while (std::getline(datin, line)) {
            this->rucksacks.push_back(new Rucksack(line, this->rucksacks.size() + 1));
        }

        datin.close();
    }
}

/*************************************************************************************************/
WarGrey::AoC::Rucksack::Rucksack(const std::string& items, int id)
    : Sprite("sprite/rucksack/%s/%s", random_rucksack_style(id), random_rucksack_gender(id))
    , items(items), id(id) {}
