#pragma once

const float C4 = 261.626;

inline float voctToFreq(float voct) {
	return powf(2.0, voct) * C4;
}

inline float freqToVOct(float freq) {
	return log2f(freq / C4);
}

