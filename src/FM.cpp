#include "plugin.hpp"
#include "widgets.hpp"
#include "dsp.hpp"

#define ZPH_DEBUG

struct FM : Module {

    enum ParamId {
        RATIO_PARAM,
        RATIO_STEP_PARAM,
        OFFSET_PARAM,

        RATIO_CV_PARAM,
        OFFSET_CV_PARAM,

        PARAMS_LEN
    };

    enum InputId {
        RATIO_CV_INPUT,
        OFFSET_CV_INPUT,

        CAR_VOCT_INPUT,

        INPUTS_LEN
    };

    enum OutputId {
        MOD_VOCT_OUTPUT,
#ifdef ZPH_DEBUG
        DBG1_OUTPUT,
        DBG2_OUTPUT,
        DBG3_OUTPUT,
        DBG4_OUTPUT,
#endif
        OUTPUTS_LEN
    };

    FM() {
        config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, 0);

        configParam(RATIO_PARAM, 0.01f, 10.0f, 1.0f, "Ratio");
        configParam(RATIO_STEP_PARAM, 0.0f, 1.0f, 0.0f, "Ratio Step Mode");
        configParam(OFFSET_PARAM, -5.0f, 5.0f, 0.0f, "Offset");

        configParam(RATIO_CV_PARAM, -1.0f, 1.0f, 0.0f, "Ratio CV amount");
        configParam(OFFSET_CV_PARAM, -1.0f, 1.0f, 0.0f, "Offset CV amount");

        configInput(RATIO_CV_INPUT, "Ratio CV");
        configInput(OFFSET_CV_INPUT, "Offset CV");

        configInput(CAR_VOCT_INPUT, "Carrier V/Oct");

        configOutput(MOD_VOCT_OUTPUT, "Modulator V/Oct");

#ifdef ZPH_DEBUG
        configOutput(DBG1_OUTPUT, "Debug 1");
        configOutput(DBG2_OUTPUT, "Debug 2");
        configOutput(DBG3_OUTPUT, "Debug 3");
        configOutput(DBG4_OUTPUT, "Debug 4");
#endif
    }

    inline bool active() {
        return outputs[MOD_VOCT_OUTPUT].isConnected();
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

    void process(const ProcessArgs& args) override {
        if (!active()) return;

        float ratio = params[RATIO_PARAM].getValue();
        float ratioCV = calculateCV(RATIO_CV_INPUT, RATIO_CV_PARAM);
        ratio = clamp(ratio + ratioCV, 0.01f, 10.0f);

        bool isRatioStep = params[RATIO_STEP_PARAM].getValue() < 0.05f;
        if (isRatioStep) {
            ratio = applyRatioStep(ratio);
        }

        float offset = params[OFFSET_PARAM].getValue();
        float offsetCV = calculateCV(OFFSET_CV_INPUT, OFFSET_CV_PARAM);
        offset = clamp(offset + offsetCV, -5.0f, 5.0f) * 40.0f; // -200Hz to200 Hz

        // TODO getPolyVoltage
        float inFreq = voctToFreq(inputs[CAR_VOCT_INPUT].getVoltage());
        float outFreq = inFreq * ratio + offset;
        outputs[MOD_VOCT_OUTPUT].setVoltage(freqToVoct(outFreq));

#ifdef ZPH_DEBUG
        outputs[DBG1_OUTPUT].setVoltage(ratio);
        outputs[DBG2_OUTPUT].setVoltage(ratioCV);
        outputs[DBG3_OUTPUT].setVoltage(isRatioStep);
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
        addParam(createParamCentered<ZphKnob50> (Vec(37.5,  82), module, FM::RATIO_PARAM));
        addParam(createParamCentered<ZphHSwitch>(Vec(37.5, 122), module, FM::RATIO_STEP_PARAM));
        addParam(createParamCentered<ZphKnob40> (Vec(37.5, 180), module, FM::OFFSET_PARAM));

        // row 1
        addParam(createParamCentered<ZphKnob18>(Vec(22, 236), module, FM::RATIO_CV_PARAM));
        addParam(createParamCentered<ZphKnob18>(Vec(53, 236), module, FM::OFFSET_CV_PARAM));

        // row 2
        addInput(createInputCentered<ZphPort>(Vec(22, 278), module, FM::RATIO_CV_INPUT));
        addInput(createInputCentered<ZphPort>(Vec(53, 278), module, FM::OFFSET_CV_INPUT));

        // row 3
        addInput (createInputCentered<ZphPort> (Vec(22, 320), module, FM::CAR_VOCT_INPUT));
        addOutput(createOutputCentered<ZphPort>(Vec(53, 320), module, FM::MOD_VOCT_OUTPUT));

#ifdef ZPH_DEBUG
        addOutput(createOutputCentered<ZphPort>(Vec(12, 12), module, FM::DBG1_OUTPUT));
        addOutput(createOutputCentered<ZphPort>(Vec(12, 36), module, FM::DBG2_OUTPUT));
        addOutput(createOutputCentered<ZphPort>(Vec(12, 60), module, FM::DBG3_OUTPUT));
        addOutput(createOutputCentered<ZphPort>(Vec(12, 84), module, FM::DBG4_OUTPUT));
#endif
    }
};

Model* modelFM = createModel<FM, FMWidget>("FM");
