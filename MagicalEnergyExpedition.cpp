#include "digitama/magical_energy_expedition/calorie_counting.hpp"
#include "digitama/magical_energy_expedition/rochambo.hpp"
#include "digitama/magical_energy_expedition/rucksack_reorganization.hpp"

#include "digitama/aoc.hpp"
#include "digitama/pltmos/stream.hpp"

#include "digitama/big_bang/datum/string.hpp"

#include <vector>
#include <filesystem>

using namespace WarGrey::STEM;
using namespace WarGrey::AoC;
using namespace WarGrey::PLT;

/*************************************************************************************************/
namespace {
#ifndef __windows__
    static const char* unknown_task_name = "冒\n险\n越\n来\n越\n深\n入\n了";
    static const char* task_name_fmt = "%02d\n%s";
#else
    static const char* unknown_task_name = "冒险越来越深入了";
    static const char* task_name_fmt = "%02d %s";
#endif

    static const int elf_on_boat_count = 0;
    static const int advent_days = 25;

    /*********************************************************************************************/
    class StarFruitlet : public WarGrey::STEM::Sprite {
    public:
#ifndef __windows__
        StarFruitlet(int day) : Sprite("sprite/star"), day(day) {}
#else
        StarFruitlet(const std::string& name, int day)
            : Sprite("sprite/star"), name(name), day(day) {}

    public:
        std::string name;
#endif
        
    public:
        int day;
    };

    /*********************************************************************************************/
    class MagicalEnergyExpeditionPlane : public Plane {
    public:
        MagicalEnergyExpeditionPlane(Cosmos* master) : Plane("Magical Energy Expedition"), master(master) {}

    public:  // 覆盖游戏基本方法
        void load(float width, float height) override {
            int bonus_idx = 0;

            this->sledge = this->insert(new GridAtlas("sledge.png"));
            this->splash = this->insert(new GridAtlas("splash.png"));
            this->title = this->insert(new Labellet(GameFont::Title(), BLACK, title0_fmt, "魔法能量远征"));
            this->boat = this->insert(new Sprite("boat.png"));

            this->agent = this->insert(new Linkmon());
            this->agent->scale(-1.0F, 1.0F);
            
            for (int idx = 0; idx < advent_days; idx ++) {
                const char* task_name = this->master->plane_name(idx + 1);
                bool is_plt_name = is_plt_plane_name(task_name);
                
                if ((bonus_idx > 0) || (task_name == nullptr) || is_plt_name) {
                    std::string vname = make_nstring(task_name_fmt, idx + 1, unknown_task_name);
            
#ifndef __windows__
                    this->names.push_back(this->insert(new Labellet(GameFont::fangsong(), GAINSBORO, "%s", vname.c_str())));
                    this->stars.push_back(this->insert(new StarFruitlet(idx + 1)));
#else
                    this->stars.push_back(this->insert(new StarFruitlet(vname, idx + 1)));
#endif

                    this->stars[idx]->scale(0.05F);
                    this->stars.back()->switch_to_costume("dark");
                    
                    if (is_plt_name) {
                        bonus_idx = idx + 1;
                    }
                } else {
#ifndef __windows__
                    std::string vname = make_nstring(task_name_fmt, idx + 1, string_add_between(task_name).c_str());

                    this->names.push_back(this->insert(new Labellet(GameFont::fangsong(), ROYALBLUE, "%s", vname.c_str())));
                    this->stars.push_back(this->insert(new StarFruitlet(idx + 1)));
#else
                    std::string vname = make_nstring(task_name_fmt, idx + 1, task_name);

                    this->stars.push_back(this->insert(new StarFruitlet(vname, idx + 1)));
#endif

                    this->stars[idx]->scale(0.05F);
                    this->stars.back()->switch_to_costume("bright");
                }
            }

            for (int idx = bonus_idx; idx < this->master->plane_count(); idx ++) {
                this->bonuses.push_back(this->insert(new Coinlet(this->master->plane_name(idx), idx)));
            }

            this->tux = this->insert(new Tuxmon());
            this->tux->wear("santa_hat");

            for (int idx = 0; idx < santa_elf_type_count; idx ++) {
                this->elves[idx] = this->insert(new ElfSheet(idx));
                if (idx < elf_on_boat_count) {
                    this->elves[idx]->scale(0.618F);
                }
            }

            this->tooltip = this->insert(make_label_for_tooltip(GameFont::Tooltip()));
            this->set_tooltip_matter(this->tooltip);

            this->sledge->scale(0.80F);
            this->splash->scale(1.80F);
        }
        
