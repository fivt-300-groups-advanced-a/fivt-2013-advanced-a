TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=gnu++0x

unix:{
LIBS += -lpthread
QMAKE_LIBS += /usr/local/lib/libgtest.a
}

win32:{
GTEST = D:\Dropbox\gtest
DEFINES += GTEST_LINKED_AS_SHARED_LIBRARY=1
LIBS += -L$$GTEST/lib -lgtest
INCLUDEPATH += $$GTEST/include/
}

include(../src/EditDistance.pri)

SOURCES += \
    gtest_main.cc \
    manualTests.cpp \
    fancyOutput.cpp \
    randomTests.cpp

HEADERS += \
    utils.h \
    informaticsTest.h
