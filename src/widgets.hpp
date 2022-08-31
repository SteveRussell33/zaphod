#pragma once

#include "rack.hpp"

using namespace rack;

extern Plugin *pluginInstance;

struct ZaphodKnob : RoundKnob {
    ZaphodKnob(const char* svg, int dim) {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, svg)));
        box.size = Vec(dim, dim);
    }

    void redraw() {
        event::Change c;
        onChange(c);
    }
};

struct ZaphodKnob18 : ZaphodKnob {
    ZaphodKnob18() : ZaphodKnob("res/knob18.svg", 18) {
        shadow->blurRadius = 2.0;
        shadow->box.pos = Vec(0.0, 3.0);
    }
};

struct ZaphodKnob40 : ZaphodKnob {
    ZaphodKnob40() : ZaphodKnob("res/knob40.svg", 40) {
        shadow->blurRadius = 2.0;
        shadow->box.pos = Vec(0.0, 3.0);
    }
};

struct ZaphodKnob50 : ZaphodKnob {
    ZaphodKnob50() : ZaphodKnob("res/knob50.svg", 50) {
        shadow->blurRadius = 2.7;
        shadow->box.pos = Vec(0.0, 4.0);
    }
};

struct ZaphodPort : SvgPort {
    ZaphodPort() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/port.svg")));
        box.size = Vec(24, 24);
        shadow->blurRadius = 1.0;
        shadow->box.pos = Vec(0.0, 1.5);
    }
};

struct ZaphodHSwitch14 : SvgSwitch {
	ZaphodHSwitch14() {
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/hswitch14-0.svg")));
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/hswitch14-1.svg")));
    }
};
