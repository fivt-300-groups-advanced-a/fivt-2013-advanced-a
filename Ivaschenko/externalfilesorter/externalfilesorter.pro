QMAKE_CXXFLAGS += -std=gnu++0x

QT -= core gui
CONFIG += console
TEMPLATE = app

HEADERS += \
    io/binaryfilereader.h \
    io/abstractreader.h \
    io/inputstreamreader.h \
    io/abstractwriter.h \
    io/outputstreamwriter.h \
    io/binaryfilewriter.h

SOURCES += \
    test.cpp