        void reflow(float width, float height) override {
            this->move_to(this->title, this->agent, MatterAnchor::RB, MatterAnchor::LB);
            this->move_to(this->sledge, width, 0.0F, MatterAnchor::RT);
            this->move_to(this->splash, width * 0.5F, height, MatterAnchor::CB);

            this->move_to(this->boat, this->splash, 0.1F, 0.9F, MatterAnchor::LB);
            
            for (int idx = 0; idx < elf_on_boat_count; idx ++) {
                if (idx == 0) {
                    this->move_to(this->elves[idx], this->boat, MatterAnchor::LB, MatterAnchor::RB);
                } else {
                    this->move_to(this->elves[idx], this->elves[idx - 1], MatterAnchor::LB, MatterAnchor::RB);
                }
            }

            for (int idx = elf_on_boat_count; idx < santa_elf_type_count; idx ++) {
                if (idx == elf_on_boat_count) {
                    this->move_to(this->elves[idx], this->sledge, MatterAnchor::RB, MatterAnchor::RT);
                } else {
                    this->move_to(this->elves[idx], this->elves[idx - 1], MatterAnchor::LC, MatterAnchor::RC);
                }
            }
            
            for (int idx = 0; idx < this->stars.size(); idx ++) {
                if (idx == 0) {
                    this->move_to(this->stars[idx], this->agent, MatterAnchor::LB, MatterAnchor::LT);
                } else {
                    this->move_to(this->stars[idx], this->stars[idx - 1], MatterAnchor::RC, MatterAnchor::LC);
                }

#ifndef __windows__
                this->move_to(this->names[idx], this->stars[idx], MatterAnchor::CB, MatterAnchor::CT);
#endif
            }

            if (this->stars.size() == 0) {
                this->move_to(this->tux, this->agent, MatterAnchor::LB, MatterAnchor::LT);
            } else {
                this->move_to(this->tux, this->stars[0], MatterAnchor::LB, MatterAnchor::LT);
            }

            if (this->bonuses.size() > 0) {
                this->move_to(this->bonuses[0], this->boat, MatterAnchor::LT, MatterAnchor::LT);
            }
        }

        void update(uint64_t count, uint32_t interval, uint64_t uptime) override {
            if (this->stars.size() > 0) {
                float tux_lx, tux_rx, stars_rx;

                this->feed_matter_location(this->tux, &tux_rx, nullptr, MatterAnchor::RB);
                this->feed_matter_location(this->stars.back(), &stars_rx, nullptr, MatterAnchor::RB);

                if (tux_rx >= stars_rx) {
                    this->feed_matter_location(this->tux, &tux_lx, nullptr, MatterAnchor::CB);
            
                    if (tux_lx < stars_rx) {
                        this->tux->play("skid");
                    } else {
                        this->move(this->tux, - tux_rx, 0.0F);
                        this->tux->play("walk");
                    }
                }
            }

            if (count % 4 == 0) { /* move the expedition team */
                float dx = float(random_uniform(-1, 1));
                float dy = float(random_uniform(-1, 1));

                this->move(this->boat, dx, dy);
                for (int idx = 0; idx < elf_on_boat_count; idx ++) {
                    this->move(this->elves[idx], dx, dy);
                }

                if (this->bonuses.size() > 0) {
                    this->move(this->bonuses[0], dx, dy);
                }
            }

            if (this->target_plane > 0) {
                if (!this->agent->in_playing()) {
                    this->master->transfer_to_plane(this->target_plane);
                    this->target_plane = 0;
                }
            }
        }

        void on_enter(IPlane* from) override {
            this->agent->play("Greeting", 1);

            this->tux->set_border_strategy(BorderStrategy::IGNORE);
            this->tux->set_velocity(2.0F, 0.0F);
            
            for (int idx = 0; idx < santa_elf_type_count; idx ++) {
                if (idx < elf_on_boat_count) {
                    this->elves[idx]->play("rwalk");
                } else {
                    this->elves[idx]->play("lwalk");
                }
            }
        }

    public:
        bool can_select(IMatter* m) override {
            return dynamic_cast<ElfSheet*>(m) != nullptr;
        }

