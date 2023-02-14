#include "rucksack_reorganization.hpp"

#include "../big_bang/graphics/text.hpp"
#include "../aoc.hpp"

using namespace WarGrey::STEM;
using namespace WarGrey::AoC;

/*************************************************************************************************/
static const char* rucksack_styles [] = { "yellow", "green", "khaki", "white", "gold" };

static const char* population_desc = "背包数量";
static const char* misplaced_desc = "错放物品优先级总和";
static const char* badge_desc = "队徽物品优先级总和";

static const float rucksack_size = 36.0F;
static const float rucksack_item_size = 32.0F;
static const float hash_scale = 1.5F;
static const int grid_column = 30;

static inline const char* random_rucksack_style(size_t hint) {
    return rucksack_styles[random_uniform(0, int(sizeof(rucksack_styles) / sizeof(char*) - 1))];
}

static inline const char* random_rucksack_gender(size_t hint) {
    return (random_uniform(1, 100) % 2 == 0) ? "male" : "female";
}

/*************************************************************************************************/
template<typename T>
static inline void dict_zero(T dict[]) {
    memset(dict, 0, DICT_SIZE * sizeof(T));
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

static inline int atlas_vertical_index(int prior, int row, int col) {
    int r = prior % row;
    int c = prior / row;

    return r * col + c;
}

/*************************************************************************************************/
static int misplaced_item_priority(const std::string& item_list, char misplaced_dict1[], char misplaced_dict2[]) {
    const char* items = item_list.c_str();
    int midpos = int(item_list.size()) / 2;
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

static int badge_item_priority(char* badge_dicts[], const char* items[], size_t sizes[], int n) {
    int last_shared_item_prior = 0;
    int midx = n - 1;

    if (midx > 1) {
        feed_item_dict(badge_dicts[0], items[0], sizes[0]);

        for (int badge_idx = 1; badge_idx < midx; badge_idx ++) {
            feed_shared_item_dict(badge_dicts[badge_idx], badge_dicts[badge_idx - 1], items[badge_idx], sizes[badge_idx]);
        }

        last_shared_item_prior = find_last_shared_item_prior(badge_dicts[midx - 1], items[midx], sizes[midx]);
    }

    return last_shared_item_prior;
}

/*************************************************************************************************/
WarGrey::AoC::RucksackReorganizationPlane::~RucksackReorganizationPlane() {
    for (int idx = 0; idx < sizeof(this->badge_dicts) / sizeof(char*); idx ++) {
        delete [] this->badge_dicts[idx];
    }
}

void WarGrey::AoC::RucksackReorganizationPlane::construct(float width, float height) {
    this->load_item_list(digimon_path("mee/03_rr", aoc_ext));

    for (int idx = 0; idx < sizeof(this->badge_dicts) / sizeof(char*); idx ++) {
        // the `badge_dicts` is an array of 1-dim array, instead of a 2-dim array
        // using a 2-dim array might cause the damn 'segfault'
        this->badge_dicts[idx] = new char[DICT_SIZE];
    }

    this->style = make_highlight_dimension_style(answer_fontsize, 5U, 0);
}

void WarGrey::AoC::RucksackReorganizationPlane::load(float width, float height) {
    this->backdrop = this->insert(new GridAtlas("backdrop/tent.png"));
    this->title = this->insert(new Labellet(aoc_font::title, BLACK, title_fmt, 3, this->name()));
    this->population = this->insert(new Labellet(aoc_font::normal, GOLDENROD, puzzle_fmt, population_desc, this->rucksacks.size()));
    this->info_board = this->insert(new Labellet(aoc_font::mono, GRAY, ""));
    this->misplaced_sum = this->insert(new Dimensionlet(this->style, "", misplaced_desc));
    this->badge_sum = this->insert(new Dimensionlet(this->style, "", badge_desc));
    this->backpack = this->insert(new Backpack());
    this->hashtable = this->insert(new PackHash());

    this->agent = this->insert(new AgentLink());
    this->agent->scale(-1.0F, 1.0F);
    
    for (auto rucksack : this->rucksacks) {
        this->insert(rucksack);
        rucksack->resize(rucksack_size);
    }
}

void WarGrey::AoC::RucksackReorganizationPlane::reflow(float width, float height) {
    this->move_to(this->title, this->agent, MatterAnchor::RB, MatterAnchor::LB);
    this->move_to(this->population, this->agent, MatterAnchor::LB, MatterAnchor::LT);
    this->move_to(this->misplaced_sum, this->population, MatterAnchor::LB, MatterAnchor::LT, 0.0F, 1.0F);
    this->move_to(this->badge_sum, this->misplaced_sum, MatterAnchor::LB, MatterAnchor::LT, 0.0F, 1.0F);
    
    this->backdrop->resize(width, 0.0F);
    this->move_to(this->backdrop, 0.0F, height * 0.70F, MatterAnchor::LB);
    
    this->move_to(this->backpack, this->backdrop, MatterAnchor::LT, MatterAnchor::LB, 0.0F, -2.0F);
    this->move_to(this->info_board, this->backdrop, MatterAnchor::RT, MatterAnchor::RB, 0.0F, -2.0F);
    this->move_to(this->hashtable, this->misplaced_sum, MatterAnchor::RT, MatterAnchor::LT, float(title_fontsize));
    
    if (this->rucksacks.size() > 0) {
        float gxoff, gyoff;
        
        gxoff = float(normal_fontsize);
        gyoff = height * 0.56F;

        this->create_grid(grid_column, gxoff, gyoff, width - gxoff - float(normal_fontsize));

        for (int idx = 0; idx < this->rucksacks.size(); idx ++) {
            this->move_rucksack_to_grid(this->rucksacks[idx]);
        }
    }
}

void WarGrey::AoC::RucksackReorganizationPlane::on_enter(IPlane* from) {
    this->on_task_done();
    this->agent->play("Greeting", 1);
}

void WarGrey::AoC::RucksackReorganizationPlane::update(uint32_t count, uint32_t interval, uint32_t uptime) {
    switch (this->status) {
        case RRStatus::FindMisplacedItems: {
            if (this->current_rucksack_idx < this->rucksacks.size()) {
                Rucksack* self = this->rucksacks[this->current_rucksack_idx];

                self->switch_to_next_costume();
                this->display_items(self);
                
                this->misplaced_item_priority_sum += misplaced_item_priority(self->items, this->misplaced_dict1, this->misplaced_dict2);
                this->misplaced_sum->set_value(float(this->misplaced_item_priority_sum));

                this->current_rucksack_idx ++;
            } else {
                this->on_task_done();
            }
        }; break;
        case RRStatus::FindBadgeItems: {
            int n = int(sizeof(this->group_sizes) / sizeof(size_t));
            int rest = int(this->rucksacks.size()) - this->current_rucksack_idx;

            if (rest >= n) {
                for (int idx = 0; idx < n; idx ++) {
                    Rucksack* self = this->rucksacks[this->current_rucksack_idx + idx];

                    self->switch_to_next_costume();
                    this->group_items[idx] = self->items.c_str();
                    this->group_sizes[idx] = self->items.size();
                }

                this->badge_item_priority_sum += badge_item_priority(this->badge_dicts, this->group_items, this->group_sizes, n);
                this->badge_sum->set_value(float(this->badge_item_priority_sum));
                
                this->current_rucksack_idx += n;
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
        } else if (m == this->agent) {
            this->agent->play("GoodBye", 1);
            this->status = RRStatus::MissionDone;
        } else if (m == this->backpack) {
            if (this->backpack->item_count() > 0) {
                if (this->status == RRStatus::TaskDone) {
                    this->status = RRStatus::CreatePackHash;
                    this->hashtable->clear();
                    this->backpack->clear_dict();
                } else if (this->status == RRStatus::CreatePackHash) {
                    char item = this->backpack->compartment_popfront();

                    if (item != '\0') {
                        this->agent->play("GetArtsy", 1);
                        this->hashtable->insert_item(item);
                    } else {
                        this->backpack->compartment_lookup(this->hashtable);
                        this->status = RRStatus::TaskDone;
                    }
                }
            }
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
    bool okay = false;

    switch (this->status) {
        case RRStatus::CreatePackHash: okay = (m == this->backpack); break;
        case RRStatus::TaskDone: okay = true; break;
        default: break;
    }

    return okay;
}

bool WarGrey::AoC::RucksackReorganizationPlane::has_mission_completed() {
    return (this->status == RRStatus::MissionDone) && (!this->agent->in_playing());
}

void WarGrey::AoC::RucksackReorganizationPlane::move_rucksack_to_grid(Rucksack* rs) {
    this->move_to_grid(rs, int(rs->id), MatterAnchor::CB);
}

void WarGrey::AoC::RucksackReorganizationPlane::on_task_start(RRStatus status) {
    this->status = status;
    this->agent->play("Processing");
}

void WarGrey::AoC::RucksackReorganizationPlane::on_task_done() {
    this->status = RRStatus::TaskDone;
    this->info_board->set_text(MatterAnchor::RB, "");
    this->backpack->clear();
    this->hashtable->clear();
    this->agent->stop(1);
}

void WarGrey::AoC::RucksackReorganizationPlane::display_items(Rucksack* self) {
    size_t size = self->items.size();
    std::string items = self->items.substr(0, size / 2);

    items.push_back('\n');
    items.append(self->items.substr(size / 2));

    this->info_board->set_text(items, MatterAnchor::RB);
    this->backpack->set_items(self->items);
    this->hashtable->insert_items(self->items);
}

/*************************************************************************************************/
void WarGrey::AoC::RucksackReorganizationPlane::load_item_list(const std::string& pathname) {
    std::ifstream datin(pathname);
    
    this->rucksacks.clear();

    if (datin) {
        std::string line;
    
        while (std::getline(datin, line)) {
            this->rucksacks.push_back(new Rucksack(line, this->rucksacks.size()));
        }

        datin.close();
    }
}

/*************************************************************************************************/
WarGrey::AoC::Rucksack::Rucksack(const std::string& items, size_t id)
    : Sprite("sprite/rucksack/%s/%s", random_rucksack_style(id), random_rucksack_gender(id))
    , items(items), id(id) {}

/*************************************************************************************************/
WarGrey::AoC::Backpack::Backpack() : GridAtlas("spritesheet/items.png", 24, 21, 4, 4, true) {
    this->camouflage(false);
}

void WarGrey::AoC::Backpack::construct(SDL_Renderer* renderer) {
    this->create_map_grid(2, 26, rucksack_item_size, rucksack_item_size);
}

void WarGrey::AoC::Backpack::draw(SDL_Renderer* renderer, float x, float y, float Width, float Height) {
    SDL_FRect region;
    
    GridAtlas::draw(renderer, x, y, Width, Height);

    for (auto s_item : this->shared_indices) {
        this->feed_map_tile_region(&region, s_item);
        game_draw_rect(renderer, region.x + x, region.y + y, region.w, region.h, ROYALBLUE);
    }
}

void WarGrey::AoC::Backpack::set_items(const std::string& items) {
    if (this->items.compare(items) != 0) {
        this->clear();

        this->items = items;
        this->create_map_grid(2, int(this->items.size()) / 2);

        for (int idx = 0; idx < this->items.size(); idx ++) {
            int prior = item_priority(this->items[idx]);

            if (prior > 0) {
                this->tile_indices[idx] = atlas_vertical_index(prior, this->atlas_row, this->atlas_col);
            }
        }

        this->notify_updated();
    }
}

char WarGrey::AoC::Backpack::compartment_popfront() {
    char item = '\0';

    for (int idx = 0; idx < this->items.size() / 2; idx ++) {
        if (this->tile_indices[idx] > 0) {
            item = this->items[idx];
            this->tile_indices[idx] = 0;
            break;
        }
    }

    return item;
}

void WarGrey::AoC::Backpack::compartment_lookup(PackHash* dict) {
    if (dict != nullptr) {
        int size = int(this->items.size());

        for (int idx = size / 2; idx < size; idx ++) {
            if (this->tile_indices[idx] > 0) {
                if (dict->lookup(this->items[idx]) > 0) {
                    this->shared_indices.push_back(idx);
                }
            }
        }

        if (!this->shared_indices.empty()) {
            this->notify_updated();
        }
    }
}

int WarGrey::AoC::Backpack::get_atlas_tile_index(size_t map_idx) {
    int idx = -1;

    if (this->tile_indices[map_idx] > 0) {
        idx = this->tile_indices[map_idx];
    }

    return idx; 
}

void WarGrey::AoC::Backpack::clear() {
    this->clear_dict();
    
    this->items.clear();
    dict_zero(this->tile_indices);
}

void WarGrey::AoC::Backpack::clear_dict() {
    this->shared_indices.clear();
}

/*************************************************************************************************/
WarGrey::AoC::PackHash::PackHash() : GridAtlas("spritesheet/items.png", 24, 21, 4, 4, true) {}

void WarGrey::AoC::PackHash::construct(SDL_Renderer* renderer) {
    this->create_map_grid(2, 16, rucksack_item_size * hash_scale, rucksack_item_size * hash_scale);
}

void WarGrey::AoC::PackHash::draw(SDL_Renderer* renderer, float x, float y, float Width, float Height) {
    GridAtlas::draw(renderer, x, y, Width, Height);
    float xoff = (rucksack_item_size * hash_scale - float(normal_fontsize) * 0.5F) * 0.5F;
    float dx = Width / float(this->map_col);
    float cx = x;
    float cy = y + Height - Height / float(this->map_row);

    for (auto it = this->dict.begin(); it != this->dict.end(); it ++) {    
        game_draw_blended_text(aoc_font::mono, renderer, GRAY, cx + xoff, cy, std::to_string(it->second));
        cx += dx;
    }
}

void WarGrey::AoC::PackHash::insert_items(const std::string& items) {
    this->clear();

    for (int idx = 0; idx < items.size() / 2; idx ++) {
        this->insert_item(items[idx], false);
    }

    this->notify_updated();
}

void WarGrey::AoC::PackHash::insert_item(char ch, bool update) {
    int prior = item_priority(ch);
    int atlas_idx = atlas_vertical_index(prior, this->atlas_row, this->atlas_col);

    this->dict[atlas_idx] += 1;

    if (update) {
        this->notify_updated();
    }
}

int WarGrey::AoC::PackHash::lookup(char ch) {
    int prior = item_priority(ch);
    int atlas_idx = atlas_vertical_index(prior, this->atlas_row, this->atlas_col);
    int val = 0;
    
    if (this->dict.find(atlas_idx) != this->dict.end()) {
        val = this->dict[atlas_idx];
    }

    return val;
}

int WarGrey::AoC::PackHash::get_atlas_tile_index(size_t map_idx) {
    int idx = -1;
    size_t row = map_idx / this->map_col;
    size_t col = map_idx % this->map_col;

    if (row == 0) {
        if (col < this->dict.size()) {
            auto dit = this->dict.begin();

            for (size_t c = 0; c < col; c ++) {
                dit ++;
            }

            idx = dit->first;
        }
    }

    return idx; 
}
