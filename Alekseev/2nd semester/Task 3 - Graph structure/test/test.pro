TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=gnu++0x

include(gtest-lib.pri)
include(graph/graph-test.pri)
include(algo/algo-test.pri)

DEFINES += TEST

