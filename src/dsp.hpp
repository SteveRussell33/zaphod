#pragma once

const float C4 = 261.626;

inline float pitchToFreq(float pitch) {
	return powf(2.0, pitch) * C4;
}

inline float freqToPitch(float freq) {
	return log2f(freq / C4);
}

