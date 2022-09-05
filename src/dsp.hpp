#pragma once

#include <math.h>
#include <cmath>

#include "../lib/earlevel/Biquad.h"

const float kC4 = 261.626;
const float kTwoPi = 2.0f * M_PI;

inline float freqToPitch(float freq) {
	return log2f(freq / kC4);
}

inline float pitchToFreq(float pitch) {
	return powf(2.0, pitch) * kC4;
}

struct Overdrive {

    float process(float in, float drive /* [0,1] */) {

        // TODO switch to wavetable lookup for tanhf, over -10 to 10. 

        // We are using cascading tanh to get full overdrive.
        float out = in * (1 - drive) + std::tanhf(in * M_PI) * drive;
        return out * (1 - drive) + std::tanhf(out * M_PI) * drive;
    }
};

//struct LowPass2PoleFilter {
//
//    Biquad filter;
//
//    void setBiquad(float cutoff, float sampleRate) {
//        float Fc = cutoff / sampleRate;
//        filter.setBiquad(bq_type_lowpass, Fc, 0.707, 0);
//    }
//
//    float process(float in) {
//        return filter.process(in);
//    }
//
//};

struct LowPass16PoleFilter {

    Biquad filter[8];

    void setBiquad(float cutoff, float sampleRate) {

        float Fc = cutoff / sampleRate;

        float q[8] = {
            0.50241929,
            0.52249861,
            0.56694403,
            0.64682178,
            0.78815462,
            1.0606777,
            1.7224471,
            5.1011486
        };

        for (int i = 0; i < 8; i++) {
            filter[i].setBiquad(bq_type_lowpass, Fc, q[i], 0);
        }
    }

    float process(float in) {
        float out = in;
        for (int i = 0; i < 8; i++) {
            out = filter[i].process(out);
        }
        return out;
    }
};

// Q values for Cascading filters
// https://www.earlevel.com/main/2016/09/29/cascading-filters/
// 
// 2-pole:
// 
//     0.70710678
// 
// 4-pole:
// 
//     0.54119610
//     1.3065630
// 
// 6-pole:
// 
//     0.51763809
//     0.70710678
//     1.9318517
// 
// 8-pole:
// 
//     0.50979558
//     0.60134489
//     0.89997622
//     2.5629154
// 
// 10-pole:
// 
//     0.50623256
//     0.56116312
//     0.70710678
//     1.1013446
//     3.1962266
// 
// 12-pole:
// 
//     0.50431448
//     0.54119610
//     0.63023621
//     0.82133982
//     1.3065630
//     3.8306488
// 
// 14-pole:
// 
//     0.50316379
//     0.52972649
//     0.59051105
//     0.70710678
//     0.93979296
//     1.5138713
//     4.4657021
// 
// 16-pole:
// 
//     0.50241929
//     0.52249861
//     0.56694403
//     0.64682178
//     0.78815462
//     1.0606777
//     1.7224471
//     5.1011486
// 
// 
