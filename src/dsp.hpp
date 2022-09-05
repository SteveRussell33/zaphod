#pragma once

#include <math.h>
#include <cmath>

const float kC4 = 261.626;
const float kTwoPi = 2.0f * M_PI;

inline float freqToPitch(float freq) {
	return log2f(freq / kC4);
}

inline float pitchToFreq(float pitch) {
	return powf(2.0, pitch) * kC4;
}

struct Overdrive {

    inline float value(float in, float drive /* [0,1] */) {

        // TODO switch to wavetable lookup for tanhf, over -10 to 10. 

        // use cascading tanh for full overdrive
        float out = in * (1 - drive) + std::tanhf(in * M_PI) * drive;
        return out * (1 - drive) + std::tanhf(out * M_PI) * drive;
    }
};

