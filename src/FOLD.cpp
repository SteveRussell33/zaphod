#include "dsp.hpp"
#include "plugin.hpp"
#include "widgets.hpp"

#include "../lib/bogaudio/BogaudioModules/src/dsp/filters/multimode.hpp"

// define FOLD_DEBUG

struct FOLD : Module {

    Overdrive overdrive;

    int kOversample = 4;
    bogaudio::dsp::MultimodeFilter16 oversampleFilter;

    enum ParamId {
        kTimbreParam,
        kTimbreCvAmountParam,

        kParamsLen
    };

    enum InputId {
        kTimbreCvInput,
        kInput,

        kInputsLen
    };

    enum OutputId {
        kOutput,

#ifdef FOLD_DEBUG
        kDebug1,
        kDebug2,
        kDebug3,
        kDebug4,
#endif
        kOutputsLen
    };

    FOLD() {
        config(kParamsLen, kInputsLen, kOutputsLen, 0);

        configParam(kTimbreParam, 0.0f, 10.0f, 0.0f, "Timbre");
        configParam(kTimbreCvAmountParam, -1.0f, 1.0f, 0.0f, "Timbre CV amount");

        configInput(kTimbreCvInput, "Timbre CV");
        configInput(kInput, "Audio");

        configOutput(kOutput, "Audio");

        configBypass(kInput, kOutput);

#ifdef FOLD_DEBUG
        configOutput(kDebug1, "Debug 1");
        configOutput(kDebug2, "Debug 2");
        configOutput(kDebug3, "Debug 3");
        configOutput(kDebug4, "Debug 4");
#endif
    }

    void onSampleRateChange(const SampleRateChangeEvent& e) override {

        oversampleFilter.setParams(
            e.sampleRate,
            bogaudio::dsp::MultimodeFilter::BUTTERWORTH_TYPE,
            12,
            bogaudio::dsp::MultimodeFilter::LOWPASS_MODE,
            e.sampleRate / 4.0f,
            0);
    }

    // This folding algorithm is derived from a permissively licensed
    // Max/MSP patch created by Randy Jones of Madrona Labs.
    float fold(float in, float timbre /* [0,1] */) {

        float ampOffset = timbre * 2.0f + 0.1f;

        // TODO we may not need this if we use sine instead of cosine below.
        float phaseOffset = timbre + 0.25f;

        float out = overdrive.value(in, timbre);
        out = out * ampOffset;

        // TODO switch to wavetable lookup.  Also, we might be able to use
        // the existing SineTable rather than building a CosineTable.
        out = std::cosf(kTwoPi * (out + phaseOffset));

        return overdrive.value(out, timbre);
    }

    float oversampleFold(float in, float timbre /* [0,1] */) {
        float out = oversampleFilter.next(in);
        //out = fold(out, timbre);
        //out = oversampleFilter.next(in);
        return out;
    }

    void process(const ProcessArgs& args) override {
        if (!outputs[kOutput].isConnected()) {
            return;
        }

        float pTimbre = params[kTimbreParam].getValue() / 10.0f;
        float pTimbreCvAmount = params[kTimbreCvAmountParam].getValue();

        int channels = std::max(inputs[kInput].getChannels(), 1);

        for (int ch = 0; ch < channels; ch++) {

            float inTimbreCv = inputs[kTimbreCvInput].getPolyVoltage(ch);
            float timbre = pTimbre + inTimbreCv * pTimbreCvAmount;

            float in = inputs[kInput].getPolyVoltage(ch) / 5.0f;

            float out = oversampleFold(in, timbre);

            outputs[kOutput].setVoltage(out * 5.0f, ch);
        }
        outputs[kOutput].setChannels(channels);
    }
};

struct FOLDWidget : ModuleWidget {
    FOLDWidget(FOLD* module) {
        setModule(module);
        setPanel(createPanel(asset::plugin(pluginInstance, "res/FOLD.svg")));

        // addChild(createWidget<ScrewSilver>(Vec(15, 0)));
        // addChild(createWidget<ScrewSilver>(Vec(15, 365)));
        // addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 0)));
        // addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 365)));
        addChild(createWidget<ScrewSilver>(Vec(0, 0)));
        addChild(createWidget<ScrewSilver>(Vec(box.size.x - 15, 365)));

        addParam(createParamCentered<MKnob32>(Vec(22.5, 78), module, FOLD::kTimbreParam));

        addParam(createParamCentered<MKnob18>(Vec(22.5, 120), module, FOLD::kTimbreCvAmountParam));
        addInput(createInputCentered<MPort>(Vec(22.5, 162), module, FOLD::kTimbreCvInput));
        addInput(createInputCentered<MPort>(Vec(22.5, 278), module, FOLD::kInput));
        addOutput(createOutputCentered<MPort>(Vec(22.5, 320), module, FOLD::kOutput));

#ifdef FOLD_DEBUG
        addOutput(createOutputCentered<MPort>(Vec(12, 12), module, FOLD::kDebug1));
        addOutput(createOutputCentered<MPort>(Vec(12, 36), module, FOLD::kDebug2));
        addOutput(createOutputCentered<MPort>(Vec(12, 60), module, FOLD::kDebug3));
        addOutput(createOutputCentered<MPort>(Vec(12, 84), module, FOLD::kDebug4));
#endif
    }
};

Model* modelFOLD = createModel<FOLD, FOLDWidget>("FOLD");
