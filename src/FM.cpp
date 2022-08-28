#include "plugin.hpp"
#include "widgets.hpp"

struct FM : Module {
    enum ParamId {
        //PITCH_PARAM,
        PARAMS_LEN
    };
    enum InputId {
        FM_INPUT,
        INPUTS_LEN
    };
    enum OutputId {
        FM_OUTPUT,
        OUTPUTS_LEN
    };

    FM() {
        config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, 0);
        //configParam(PITCH_PARAM, 0.f, 1.f, 0.f, "");
        configInput(FM_INPUT, "V/Oct");
        configOutput(FM_OUTPUT, "V/Oct");
    }

    void process(const ProcessArgs& args) override {
    }
};


struct FMWidget : ModuleWidget {
    FMWidget(FM* module) {
        setModule(module);
        setPanel(createPanel(asset::plugin(pluginInstance, "res/FM.svg")));

        addChild(createWidget<ScrewSilver>(Vec(0, 0)));
        addChild(createWidget<ScrewSilver>(Vec(box.size.x - 15, 365)));

        addInput(createInputCentered<ZaphodPort24>(Vec(30, 266), module, FM::FM_INPUT));
        addOutput(createOutputCentered<ZaphodPort24>(Vec(30, 310), module, FM::FM_OUTPUT));
    }
};


Model* modelFM = createModel<FM, FMWidget>("FM");
