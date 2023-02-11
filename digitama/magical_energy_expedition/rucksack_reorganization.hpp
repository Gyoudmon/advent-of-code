#pragma once // 确保只被 include 一次

#include "../big_bang/game.hpp"
#include "../aoc.hpp"

#include <vector>
#include <map>

namespace WarGrey::AoC {
    /******************************************* 声明游戏精灵 ******************************************/
    static const unsigned int DICT_SIZE = 53;

    class Rucksack : public WarGrey::STEM::Sprite {
    public:
        Rucksack(const std::string& items, size_t id);
        
    public:
        std::string items;
        size_t id;
    };

    class PackHash : public WarGrey::STEM::GridAtlas {
    public:
        PackHash();

    public:
        void construct(SDL_Renderer* renderer) override;
        void draw(SDL_Renderer* renderer, float x, float y, float Width, float Height) override;

    public:
        void insert_items(const std::string& items);
        void insert_item(char ch, bool update = true);
        int lookup(char ch);
        void clear() { this->dict.clear(); }

    protected:
        int get_atlas_tile_index(size_t map_idx) override;
    
    private:
        std::map<int, int> dict;
    };

    class Backpack : public WarGrey::STEM::GridAtlas {
    public:
        Backpack();

    public:
        void construct(SDL_Renderer* renderer) override;
        void draw(SDL_Renderer* renderer, float x, float y, float Width, float Height) override;

    public:
        void set_items(const std::string& items);
        size_t item_count() { return this->items.size(); }
        char compartment_popfront();
        void compartment_lookup(WarGrey::AoC::PackHash* dict);
        void clear();
        void clear_dict();

    protected:
        int get_atlas_tile_index(size_t map_idx) override;

    private:
        std::string items;
        int tile_indices[DICT_SIZE] = {};
        std::vector<int> shared_indices;
    };

    /******************************************* 声明游戏状态 ******************************************/
    enum class RRStatus {
        FindMisplacedItems,
        FindBadgeItems,
        CreatePackHash,
        TaskDone,
        MissionDone,
    };
    
    /******************************************* 声明游戏世界 ******************************************/
    class RucksackReorganizationPlane : public WarGrey::STEM::Plane {
    public:
        RucksackReorganizationPlane() : Plane("背包重组") {}
        virtual ~RucksackReorganizationPlane();

    public:  // 覆盖游戏基本方法
        void construct(float width, float height) override;
        void load(float width, float height) override;
        void reflow(float width, float height) override;
        void update(uint32_t count, uint32_t interval, uint32_t uptime) override;

    public:
        bool can_select(WarGrey::STEM::IMatter* m) override;
        void after_select(WarGrey::STEM::IMatter* m, bool yes_or_no) override;

    public:
        bool has_mission_completed() override;
        void on_enter(WarGrey::STEM::IPlane* from) override;

    private:
        void on_task_start(WarGrey::AoC::RRStatus status);
        void on_task_done();

    private:
        void move_rucksack_to_grid(WarGrey::AoC::Rucksack* rucksack);
        void display_items(WarGrey::AoC::Rucksack* rucksack);

    private:
        void load_item_list(const std::string& pathname);

    private: // 本游戏世界中的物体
        WarGrey::AoC::AgentLink* agent;
        WarGrey::STEM::Labellet* title;
        WarGrey::STEM::GridAtlas* backdrop;
        WarGrey::STEM::Labellet* population;
        WarGrey::STEM::Labellet* info_board;
        WarGrey::STEM::Dimensionlet* misplaced_sum;
        WarGrey::STEM::Dimensionlet* badge_sum;
        std::vector<WarGrey::AoC::Rucksack*> rucksacks;
        WarGrey::AoC::Backpack* backpack;
        WarGrey::AoC::PackHash* hashtable;

    private:
        WarGrey::AoC::RRStatus status;
        WarGrey::STEM::DimensionStyle style;

    private: // shared variable
        int current_rucksack_idx;

    private: // for FindMisplacedItems
        int misplaced_item_priority_sum;
        char misplaced_dict1[DICT_SIZE];
        char misplaced_dict2[DICT_SIZE];

    private: // for FindBadgeItem
        char* badge_dicts[2];
        const char* group_items[3];
        size_t group_sizes[3];
        int badge_item_priority_sum;
    };
}
