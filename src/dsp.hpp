#pragma once

#include <math.h>

#include "../lib/bogaudio/BogaudioModules/src/dsp/math.hpp"

// const float kTwoPi = 2.0f * M_PI;

struct Overdrive {
    bogaudio::dsp::FastTanhf fastTanhf;

    inline float value(float input, float drive /* [0,1] */) {
        float tanh = fastTanhf.value(input * M_PI);
        return input * (1 - drive) + tanh * drive;
    }
};
