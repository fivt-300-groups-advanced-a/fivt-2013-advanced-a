TEMPLATE = app
CONFIG += console
CONFIG -= qt

QMAKE_CXXFLAGS += -std=gnu++0x

SOURCES +=

HEADERS += \
    src/externalsort.h \
    src/istreamreader.h \
    src/ostreamwriter.h \
    src/optimalstreamio.h \
    src/fstreamqueue.h
