#include "dsp.hpp"
#include "oversample.hpp"
#include "plugin.hpp"
#include "widgets.hpp"

// define ODRV_DEBUG

struct ODRV : Module {

    int oversampleFactor = 16;
    Oversample oversample;

    Overdrive overdrive;

    enum ParamId {
        kDriveParam,
        kDriveCvAmountParam,

        kParamsLen
    };

    enum InputId {
        kDriveCvInput,
        kInput,

        kInputsLen
    };

    enum OutputId {
        kOutput,

#ifdef ODRV_DEBUG
        kDebug1,
        kDebug2,
        kDebug3,
        kDebug4,
#endif
        kOutputsLen
    };

    ODRV() {
        oversample.setOversample(oversampleFactor);

        config(kParamsLen, kInputsLen, kOutputsLen, 0);

        configParam(kDriveParam, 0.0f, 10.0f, 0.0f, "Drive");
        configParam(kDriveCvAmountParam, -1.0f, 1.0f, 0.0f, "Drive CV amount");

        configInput(kDriveCvInput, "Drive CV");
        configInput(kInput, "Audio");

        configOutput(kOutput, "Audio");

        configBypass(kInput, kOutput);

#ifdef ODRV_DEBUG
        configOutput(kDebug1, "Debug 1");
        configOutput(kDebug2, "Debug 2");
        configOutput(kDebug3, "Debug 3");
        configOutput(kDebug4, "Debug 4");
#endif
    }

    void onSampleRateChange(const SampleRateChangeEvent& e) override {
        oversample.sampleRateChange(e.sampleRate);
    }

    float oversampleDrive(float in, float drive /* [0,1] */) {

        float buffer[kMaxOversample] = {};
        oversample.up(in, buffer);

        // Process
        for (int i = 0; i < oversampleFactor; i++) {
            buffer[i] = overdrive.process(buffer[i], drive);
        }

        return oversample.down(buffer);
    }

    void process(const ProcessArgs& args) override {
        if (!outputs[kOutput].isConnected()) {
            return;
        }

        float pDrive = params[kDriveParam].getValue() / 10.0f;
        float pDriveCvAmount = params[kDriveCvAmountParam].getValue();

        int channels = std::max(inputs[kInput].getChannels(), 1);

        for (int ch = 0; ch < channels; ch++) {

            float inDriveCv = inputs[kDriveCvInput].getPolyVoltage(ch);
            float drive = pDrive + inDriveCv * pDriveCvAmount;

            float in = inputs[kInput].getPolyVoltage(ch) / 5.0f;

            float out;
            if (oversampleFactor == 1) {
                out = overdrive.process(in, drive);
            } else {
                out = oversampleDrive(in, drive);
            }

            outputs[kOutput].setVoltage(out * 5.0f, ch);
        }
        outputs[kOutput].setChannels(channels);
    }
};

struct ODRVWidget : ModuleWidget {
    ODRVWidget(ODRV* module) {
        setModule(module);
        setPanel(createPanel(asset::plugin(pluginInstance, "res/ODRV.svg")));

        // addChild(createWidget<ScrewSilver>(Vec(15, 0)));
        // addChild(createWidget<ScrewSilver>(Vec(15, 365)));
        // addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 0)));
        // addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 365)));
        addChild(createWidget<ScrewSilver>(Vec(0, 0)));
        addChild(createWidget<ScrewSilver>(Vec(box.size.x - 15, 365)));

        addParam(createParamCentered<MKnob32>(Vec(22.5, 78), module, ODRV::kDriveParam));

        addParam(createParamCentered<MKnob18>(Vec(22.5, 120), module, ODRV::kDriveCvAmountParam));
        addInput(createInputCentered<MPort>(Vec(22.5, 162), module, ODRV::kDriveCvInput));
        addInput(createInputCentered<MPort>(Vec(22.5, 278), module, ODRV::kInput));
        addOutput(createOutputCentered<MPort>(Vec(22.5, 320), module, ODRV::kOutput));

#ifdef ODRV_DEBUG
        addOutput(createOutputCentered<MPort>(Vec(12, 12), module, ODRV::kDebug1));
        addOutput(createOutputCentered<MPort>(Vec(12, 36), module, ODRV::kDebug2));
        addOutput(createOutputCentered<MPort>(Vec(12, 60), module, ODRV::kDebug3));
        addOutput(createOutputCentered<MPort>(Vec(12, 84), module, ODRV::kDebug4));
#endif
    }
};

Model* modelODRV = createModel<ODRV, ODRVWidget>("ODRV");
