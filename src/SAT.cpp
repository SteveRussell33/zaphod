#include "plugin.hpp"
#include "widgets.hpp"
#include "dsp.hpp"

//define SAT_DEBUG

struct SAT : Module {

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

#ifdef SAT_DEBUG
        kDebug1,
        kDebug2,
        kDebug3,
        kDebug4,
#endif
        kOutputsLen
    };

    SAT() {
        config(kParamsLen, kInputsLen, kOutputsLen, 0);

        configParam(kDriveParam, 0.01f, 10.0f, 1.0f, "Drive");
        configParam(kDriveCvAmountParam, -1.0f, 1.0f, 0.0f, "Drive CV amount");

        configInput(kDriveCvInput, "Drive CV");
        configInput(kSatInput, "Audio");

        configOutput(kSatOutput, "Audio");

        configBypass(kSatInput, kSatOutput);

#ifdef SAT_DEBUG
        configOutput(kDebug1, "Debug 1");
        configOutput(kDebug2, "Debug 2");
        configOutput(kDebug3, "Debug 3");
        configOutput(kDebug4, "Debug 4");
#endif
    }

    void process(const ProcessArgs& args) override {
    }
};

struct SATWidget : ModuleWidget {
    SATWidget(SAT* module) {
        setModule(module);
        setPanel(createPanel(asset::plugin(pluginInstance, "res/SAT.svg")));

        //addChild(createWidget<ScrewSilver>(Vec(15, 0)));
        //addChild(createWidget<ScrewSilver>(Vec(15, 365)));
        //addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 0)));
        //addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 365)));
        addChild(createWidget<ScrewSilver>(Vec(0, 0)));
        addChild(createWidget<ScrewSilver>(Vec(box.size.x - 15, 365)));

        // knobs and switches
        addParam(createParamCentered<MKnob30> (Vec(22.5,  78), module, SAT::kDriveParam));

        // row 1
        addParam(createParamCentered<MKnob18>(Vec(22.5, 120), module, SAT::kDriveCvAmountParam));

        // row 2
        addInput(createInputCentered<MPort>(Vec(22.5, 162), module, SAT::kDriveCvInput));

        // row 3
        addInput (createInputCentered<MPort> (Vec(22.5, 278), module, SAT::kSatInput));

        // row 3
        addOutput(createOutputCentered<MPort>(Vec(22.5, 320), module, SAT::kSatOutput));

#ifdef SAT_DEBUG
        addOutput(createOutputCentered<MPort>(Vec(12, 12), module, SAT::kDebug1));
        addOutput(createOutputCentered<MPort>(Vec(12, 36), module, SAT::kDebug2));
        addOutput(createOutputCentered<MPort>(Vec(12, 60), module, SAT::kDebug3));
        addOutput(createOutputCentered<MPort>(Vec(12, 84), module, SAT::kDebug4));
#endif
    }
};

Model* modelSAT = createModel<SAT, SATWidget>("SAT");
