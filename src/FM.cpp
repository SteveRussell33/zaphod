#include "plugin.hpp"
#include "widgets.hpp"
#include "dsp.hpp"

struct FM : Module {

    enum ParamId {
        RATIO_PARAM,
        OFFSET_PARAM,
        INDEX_PARAM,

        RATIO_CV_PARAM,
        OFFSET_CV_PARAM,
        INDEX_CV_PARAM,

        PARAMS_LEN
    };

    enum InputId {
        RATIO_CV_INPUT,
        OFFSET_CV_INPUT,
        INDEX_CV_INPUT,

        VOCT_INPUT,
        MOD_AUDIO_INPUT,

        INPUTS_LEN
    };

    enum OutputId {
        MOD_VOCT_OUTPUT,
        CAR_VOCT_OUTPUT,
        OUTPUTS_LEN
    };

    FM() {
        config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, 0);

        configParam(RATIO_PARAM, 0.0, 10.0, 1.0, "Ratio");
        configParam(OFFSET_PARAM, -5.0, 5.0, 0.0, "Offset");
        configParam(INDEX_PARAM, 0.0, 10.0, 0.0, "Mod Index");

        configParam(RATIO_CV_PARAM, -1.0, 1.0, 0.0, "Ratio CV amount");
        configParam(OFFSET_CV_PARAM, -1.0, 1.0, 0.0, "Offset CV amount");
        configParam(INDEX_CV_PARAM, -1.0, 1.0, 0.0, "Mod Index CV amount");

        configInput(RATIO_CV_INPUT, "Ratio CV");
        configInput(OFFSET_CV_INPUT, "Offset CV");
        configInput(INDEX_CV_INPUT, "Mod Index CV");

        configInput(VOCT_INPUT, "V/Oct");
        configInput(MOD_AUDIO_INPUT, "Modulator Audio");

        configOutput(MOD_VOCT_OUTPUT, "Modulator V/Oct");
        configOutput(CAR_VOCT_OUTPUT, "Carrier V/Oct");
    }

    bool active() {
        return 
            outputs[MOD_VOCT_OUTPUT].isConnected() || 
            outputs[CAR_VOCT_OUTPUT].isConnected();
    }

    void process(const ProcessArgs& args) override {

        float ratio = params[RATIO_PARAM].getValue();
        float offset = params[OFFSET_PARAM].getValue() * 40; // -200Hz to 200 Hz
        //float index = params[INDEX_PARAM].getValue();

        float baseFreq = voctToFreq(inputs[VOCT_INPUT].getVoltage());

        float ratioFreq = baseFreq * ratio;
        float modFreq = ratioFreq + offset;

        outputs[MOD_VOCT_OUTPUT].setVoltage(freqToVoct(modFreq));
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
        addParam(createParamCentered<ZaphodKnob50>(Vec(60,  82), module, FM::RATIO_PARAM));
        addParam(createParamCentered<ZaphodKnob40>(Vec(33, 150), module, FM::OFFSET_PARAM));
        addParam(createParamCentered<ZaphodKnob40>(Vec(87, 150), module, FM::INDEX_PARAM));

        // row 1
        addParam(createParamCentered<ZaphodKnob18>(Vec(24, 194), module, FM::RATIO_CV_PARAM));
        addParam(createParamCentered<ZaphodKnob18>(Vec(60, 194), module, FM::OFFSET_CV_PARAM));
        addParam(createParamCentered<ZaphodKnob18>(Vec(96, 194), module, FM::INDEX_CV_PARAM));

        // row 2
        addInput(createInputCentered<ZaphodPort>(Vec(24, 236), module, FM::RATIO_CV_INPUT));
        addInput(createInputCentered<ZaphodPort>(Vec(60, 236), module, FM::OFFSET_CV_INPUT));
        addInput(createInputCentered<ZaphodPort>(Vec(96, 236), module, FM::INDEX_CV_INPUT));

        // row 3
        addInput(createInputCentered<ZaphodPort>(Vec(24, 278), module, FM::VOCT_INPUT));
        addInput(createInputCentered<ZaphodPort>(Vec(60, 278), module, FM::MOD_AUDIO_INPUT));

        // row 4
        addOutput(createOutputCentered<ZaphodPort>(Vec(24, 320), module, FM::MOD_VOCT_OUTPUT));
        addOutput(createOutputCentered<ZaphodPort>(Vec(60, 320), module, FM::CAR_VOCT_OUTPUT));
    }
};

Model* modelFM = createModel<FM, FMWidget>("FM");
