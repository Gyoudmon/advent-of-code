#include "digitama/magical_energy_expedition/calorie_counting.hpp"
#include "digitama/magical_energy_expedition/rochambo.hpp"

#include "digitama/big_bang/physics/random.hpp"
#include "digitama/sprite/ulpc.hpp"
#include "digitama/aoc.hpp"

#include <vector>

using namespace WarGrey::AoC;
using namespace WarGrey::STEM;

/*************************************************************************************************/
namespace {
    static const char* unknown_task_name = "冒\n险\n越\n来\n越\n深\n入\n了";

    class StarFruitlet : public WarGrey::STEM::Sprite {
    public:
        StarFruitlet(int day) : Sprite("sprite/star"), day(day) {}
        
    public:
        int day;
    };

    class MagicalEnergyExpeditionPlane : public Plane {
    public:
        MagicalEnergyExpeditionPlane(Cosmos* master)
            : Plane("Magical Energy Expedition")
            , master(master) {}

    public:  // 覆盖游戏基本方法
        void load(float width, float height) override {
            this->logo = this->insert(new Sprite("logo.png"));
            this->title = this->insert(new Labellet(aoc_font::title, BLACK, title_fmt, 0, "魔法能量探险"));
            this->sledge = this->insert(new Sprite("sledge.png"));
            this->island = this->insert(new Sprite("island.png"));
            this->boat = this->insert(new Sprite("boat.png"));

            for (int idx = 0; idx < 25; idx ++) {
                const char* task_name = this->master->plane_name(idx + 1);
                
                this->stars.push_back(this->insert(new StarFruitlet(idx + 1)));
                this->stars[idx]->scale(0.05F);

                if (task_name != nullptr) {
                    std::string vname = day_to_string(idx + 1) + "\n" + string_to_vertical_name(task_name);
            
                    this->names.push_back(this->insert(new Labellet(aoc_font::vertical, ROYALBLUE, "%s", vname.c_str())));
                    this->stars.back()->switch_to_costume("bright");
                } else {
                    std::string vname = day_to_string(idx + 1) + "\n" + unknown_task_name;
            
                    this->names.push_back(this->insert(new Labellet(aoc_font::vertical, GAINSBORO, "%s", vname.c_str())));
                    this->stars.back()->switch_to_costume("dark");
                }
            }

            this->tux = this->insert(new Sprite("sprite/tux"));
            this->elves[0] = this->insert(new ElfSheet("male"));
            this->elves[1] = this->insert(new ElfSheet("dress"));
            this->elves[2] = this->insert(new ElfSheet("goblin"));
            this->elves[3] = this->insert(new ElfSheet("female"));

            this->sledge->scale(0.80F);
            this->island->scale(1.80F);
        }
        
        void reflow(float width, float height) override {
            this->move_to(this->title, this->logo, MatterAnchor::RC, MatterAnchor::LC);
            this->move_to(this->sledge, width, 0.0F, MatterAnchor::RT);
            this->move_to(this->island, width * 0.5F, height * 0.5F, MatterAnchor::CC, 0.0F, float(title_fontsize));
            this->move_to(this->boat, this->island, MatterAnchor::LB, MatterAnchor::LB);
            this->move_to(this->elves[0], this->boat, MatterAnchor::LC, MatterAnchor::RC);
            this->move_to(this->elves[1], this->elves[0], MatterAnchor::LC, MatterAnchor::RC);
            this->move_to(this->elves[2], this->sledge, MatterAnchor::RB, MatterAnchor::RT);
            this->move_to(this->elves[3], this->elves[2], MatterAnchor::LC, MatterAnchor::RC);
            
            for (int idx = 0; idx < this->stars.size(); idx ++) {
                if (idx == 0) {
                    this->move_to(this->stars[idx], this->logo, MatterAnchor::LB, MatterAnchor::LT);
                } else {
                    this->move_to(this->stars[idx], this->stars[idx - 1], MatterAnchor::RC, MatterAnchor::LC);
                }

                this->move_to(this->names[idx], this->stars[idx], MatterAnchor::CB, MatterAnchor::CT);
            }

            if (this->stars.size() == 0) {
                this->move_to(this->tux, this->title, MatterAnchor::LB, MatterAnchor::LT);
            } else {
                this->move_to(this->tux, this->stars[0], MatterAnchor::LB, MatterAnchor::LT);
            }
        }

        void update(uint32_t count, uint32_t interval, uint32_t uptime) override {
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

            /* move the expedition team */ {
                float dx = float(random_uniform(-1, 2));
                float dy = float(random_uniform(-1, 1));

                this->move(this->boat, dx, dy);
                this->move(this->elves[0], dx, dy);
                this->move(this->elves[1], dx, dy);
            }
        }

        void on_enter(IPlane* from) override {
            this->tux->play("walk");
            this->tux->set_border_strategy(BorderStrategy::IGNORE);
            this->tux->set_speed(4.0F, 0.0F);

            this->elves[0]->play("rwalk");
            this->elves[1]->play("rwalk");
            this->elves[2]->play("lwalk");
            this->elves[3]->play("lwalk");
        }

    public:
        bool can_select(WarGrey::STEM::IMatter* m) override {
            return (dynamic_cast<StarFruitlet*>(m) != nullptr)
                    || (m == this->tux);
        }

        void after_select(IMatter* m, bool yes_or_no) override {
            if (yes_or_no) {
                if (m == this->tux) {
                    if (this->tux->is_wearing()) {
                        this->tux->take_off();
                    } else {
                        this->tux->wear("santa_hat");
                    }
                } else {
                    StarFruitlet* self = dynamic_cast<StarFruitlet*>(m);

                    if (self->day < this->master->plane_count()) {
                        this->master->transfer_to_plane(self->day);
                    }
                }
            }
        }

    private:
        WarGrey::STEM::Sprite* logo;
        Labellet* title;
        std::vector<Sprite*> stars;
        std::vector<Labellet*> names;
        Sprite* tux;
        ElfSheet* elves[4];
        Sprite* sledge;
        Sprite* island;
        Sprite* boat;
        
    private:
        Cosmos* master;
    };

    /*************************************************************************************************/
    enum class CmdlineOps { TopCount, _ };

    class MagicalEnergyExpeditionCosmos : public Cosmos {
    public:
        MagicalEnergyExpeditionCosmos() : Cosmos(24) {
            aoc_fonts_initialize();
            imgdb_setup(__ZONE__ "stone");
        }

        virtual ~MagicalEnergyExpeditionCosmos() {
            imgdb_teardown();
            aoc_fonts_destroy();
        }

    public:  // 覆盖游戏基本方法
        void construct(int argc, char* argv[]) override {
            this->parse_cmdline_options(argc, argv);
            this->set_window_size(1200, 1200);

            this->push_plane(new MagicalEnergyExpeditionPlane(this));
            this->push_plane(new CalorieCountingPlane(this->top_count));
            this->push_plane(new RochamboPlane());
        }

    protected:
        void update(uint32_t count, uint32_t interval, uint32_t uptime) override {
            if (this->has_current_mission_completed()) {
                this->transfer_to_plane(0);
            }
        }

    private:
        void parse_cmdline_options(int argc, char* argv[]) {
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
        }

    private:
        int top_count = 0;
    };
}

/*************************************************************************************************/
int main(int argc, char* args[]) {
    MagicalEnergyExpeditionCosmos universe;

    universe.construct(argc, args);
    universe.big_bang();

    return 0;
}
