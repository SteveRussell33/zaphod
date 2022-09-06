#pragma once

#include "filters.hpp"

const int kMaxOversample = 16;

struct Oversample {

    int oversample;

    TwelvePoleLpf upLpf;
    TwelvePoleLpf downLpf;

    Oversample(int oversample_) : oversample(oversample_) {}

    void sampleRateChange(float sampleRate) {

        float nyquist = sampleRate / 2.0f;
        float oversampleRate = sampleRate * oversample;

        upLpf.setBiquad(nyquist, oversampleRate);
        downLpf.setBiquad(nyquist, oversampleRate);
    }

    void up(float in, float* buffer) {

        // Apply gain to compensate for zero-interpolation
        buffer[0] = upLpf.process(in * oversample);

        // Interpolate with zeros
        for (int i = 1; i < oversample; ++i) {
            buffer[i] = upLpf.process(0.0f);
        }
    }

    float down(float* buffer) {
        for (int i = 0; i < oversample; ++i) {
            downLpf.process(buffer[i]);
        }
        return buffer[0];
    }
};
