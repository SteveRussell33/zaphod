#include "widgets.hpp"

////-----------------------------------------------------------------
//
//ZaphodKnob::ZaphodKnob(const char* svg, int dim) {
//    setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, svg)));
//    box.size = Vec(dim, dim);
//    shadow->blurRadius = 2.0;
//    shadow->box.pos = Vec(0.0, 3.0);
//}
//
//void ZaphodKnob::redraw() {
//    event::Change c;
//    onChange(c);
//}
//
////-----------------------------------------------------------------
//
//ZaphodKnob45::ZaphodKnob45() : ZaphodKnob("res/knob45.svg", 45) {
//}
//
////-----------------------------------------------------------------

ZaphodPort24::ZaphodPort24() {
    setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/port.svg")));
    box.size = Vec(24, 24);
    shadow->blurRadius = 1.0;
    shadow->box.pos = Vec(0.0, 1.5);
}
