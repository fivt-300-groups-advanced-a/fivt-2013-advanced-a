TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(../src/MeldableHeap.pri)

QMAKE_CXXFLAGS += -std=gnu++0x

GTEST = D:\Dropbox\gtest

DEFINES += GTEST_LINKED_AS_SHARED_LIBRARY=1
LIBS += -L$$GTEST/lib -lgtest
INCLUDEPATH += $$GTEST/include/

SOURCES += gtest_main.cc \
    leftistheap-integration-stress-test.cpp

HEADERS += \
    testutils.h \
    dijkstra.h \
    sort.h
