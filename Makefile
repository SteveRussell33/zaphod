
LIB_SOURCES = $(wildcard lib/bogaudio/BogaudioModules/src/dsp/*.cpp lib/bogaudio/BogaudioModules/src/dsp/filters/*.cpp lib/earlevel/*.cpp)
SOURCES = $(wildcard src/*.cpp) $(LIB_SOURCES)

DISTRIBUTABLES += $(wildcard LICENSE*) res

RACK_DIR ?= ../..
include $(RACK_DIR)/plugin.mk

CXXFLAGS += -Isrc -Ilib/bogaudio/BogaudioModules/src/dsp -Ilib/bogaudio/BogaudioModules/lib -Ilib/earlevel
