TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=gnu++0x

GTEST = D:\Dropbox\gtest

DEFINES += GTEST_LINKED_AS_SHARED_LIBRARY=1
LIBS += -L$$GTEST/lib -lgtest
INCLUDEPATH += $$GTEST/include/

include(../src/MaxExpression.pri)

SOURCES += \
    gtest_main.cc \
    manualtests.cpp \
    stresstests.cpp

HEADERS += \
    dummy.h
