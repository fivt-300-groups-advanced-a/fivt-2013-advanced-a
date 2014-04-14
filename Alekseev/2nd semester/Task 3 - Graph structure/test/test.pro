TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=gnu++0x

include(gtest-lib.pri)
include(../src/graph.pri)


HEADERS += \
    utils.h

SOURCES += \
    adjacencylist-test.cpp \
    adjacencybitset-test.cpp \
    emptyadjacency-test.cpp \
    singleadjacency-test.cpp
