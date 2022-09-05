#pragma once

#include <math.h>

#include "../lib/bogaudio/BogaudioModules/src/dsp/math.hpp"

const float kTwoPi = 2.0f * M_PI;

struct Overdrive {
    bogaudio::dsp::FastTanhf fastTanhf;

    inline float value(float in, float drive /* [0,1] */) {

        // use cascading tanh for full overdrive
        float out = in * (1 - drive) + fastTanhf.value(in * M_PI) * drive;
        return out * (1 - drive) + fastTanhf.value(out * M_PI) * drive;
    }
};
