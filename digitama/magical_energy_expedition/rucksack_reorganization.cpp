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
static const unsigned int DICT_SIZE = 53;
static char misplaced_dict1[DICT_SIZE];
static char misplaced_dict2[DICT_SIZE];

static inline void dict_zero(char dict[]) {
    memset(dict, 0, DICT_SIZE * sizeof(char));
}

static int item_priority(char ch) {
    int prior = 0;

    if ((ch >= 'a') && (ch <= 'z')) {
        prior = int(ch - 'a' + 1);
    } else if ((ch >= 'A') && (ch <= 'Z')) {
        prior = int(ch - 'A' + 27);
    }

    return prior;
}

static int misplaced_item_priority(const std::string& item_list) {
    const char* items = item_list.c_str();
    int midpos = item_list.size() / 2;
    int priority = 0;

    dict_zero(misplaced_dict1);
    dict_zero(misplaced_dict2);
        
    for (int idx = 0; idx < midpos; idx ++) {
        if (items[idx] == items[idx + midpos]) {
            priority = item_priority(items[idx]);
            break;
        } else {
            int prior1 = item_priority(items[idx]);
            int prior2 = item_priority(items[idx + midpos]);

            if (misplaced_dict2[prior1] != '\0') {
                priority = prior1;
                break;
            } else if (misplaced_dict1[prior2] != '\0') {
                priority = prior2;
                break;
            } else {
                misplaced_dict1[prior1] = items[idx];
                misplaced_dict2[prior2] = items[idx + midpos];
            }
        }
    }

    return priority;
}

static void feed_item_dict(char dict[], const char* items, size_t endpos) {
    dict_zero(dict);

    for (int idx = 0; idx < endpos; idx ++) {
        dict[item_priority(items[idx])] = items[idx];
    }
}

static void feed_shared_item_dict(char dict[], char dict0[], const char* items, size_t endpos) {
    dict_zero(dict);

    for (int idx = 0; idx < endpos; idx ++) {
        int prior = item_priority(items[idx]);

        if (dict0[prior] != '\0') {
            dict[prior] = items[idx];
        }
    }
}

static int find_last_shared_item_prior(char dict0[], const char* items, size_t endpos) {
    int last_shared_item_prior = 0;

    for (int idx = 0; idx < endpos; idx ++) {
        int prior = item_priority(items[idx]);

        if (dict0[prior] != '\0') {
            last_shared_item_prior = prior;
        }
    }

    return last_shared_item_prior;
}

/*************************************************************************************************/
WarGrey::AoC::RucksackReorganizationPlane::~RucksackReorganizationPlane() {
    for (int idx = 0; idx < this->group_size - 1; idx ++) {
        free(this->badge_dicts[idx]);
    }
    
    free(this->badge_dicts);
}

void WarGrey::AoC::RucksackReorganizationPlane::construct(float width, float height) {
    this->load_item_list(digimon_path("mee/03.rr.dat"));
    
    if (this->group_size <= 0) {
        this->group_size = 3;
    } else if (this->group_size > this->rucksacks.size()) {
        this->group_size = this->rucksacks.size();
    }

    this->badge_dicts = (char**)malloc((this->group_size - 1)* sizeof(char**));
    for (int idx = 0; idx < this->group_size - 1; idx ++) {
        this->badge_dicts[idx] = (char*)malloc(DICT_SIZE * sizeof(char));
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
        case RRStatus::FindMisplacedItems: {
            if (this->current_rucksack_idx < this->rucksacks.size()) {
                this->rucksacks[this->current_rucksack_idx]->switch_to_next_costume();
                this->display_items(this->rucksacks[this->current_rucksack_idx]);
                
                this->misplaced_item_priority_sum += misplaced_item_priority(this->rucksacks[this->current_rucksack_idx]->items);
                this->misplaced_sum->set_value(this->misplaced_item_priority_sum);

                this->current_rucksack_idx ++;
            } else {
                this->on_task_done();
            }
        }; break;
        case RRStatus::FindBadgeItems: {
            if (this->current_rucksack_idx < this->rucksacks.size()) {
                const char* items = this->rucksacks[this->current_rucksack_idx]->items.c_str();
                size_t endpos = this->rucksacks[this->current_rucksack_idx]->items.size();
                size_t gidx = this->current_rucksack_idx % this->group_size;
                
                if (gidx == 0) {
                    feed_item_dict(badge_dicts[gidx], items, endpos);
                } else if (gidx < this->group_size - 1) {
                    feed_shared_item_dict(badge_dicts[gidx], badge_dicts[gidx - 1], items, endpos);
                } else {
                    this->badge_item_priority_sum += find_last_shared_item_prior(badge_dicts[gidx - 1], items, endpos);
                    this->badge_sum->set_value(this->badge_item_priority_sum);
                }

                this->rucksacks[this->current_rucksack_idx]->switch_to_next_costume();
                this->display_items(this->rucksacks[this->current_rucksack_idx]);
                
                this->current_rucksack_idx ++;
            } else {
                this->on_task_done();
            }
        }; break;
        default: /* do nothing */;
    }
}

void WarGrey::AoC::RucksackReorganizationPlane::after_select(IMatter* m, bool yes_or_no) {
    if (yes_or_no) {
        if (m == this->misplaced_sum) {
            this->current_rucksack_idx = 0;
            this->misplaced_item_priority_sum = 0;
            this->on_task_start(RRStatus::FindMisplacedItems);
        } else if (m == this->badge_sum) {
            this->current_rucksack_idx = 0;
            this->badge_item_priority_sum = 0;
            this->on_task_start(RRStatus::FindBadgeItems);
        } else if (m == this->logo) {
            this->status = RRStatus::MissionDone;
        } else {
            Rucksack* self = dynamic_cast<Rucksack*>(m);

            if (self != nullptr) {
                this->display_items(self);
                self->play();
            }
        }
    } else {
        Rucksack* self = dynamic_cast<Rucksack*>(m);

        if (self != nullptr) {
            self->stop();
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
