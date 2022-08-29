#pragma once

#include "rack.hpp"

using namespace rack;

extern Plugin *pluginInstance;

struct ZaphodKnob : RoundKnob {
    ZaphodKnob(const char* svg, int dim) {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, svg)));
        box.size = Vec(dim, dim);
        shadow->blurRadius = 2.0;
        shadow->box.pos = Vec(0.0, 3.0);
    }

    void redraw() {
        event::Change c;
        onChange(c);
    }
};

struct ZaphodKnob18 : ZaphodKnob {
    ZaphodKnob18() : ZaphodKnob("res/knob18.svg", 18) {}
};

struct ZaphodKnob45 : ZaphodKnob {
    ZaphodKnob45() : ZaphodKnob("res/knob45.svg", 45) {}
};

struct ZaphodPort : SvgPort {
    ZaphodPort() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/port.svg")));
        box.size = Vec(24, 24);
        shadow->blurRadius = 1.0;
        shadow->box.pos = Vec(0.0, 1.5);
    }
};

