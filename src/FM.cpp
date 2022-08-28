#include "plugin.hpp"
#include "widgets.hpp"

struct FM : Module {
    enum ParamId {
        //PITCH_PARAM,
        PARAMS_LEN
    };
    enum InputId {
        CV_INPUT,
        INPUTS_LEN
    };
    enum OutputId {
        //SINE_OUTPUT,
        OUTPUTS_LEN
    };
    enum LightId {
        //BLINK_LIGHT,
        LIGHTS_LEN
    };

    FM() {
        config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
        //configParam(PITCH_PARAM, 0.f, 1.f, 0.f, "");
        configInput(CV_INPUT, "CV Input");
        //configOutput(SINE_OUTPUT, "");
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

        addInput(createInputCentered<ZaphodPort24>(Vec(30, 266), module, FM::CV_INPUT));
    }
};


Model* modelFM = createModel<FM, FMWidget>("FM");
