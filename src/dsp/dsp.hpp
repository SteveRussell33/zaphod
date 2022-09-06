#pragma once

#include <cmath>
#include <math.h>

#include "Biquad.h"

//--------------------------------------------------------------
// Math
//--------------------------------------------------------------

const float kTwoPi = 2.0f * M_PI;

//--------------------------------------------------------------
// Pitch
//--------------------------------------------------------------

const float kC4 = 261.626;

inline float freqToPitch(float freq) { return log2f(freq / kC4); }

inline float pitchToFreq(float pitch) { return powf(2.0, pitch) * kC4; }
