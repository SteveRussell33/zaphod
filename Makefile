
SOURCES = $(wildcard src/*.cpp lib/earlevel/*.cpp)

DISTRIBUTABLES += $(wildcard LICENSE*) res

RACK_DIR ?= ../..
include $(RACK_DIR)/plugin.mk

CXXFLAGS += -Isrc -Isrc/dsp -Ilib/earlevel
