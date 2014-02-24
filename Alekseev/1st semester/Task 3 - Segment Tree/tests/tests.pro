TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(../src/SegmentTree.pri)

QMAKE_CXXFLAGS += -std=gnu++0x

GTEST = D:\Dropbox\gtest

DEFINES += GTEST_LINKED_AS_SHARED_LIBRARY=1
LIBS += -L$$GTEST/lib -lgtest
INCLUDEPATH += $$GTEST/include/

SOURCES += gtest_main.cc \
    AdditionAssignmentTest.cpp \
    AssignmentTest.cpp \
    AdditionTest.cpp \
    InternalCallsTest.cpp

HEADERS += \ 
    dummystructure.h
