#pragma once

const float C4 = 261.626;

inline float voctToFreq(float pitch) {
	return powf(2.0, pitch) * C4;
}

inline float freqToVoct(float freq) {
	return log2f(freq / C4);
}

