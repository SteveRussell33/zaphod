#pragma once

#include <math.h>

#include "../lib/bogaudio/BogaudioModules/src/dsp/math.hpp"

const float kTwoPi = 2.0f * M_PI;

struct Overdrive {
    bogaudio::dsp::FastTanhf fastTanhf;

    inline float value(float input, float drive /* [0,1] */) {

        // use cascading tanh for full overdrive
        float output = input * (1 - drive) + fastTanhf.value(input * M_PI) * drive;
        return output * (1 - drive) + fastTanhf.value(output * M_PI) * drive;
    }
};
