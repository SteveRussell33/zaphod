#pragma once

#include "rack.hpp"

using namespace rack;

extern Plugin *pluginInstance;

//struct ZaphodKnob : RoundKnob {
//    ZaphodKnob(const char* svg, int dim);
//
//    void redraw();
//};
//
//struct ZaphodKnob45 : ZaphodKnob {
//    ZaphodKnob45();
//};

struct ZaphodPort24 : SvgPort {
    ZaphodPort24();
};

