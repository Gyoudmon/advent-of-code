#include "digitama/magical_energy_expedition/rochambo.hpp"

#include "digitama/aoc.hpp"

#include <vector>

using namespace WarGrey::AoC;
using namespace WarGrey::STEM;

/*************************************************************************************************/
namespace {
    static const char* task_names[] {
        "冒\n险\n越\n来\n越\n深\n入\n了",
        "计点卡路里", "猜拳大赛"
    };

    class StarFruitlet : public WarGrey::STEM::Sprite {
        public:
            StarFruitlet(int day) : Sprite(digimon_path("sprite/star")), day(day) {}
            
        public:
            int day;
    };

    class MagicalEnergyExpeditionPlane : public Plane {
        public:
            MagicalEnergyExpeditionPlane() : Plane("Magical Energy Expedition") {}

        public:  // 覆盖游戏基本方法
            void load(float width, float height) override {
                this->titlet = this->insert(new Labellet(aoc_font::title, BLACK, title_fmt, 0, "魔法能量探险"));
                this->sledge = this->insert(new Sprite(digimon_path("sprite/sledge", ".png")));
                this->island = this->insert(new Sprite(digimon_path("sprite/island", ".png")));
                this->boat = this->insert(new Sprite(digimon_path("sprite/boat", ".png")));

                for (int idx = 0; idx < 25; idx ++) {
                    this->stars.push_back(this->insert(new StarFruitlet(idx + 1)));
                    if (idx < sizeof(task_names) / sizeof(char*) - 1) {
                        std::string vname = day_to_string(idx + 1) + "\n" + string_to_vertical_name(task_names[idx + 1]);
                        
                        this->names.push_back(this->insert(new Labellet(aoc_font::vertical, ROYALBLUE, "%s", vname.c_str())));
                        this->stars.back()->switch_to_custome("bright");
                    } else {
                        std::string vname = day_to_string(idx + 1) + "\n" + task_names[0];
                        
                        this->names.push_back(this->insert(new Labellet(aoc_font::vertical, GAINSBORO, "%s", vname.c_str())));
                        this->stars.back()->switch_to_custome("dark");
                    }
                    this->stars[idx]->scale(0.05F);
                }

                this->tux = this->insert(new Sprite(digimon_path("sprite/tux")));

                this->sledge->scale(0.80F);
                this->island->scale(1.80F);
            }
            
            void reflow(float width, float height) override {
                this->move_to(this->sledge, width, 0.0F, MatterAnchor::RT);
                this->move_to(this->island, width * 0.5F, height * 0.5F, MatterAnchor::CC, 0.0F, float(title_fontsize));
                this->move_to(this->boat, this->island, MatterAnchor::LB, MatterAnchor::LB);

                for (int idx = 0; idx < this->stars.size(); idx ++) {
                    if (idx == 0) {
                        this->move_to(this->stars[idx], this->titlet, MatterAnchor::LB, MatterAnchor::LT);
                    } else {
                        this->move_to(this->stars[idx], this->stars[idx - 1], MatterAnchor::RC, MatterAnchor::LC);
                    }

                    this->move_to(this->names[idx], this->stars[idx], MatterAnchor::CB, MatterAnchor::CT);
                }

                if (this->stars.size() == 0) {
                    this->move_to(this->tux, this->titlet, MatterAnchor::LB, MatterAnchor::LT);
                } else {
                    this->move_to(this->tux, this->stars[0], MatterAnchor::LB, MatterAnchor::LT);
                    this->tux->set_speed(5.0F, 0.0F);
                    this->tux->set_border_strategy(BorderStrategy::IGNORE);
                }

                this->tux->play("walk");
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
            }

        public:
            bool can_select(WarGrey::STEM::IMatter* m) override {
                return (dynamic_cast<StarFruitlet*>(m) != nullptr);
            }

            void after_select(IMatter* m, bool yes_or_no) override {
                if (yes_or_no) {

                }
            }

        private:
            Labellet* titlet;
            std::vector<Sprite*> stars;
            std::vector<Labellet*> names;
            Sprite* sledge;
            Sprite* island;
            Sprite* boat;
            Sprite* tux;
    };

    class MagicalEnergyExpeditionCosmos : public Cosmos {
        public:
            MagicalEnergyExpeditionCosmos() : Cosmos(24) {
                aoc_fonts_initialize();
            }

            virtual ~MagicalEnergyExpeditionCosmos() {
                aoc_fonts_destroy();
            }

        public:  // 覆盖游戏基本方法
            void construct(int argc, char* argv[]) override {
                this->set_window_size(1200, 1200);

                this->push_plane(new MagicalEnergyExpeditionPlane());
                this->push_plane(new RochamboPlane(task_names[2]));
            }
    };
}

/*************************************************************************************************/
int main(int argc, char* args[]) {
    MagicalEnergyExpeditionCosmos universe;

    universe.construct(argc, args);
    universe.big_bang();

    return 0;
}
