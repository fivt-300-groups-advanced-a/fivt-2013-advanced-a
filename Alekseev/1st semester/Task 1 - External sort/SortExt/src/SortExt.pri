TEMPLATE = app
CONFIG += console
CONFIG -= qt

QMAKE_CXXFLAGS += -std=gnu++0x

SOURCES +=

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/io/istreamreader.h \
    $$PWD/io/ostreamwriter.h \
    $$PWD/io/optimalstreamio.h \
    $$PWD/io/fstreamqueue.h \
    $$PWD/io/rawistreamreader.h \
    $$PWD/io/rawostreamwriter.h \
    $$PWD/externalsort.h \
    $$PWD/stdsorter.h
