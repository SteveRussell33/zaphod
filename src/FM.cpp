#include "plugin.hpp"
#include "widgets.hpp"

struct FM : Module {
    enum ParamId {
        RATIO_PARAM,
        RATIO_CV_PARAM,
        OFFSET_PARAM,
        OFFSET_CV_PARAM,
        PARAMS_LEN
    };
    enum InputId {
        RATIO_INPUT,
        OFFSET_INPUT,
        FM_INPUT,
        INPUTS_LEN
    };
    enum OutputId {
        FM_OUTPUT,
        OUTPUTS_LEN
    };

    FM() {
        config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, 0);

        configParam(RATIO_PARAM, 0.f, 1.f, 0.f, "Ratio");
        configParam(RATIO_CV_PARAM, -1.0, 1.f, 0.f, "Ratio CV amount");
        configInput(RATIO_INPUT, "Ratio CV");

        configParam(OFFSET_PARAM, 0.f, 1.f, 0.f, "Offset");
        configParam(OFFSET_CV_PARAM, -1.0, 1.f, 0.f, "Offset CV amount");
        configInput(OFFSET_INPUT, "Offset CV");

        configInput(FM_INPUT, "V/Oct");
        configOutput(FM_OUTPUT, "V/Oct");
    }

    bool active() {
        return outputs[FM_OUTPUT].isConnected();
    }

    void process(const ProcessArgs& args) override {
        float voct = inputs[FM_INPUT].getVoltage();
        outputs[FM_OUTPUT].setVoltage(voct);
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
        addParam(createParamCentered<ZaphodKnob45>(Vec(37.5, 80), module, FM::RATIO_PARAM));
        addParam(createParamCentered<ZaphodKnob45>(Vec(37.5, 160), module, FM::OFFSET_PARAM));

        // left column
        addParam(createParamCentered<ZaphodKnob18>(Vec(19.5, 236), module, FM::RATIO_CV_PARAM));
        addInput(createInputCentered<ZaphodPort>  (Vec(19.5, 278), module, FM::RATIO_INPUT));
        addInput(createInputCentered<ZaphodPort>  (Vec(19.5, 320), module, FM::FM_INPUT));

        // right column
        addParam(createParamCentered<ZaphodKnob18>(Vec(55.5, 236), module, FM::OFFSET_CV_PARAM));
        addInput(createInputCentered<ZaphodPort>  (Vec(55.5, 278), module, FM::OFFSET_INPUT));
        addOutput(createOutputCentered<ZaphodPort>(Vec(55.5, 320), module, FM::FM_OUTPUT));
    }
};

Model* modelFM = createModel<FM, FMWidget>("FM");
