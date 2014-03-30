include(gtest.pri)

QMAKE_CXXFLAGS += -std=gnu++0x -lgtest -pthread
QMAKE_LIBS += /usr/local/lib/libgtest.a


SOURCES += main.cpp

HEADERS += \
    model/graph.h \
    model/bitsetincidencelist.h \
    model/setincidencelist.h \
    model/incidencelist.h \
    model/vectorincidencelist.h
