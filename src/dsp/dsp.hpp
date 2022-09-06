#pragma once

#include <cmath>
#include <math.h>

#include "Biquad.h"

//--------------------------------------------------------------
// Math
//--------------------------------------------------------------

const float kTwoPi = 2.0f * M_PI;

// https://varietyofsound.wordpress.com/2011/02/14/efficient-tanh-computation-using-lamberts-continued-fraction/
inline float fastTanh(float x) {
    float x2 = x * x;
    float a = x * (135135.0f + x2 * (17325.0f + x2 * (378.0f + x2)));
    float b = 135135.0f + x2 * (62370.0f + x2 * (3150.0f + x2 * 28.0f));
    return a / b;
}

//--------------------------------------------------------------
// Pitch
//--------------------------------------------------------------

const float kC4 = 261.626;

inline float freqToPitch(float freq) { return log2f(freq / kC4); }

inline float pitchToFreq(float pitch) { return powf(2.0, pitch) * kC4; }
