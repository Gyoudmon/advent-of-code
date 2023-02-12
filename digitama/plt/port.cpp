#include "port.hpp"

#include "../big_bang/graphics/text.hpp"
#include "../aoc.hpp"

using namespace WarGrey::STEM;
using namespace WarGrey::AoC;
using namespace WarGrey::PLT;

/*************************************************************************************************/
void WarGrey::PLT::PortPlane::construct(float width, float height) {}

void WarGrey::PLT::PortPlane::load(float width, float height) {
    this->title = this->insert(new Labellet(aoc_font::title, BLACK, title0_fmt, this->name()));
    this->backdrop = this->insert(new PortBackDrop(4, 32, 32.0F));
    
    this->agent = this->insert(new AgentLink());
    this->agent->scale(-1.0F, 1.0F);
}

void WarGrey::PLT::PortPlane::reflow(float width, float height) {
    this->move_to(this->title, this->agent, MatterAnchor::RB, MatterAnchor::LB);
    this->move_to(this->backdrop, width * 0.5F, height * 0.5F, MatterAnchor::CC);
}

void WarGrey::PLT::PortPlane::on_enter(IPlane* from) {
    this->on_task_done();
    this->agent->play("Greeting", 1);
}

void WarGrey::PLT::PortPlane::update(uint32_t count, uint32_t interval, uint32_t uptime) {
}

void WarGrey::PLT::PortPlane::after_select(IMatter* m, bool yes_or_no) {
    if (m == this->agent) {
        this->agent->play("GoodBye", 1);
        this->status = PStatus::MissionDone;
    }
}

bool WarGrey::PLT::PortPlane::can_select(IMatter* m) {
    return this->status == PStatus::TaskDone;
}

bool WarGrey::PLT::PortPlane::has_mission_completed() {
    return (this->status == PStatus::MissionDone) && (!this->agent->in_playing());
}

void WarGrey::PLT::PortPlane::on_task_start(PStatus status) {
    this->status = status;
    this->agent->play("Processing");
}

void WarGrey::PLT::PortPlane::on_task_done() {
    this->status = PStatus::TaskDone;
    this->agent->stop(1);
}
