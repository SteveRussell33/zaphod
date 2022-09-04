#include "dsp.hpp"
#include "plugin.hpp"
#include "widgets.hpp"

// define ODRV_DEBUG

struct ODRV : Module {

    Overdrive overdrive;

    enum ParamId {
        kDriveParam,
        kDriveCvAmountParam,

        kParamsLen
    };

    enum InputId {
        kDriveCvInput,
        kSatInput,

        kInputsLen
    };

    enum OutputId {
        kSatOutput,

#ifdef ODRV_DEBUG
        kDebug1,
        kDebug2,
        kDebug3,
        kDebug4,
#endif
        kOutputsLen
    };

    ODRV() {
        config(kParamsLen, kInputsLen, kOutputsLen, 0);

        configParam(kDriveParam, 0.0f, 10.0f, 0.0f, "Drive");
        configParam(kDriveCvAmountParam, -1.0f, 1.0f, 0.0f, "Drive CV amount");

        configInput(kDriveCvInput, "Drive CV");
        configInput(kSatInput, "Audio");

        configOutput(kSatOutput, "Audio");

        configBypass(kSatInput, kSatOutput);

#ifdef ODRV_DEBUG
        configOutput(kDebug1, "Debug 1");
        configOutput(kDebug2, "Debug 2");
        configOutput(kDebug3, "Debug 3");
        configOutput(kDebug4, "Debug 4");
#endif
    }

    void process(const ProcessArgs& args) override {
        if (!outputs[kSatOutput].isConnected()) {
            return;
        }

        float pDrive = params[kDriveParam].getValue() / 10.0f;
        float pDriveCvAmount = params[kDriveCvAmountParam].getValue();

        int channels = std::max(inputs[kSatInput].getChannels(), 1);

        for (int ch = 0; ch < channels; ch++) {

            float inDriveCv = inputs[kDriveCvInput].getPolyVoltage(ch);
            float drive = pDrive + inDriveCv * pDriveCvAmount;

            float input = inputs[kSatInput].getPolyVoltage(ch) / 5.0f;

            float output = overdrive.value(input, drive);

            outputs[kSatOutput].setVoltage(output * 5.0f, ch);
        }
        outputs[kSatOutput].setChannels(channels);
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
        addInput(createInputCentered<MPort>(Vec(22.5, 278), module, ODRV::kSatInput));
        addOutput(createOutputCentered<MPort>(Vec(22.5, 320), module, ODRV::kSatOutput));

#ifdef ODRV_DEBUG
        addOutput(createOutputCentered<MPort>(Vec(12, 12), module, ODRV::kDebug1));
        addOutput(createOutputCentered<MPort>(Vec(12, 36), module, ODRV::kDebug2));
        addOutput(createOutputCentered<MPort>(Vec(12, 60), module, ODRV::kDebug3));
        addOutput(createOutputCentered<MPort>(Vec(12, 84), module, ODRV::kDebug4));
#endif
    }
};

Model* modelODRV = createModel<ODRV, ODRVWidget>("ODRV");
