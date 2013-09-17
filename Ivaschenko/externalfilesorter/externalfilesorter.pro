QMAKE_CXXFLAGS += -std=gnu++0x

QT -= core gui
CONFIG += console
TEMPLATE = app

HEADERS += \
    binaryfilereader.h \
    abstractreader.h \
    inputstreamreader.h \
    abstractwriter.h \
    outputstreamwriter.h \
    binaryfilewriter.h

SOURCES += \
    test.cpp
