#include "plugin.hpp"
#include "widgets.hpp"
#include "dsp.hpp"

struct FM : Module {

    enum ParamId {
        RATIO_PARAM,
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

        //DBG1_OUTPUT,
        //DBG2_OUTPUT,
        //DBG3_OUTPUT,
        //DBG4_OUTPUT,

        OUTPUTS_LEN
    };

    FM() {
        config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, 0);

        configParam(RATIO_PARAM, 0.0f, 10.0f, 1.0f, "Ratio");
        configParam(OFFSET_PARAM, -5.0f, 5.0f, 0.0f, "Offset");

        configParam(RATIO_CV_PARAM, -1.0f, 1.0f, 0.0f, "Ratio CV amount");
        configParam(OFFSET_CV_PARAM, -1.0f, 1.0f, 0.0f, "Offset CV amount");

        configInput(RATIO_CV_INPUT, "Ratio CV");
        configInput(OFFSET_CV_INPUT, "Offset CV");

        configInput(CAR_VOCT_INPUT, "Carrier V/Oct");

        configOutput(MOD_VOCT_OUTPUT, "Modulator V/Oct");

        //configOutput(DBG1_OUTPUT, "Debug 1");
        //configOutput(DBG2_OUTPUT, "Debug 2");
        //configOutput(DBG3_OUTPUT, "Debug 3");
        //configOutput(DBG4_OUTPUT, "Debug 4");
    }

    bool active() {
        return outputs[MOD_VOCT_OUTPUT].isConnected();
    }

    void process(const ProcessArgs& args) override {

        float inFreq = voctToFreq(inputs[CAR_VOCT_INPUT].getVoltage());

        float ratio = params[RATIO_PARAM].getValue();
        float offset = params[OFFSET_PARAM].getValue() * 40.0f; // -200Hz to200 Hz
        float outFreq = inFreq * ratio + offset;

        outputs[MOD_VOCT_OUTPUT].setVoltage(freqToVoct(outFreq));

        //outputs[DBG1_OUTPUT].setVoltage(inFreq/1000.0f);
        //outputs[DBG2_OUTPUT].setVoltage(outFreq/1000.0f);
        //outputs[DBG3_OUTPUT].setVoltage(offset/1000.0f);
        //outputs[DBG4_OUTPUT].setVoltage(d/1000.0f);
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

        // knobs
        addParam(createParamCentered<ZaphodKnob50>(Vec(37.5,  82), module, FM::RATIO_PARAM));
        addParam(createParamCentered<ZaphodKnob40>(Vec(37.5, 150), module, FM::OFFSET_PARAM));

        // row 1
        addParam(createParamCentered<ZaphodKnob18>(Vec(22, 236), module, FM::RATIO_CV_PARAM));
        addParam(createParamCentered<ZaphodKnob18>(Vec(53, 236), module, FM::OFFSET_CV_PARAM));

        // row 2
        addInput(createInputCentered<ZaphodPort>(Vec(22, 278), module, FM::RATIO_CV_INPUT));
        addInput(createInputCentered<ZaphodPort>(Vec(53, 278), module, FM::OFFSET_CV_INPUT));

        // row 3
        addInput (createInputCentered<ZaphodPort> (Vec(22, 320), module, FM::CAR_VOCT_INPUT));
        addOutput(createOutputCentered<ZaphodPort>(Vec(53, 320), module, FM::MOD_VOCT_OUTPUT));

        // debug
        //addOutput(createOutputCentered<ZaphodPort>(Vec(12, 12), module, FM::DBG1_OUTPUT));
        //addOutput(createOutputCentered<ZaphodPort>(Vec(12, 36), module, FM::DBG2_OUTPUT));
        //addOutput(createOutputCentered<ZaphodPort>(Vec(12, 60), module, FM::DBG3_OUTPUT));
        //addOutput(createOutputCentered<ZaphodPort>(Vec(12, 84), module, FM::DBG4_OUTPUT));
    }
};

Model* modelFM = createModel<FM, FMWidget>("FM");