        void on_tap(IMatter* m, float x, float y) override {
            if (m == this->tux) {
                if (this->tux->is_wearing()) {
                    this->tux->take_off();
                } else {
                    this->tux->wear("santa_hat");
                }
            } else {
                StarFruitlet* star = dynamic_cast<StarFruitlet*>(m);
                Coinlet* coin = dynamic_cast<Coinlet*>(m);
                ElfSheet* elf = dynamic_cast<ElfSheet*>(m);
                
                if (star != nullptr) {
                    if (star->day < this->master->plane_count()) {
                        this->target_plane = star->day;
                        this->agent->play("Hide", 1);
                    }
                } else if (coin != nullptr) {
                    this->target_plane = coin->idx;
                    this->agent->play("Hide", 1);
                } else if (elf != nullptr) {
                    this->glide_to_random_location(4.0F, elf);
                    this->set_selected(elf);
                }
            }
        }

        void on_tap_selected(IMatter* m, float x, float y) override {
            ElfSheet* elf = dynamic_cast<ElfSheet*>(m);
            
            if (elf != nullptr) {
                this->glide(1.0F, elf, 32.0F, 0.0F);
                this->glide(1.0F, elf, 0.0F, -32.0F);
                this->glide(1.0F, elf, -32.0F, 0.0F);
                this->glide(1.0F, elf, 0.0F, 32.0F);
            }
        }

    protected:
        bool update_tooltip(IMatter* m, float local_x, float local_y, float global_x, float global_y) override {
            bool updated = false;
            auto coin = dynamic_cast<Coinlet*>(m);

#ifdef __windows__
            auto star = dynamic_cast<StarFruitlet*>(m);

            if (star != nullptr) {
                this->tooltip->set_text(" %s ", star->name.c_str());
                updated = true;
            } else
#endif
            if (coin != nullptr) {
                this->tooltip->set_text(" %s ", coin->name.c_str());
                updated = true;
            }

            return updated;
        }

    private:
        Linkmon* agent;
        Labellet* title;
        Labellet* tooltip;
        std::vector<Sprite*> stars;
        std::vector<Labellet*> names;
        std::vector<Coinlet*> bonuses;
        Tuxmon* tux;
        ElfSheet* elves[santa_elf_type_count];
        GridAtlas* sledge;
        GridAtlas* splash;
        Sprite* boat;
        
    private:
        Cosmos* master;
        int target_plane = 0;
    };

    /*************************************************************************************************/
    enum class CmdlineOps { TopCount, GroupSize, _ };

    class MagicalEnergyExpeditionCosmos : public Cosmos {
    public:
        MagicalEnergyExpeditionCosmos(const char* process_path) : Cosmos(60) {
            enter_digimon_zone(process_path);
            imgdb_setup(digimon_zonedir().append("stone"));
        }

        virtual ~MagicalEnergyExpeditionCosmos() {
            imgdb_teardown();
        }

    public:  // 覆盖游戏基本方法
        void construct(int argc, char* argv[]) override {
            this->parse_cmdline_options(argc, argv);
            this->set_window_size(1200, 0);
            GameFont::fontsize(21);

            this->push_plane(new MagicalEnergyExpeditionPlane(this));
            this->push_plane(new CalorieCountingPlane(this->top_count));
            this->push_plane(new RochamboPlane());
            this->push_plane(new RucksackReorganizationPlane());

            this->push_plane(new StreamPlane());
        }

    protected:
        void update(uint64_t count, uint32_t interval, uint64_t uptime) override {
            if (this->has_current_mission_completed()) {
                this->transfer_to_plane(0);
            }
        }

    private:
        void parse_cmdline_options(int argc, char* argv[]) {
            CmdlineOps opt = CmdlineOps::_;
            std::string datin;

            // this->set_cmdwin_height(32);

            for (int idx = 1; idx < argc; idx ++) {
                switch (opt) {
                    case CmdlineOps::TopCount: {
                        this->top_count = std::stoi(argv[idx]);
                        opt = CmdlineOps::_;
                    }; break;
                    default: {
                        if (strncmp("--tc", argv[idx], 5) == 0) {
                            opt = CmdlineOps::TopCount;
                        } else {
                            datin = std::string(argv[idx]);
                        }
                    }
                }
            }
        }

    private:
        int top_count = 0;
    };
}

/*************************************************************************************************/
int main(int argc, char* args[]) {
    MagicalEnergyExpeditionCosmos universe(args[argc]);

    universe.construct(argc, args);
    universe.big_bang();

    return 0;
}
