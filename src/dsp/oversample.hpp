#pragma once

#include "filters.hpp"

const int kMaxOversample = 16;

struct Oversample {

    int oversample;

    // Nuke everything above the Nyquist frequency.
    TwelvePoleLpf upLpf;
    TwelvePoleLpf downLpf;

    Oversample(int oversample_) : oversample(oversample_) {}

    void sampleRateChange(float sampleRate) {

        float nyquist = sampleRate / 2.0f;
        float oversampleRate = sampleRate * oversample;

        upLpf.setCutoff(nyquist, oversampleRate);
        downLpf.setCutoff(nyquist, oversampleRate);
    }

    void upsample(float in, float* buffer) {

        // Apply gain to compensate for zero-interpolation
        buffer[0] = upLpf.process(in * oversample);

        // Interpolate with zeros
        for (int i = 1; i < oversample; ++i) {
            buffer[i] = upLpf.process(0.0f);
        }
    }

    float downsample(float* buffer) {
        for (int i = 0; i < oversample; ++i) {
            downLpf.process(buffer[i]);
        }
        return buffer[0];
    }
};
