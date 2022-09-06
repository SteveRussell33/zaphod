#pragma once

#include <cmath>
#include <math.h>

#include "Biquad.h"

//--------------------------------------------------------------
// Math
//--------------------------------------------------------------

const float kTwoPi = 2.0f * M_PI;

struct Overdrive {

    float process(float in, float drive /* [0,1] */) {

        // TODO switch to wavetable lookup for tanhf.

        // Double Overdrive (TM)
        float out = in * (1 - drive) + std::tanhf(in * M_PI) * drive;
        return out * (1 - drive) + std::tanhf(out * M_PI) * drive;
    }
};

//--------------------------------------------------------------
// Pitch
//--------------------------------------------------------------

const float kC4 = 261.626;

inline float freqToPitch(float freq) { return log2f(freq / kC4); }

inline float pitchToFreq(float pitch) { return powf(2.0, pitch) * kC4; }
