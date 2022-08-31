#pragma once

const float kC4 = 261.626;

inline float voctToFreq(float pitch) {
	return powf(2.0, pitch) * kC4;
}

inline float freqToVoct(float freq) {
	return log2f(freq / kC4);
}

