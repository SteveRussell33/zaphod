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
        PITCH_INPUT,
        INPUTS_LEN
    };

    enum OutputId {
        MODULATOR_PITCH_OUTPUT,
        CARRIER_FM_OUTPUT,
        OUTPUTS_LEN
    };

    FM() {
        config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, 0);

        configParam(RATIO_PARAM, 0.0, 10.0, 1.0, "Ratio");
        configParam(OFFSET_PARAM, 0.0, 1.0, 0.0, "Offset");
        configParam(INDEX_PARAM, 0.0, 10.0, 0.0, "Index");

        configParam(RATIO_CV_PARAM, -1.0, 1.0, 0.0, "Ratio CV amount");
        configParam(OFFSET_CV_PARAM, -1.0, 1.0, 0.0, "Offset CV amount");
        configParam(INDEX_CV_PARAM, -1.0, 1.0, 0.0, "Index CV amount");

        configInput(RATIO_CV_INPUT, "Ratio CV");
        configInput(OFFSET_CV_INPUT, "Offset CV");
        configInput(INDEX_CV_INPUT, "Index CV");

        configInput(PITCH_INPUT, "V/Oct");
        configOutput(MODULATOR_PITCH_OUTPUT, "Modulator V/Oct");
        configOutput(CARRIER_FM_OUTPUT, "Carrier FM");
    }

    bool active() {
        return 
            outputs[MODULATOR_PITCH_OUTPUT].isConnected() || 
            outputs[CARRIER_FM_OUTPUT].isConnected();
    }

    void process(const ProcessArgs& args) override {

        //float pin = inputs[PITCH_INPUT].getVoltage();
        //float freq = pitchToFreq(pin);

        //float ratio = params[RATIO_PARAM].getValue();
        //freq = freq * ratio;

        //float pout = freqToPitch(freq);
        //outputs[MODULATOR_PITCH_OUTPUT].setVoltage(pout);
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

        // big knobs
        addParam(createParamCentered<ZaphodKnob50>(Vec(60,  82), module, FM::RATIO_PARAM));
        addParam(createParamCentered<ZaphodKnob40>(Vec(33, 164), module, FM::OFFSET_PARAM));
        addParam(createParamCentered<ZaphodKnob40>(Vec(87, 164), module, FM::INDEX_PARAM));

        // left column
        addParam(createParamCentered<ZaphodKnob18>(Vec(24, 236), module, FM::RATIO_CV_PARAM));
        addInput(createInputCentered<ZaphodPort>  (Vec(24, 278), module, FM::RATIO_CV_INPUT));
        addInput(createInputCentered<ZaphodPort>  (Vec(24, 320), module, FM::PITCH_INPUT));

        // right column
        addParam(createParamCentered<ZaphodKnob18>(Vec(60, 236), module, FM::OFFSET_CV_PARAM));
        addInput(createInputCentered<ZaphodPort>  (Vec(60, 278), module, FM::OFFSET_CV_INPUT));
        addOutput(createOutputCentered<ZaphodPort>(Vec(60, 320), module, FM::MODULATOR_PITCH_OUTPUT));

        // right column
        addParam(createParamCentered<ZaphodKnob18>(Vec(96, 236), module, FM::INDEX_CV_PARAM));
        addInput(createInputCentered<ZaphodPort>  (Vec(96, 278), module, FM::INDEX_CV_INPUT));
        addOutput(createOutputCentered<ZaphodPort>(Vec(96, 320), module, FM::CARRIER_FM_OUTPUT));
    }
};

Model* modelFM = createModel<FM, FMWidget>("FM");
