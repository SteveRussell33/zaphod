#include "plugin.hpp"
#include "widgets.hpp"
#include "dsp.hpp"

//define FM_DEBUG

struct FM : Module {

    enum ParamId {
        kRatioParam,
        kRatioStepParam,
        kRatioCvParam,

        kOffsetParam,
        kOffsetCvParam,

        kParamsLen
    };

    enum InputId {
        kRatioCvInput,
        kOffsetCvInput,

        kCarrierPitchInput,

        kInputsLen
    };

    enum OutputId {
        kModulatorPitchOutput,

#ifdef FM_DEBUG
        kDebug1,
        kDebug2,
        kDebug3,
        kDebug4,
#endif
        kOutputsLen
    };

    FM() {
        config(kParamsLen, kInputsLen, kOutputsLen, 0);

        configParam(kRatioParam, 0.01f, 10.0f, 1.0f, "Ratio");
		configSwitch(kRatioStepParam, 0.f, 1.f, 0.f, "Ratio Step mode", {"On", "Off"});
        configParam(kOffsetParam, -5.0f, 5.0f, 0.0f, "Offset", " Hz", 0.0f, 40.0f);
    
        //----------------------------------

        configParam(kRatioCvParam, -1.0f, 1.0f, 0.0f, "Ratio CV amount");
        configParam(kOffsetCvParam, -1.0f, 1.0f, 0.0f, "Offset CV amount");
        configInput(kRatioCvInput, "Ratio CV");
        configInput(kOffsetCvInput, "Offset CV");

        //----------------------------------

        configInput(kCarrierPitchInput, "Carrier V/Oct");
        configOutput(kModulatorPitchOutput, "Modulator V/Oct");

#ifdef FM_DEBUG
        configOutput(kDebug1, "Debug 1");
        configOutput(kDebug2, "Debug 2");
        configOutput(kDebug3, "Debug 3");
        configOutput(kDebug4, "Debug 4");
#endif
    }

    inline float calculateCV(int inputID, int paramID) {
        return (inputs[inputID].isConnected()) ?
            inputs[inputID].getVoltage() * params[paramID].getValue() :
            0.0f;
    }

    float applyRatioStep(float ratio) {
        if      (ratio < 0.125f) return 0.01f;
        else if (ratio < 0.375f) return 0.25f;
        else if (ratio < 0.75f)  return 0.5f;
        else                     return round(ratio);
    }

    inline bool active() {
        return outputs[kModulatorPitchOutput].isConnected();
    }

    void process(const ProcessArgs& args) override {
        if (!active()) return;

        float ratio = params[kRatioParam].getValue();
        float ratioCV = calculateCV(kRatioCvInput, kRatioCvParam);
        ratio = clamp(ratio + ratioCV, 0.01f, 10.0f);

        bool isRatioStep = params[kRatioStepParam].getValue() < 0.05f;
        if (isRatioStep) {
            ratio = applyRatioStep(ratio);
        }

        float offset = params[kOffsetParam].getValue();
        float offsetCV = calculateCV(kOffsetCvInput, kOffsetCvParam);
        offset = clamp(offset + offsetCV, -5.0f, 5.0f) * 40.0f; // -200Hz to200 Hz

        float inFreq = pitchToFreq(inputs[kCarrierPitchInput].getVoltage());
        float outFreq = inFreq * ratio + offset;
        outputs[kModulatorPitchOutput].setVoltage(freqToPitch(outFreq));

#ifdef FM_DEBUG
        outputs[kDebug1].setVoltage(ratio);
        outputs[kDebug2].setVoltage(offset/1000.0);
#endif
    }
};

struct FMWidget : ModuleWidget {
    FMWidget(FM* module) {
        setModule(module);
        setPanel(createPanel(asset::plugin(pluginInstance, "res/FM.svg")));

        addChild(createWidget<ScrewSilver>(Vec(15, 0)));
        addChild(createWidget<ScrewSilver>(Vec(15, 365)));
        addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 0)));
        addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 365)));

        // knobs and switches
        addParam(createParamCentered<ZphKnob50> (Vec(37.5,  82), module, FM::kRatioParam));
        addParam(createParamCentered<ZphHSwitch>(Vec(37.5, 122), module, FM::kRatioStepParam));
        addParam(createParamCentered<ZphKnob40> (Vec(37.5, 180), module, FM::kOffsetParam));

        // row 1
        addParam(createParamCentered<ZphKnob18>(Vec(22, 236), module, FM::kRatioCvParam));
        addParam(createParamCentered<ZphKnob18>(Vec(53, 236), module, FM::kOffsetCvParam));

        // row 2
        addInput(createInputCentered<ZphPort>(Vec(22, 278), module, FM::kRatioCvInput));
        addInput(createInputCentered<ZphPort>(Vec(53, 278), module, FM::kOffsetCvInput));

        // row 3
        addInput (createInputCentered<ZphPort> (Vec(22, 320), module, FM::kCarrierPitchInput));
        addOutput(createOutputCentered<ZphPort>(Vec(53, 320), module, FM::kModulatorPitchOutput));

#ifdef FM_DEBUG
        addOutput(createOutputCentered<ZphPort>(Vec(12, 12), module, FM::kDebug1));
        addOutput(createOutputCentered<ZphPort>(Vec(12, 36), module, FM::kDebug2));
        addOutput(createOutputCentered<ZphPort>(Vec(12, 60), module, FM::kDebug3));
        addOutput(createOutputCentered<ZphPort>(Vec(12, 84), module, FM::kDebug4));
#endif
    }
};

Model* modelFM = createModel<FM, FMWidget>("FM");
