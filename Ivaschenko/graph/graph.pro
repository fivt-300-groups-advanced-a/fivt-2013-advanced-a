include(gtest.pri)

LIBS += -lpthread
QMAKE_CXXFLAGS += -std=gnu++0x -lgtest
QMAKE_LIBS += /usr/local/lib/libgtest.a
CONFIG -= qt


SOURCES += main.cpp

HEADERS += \
    graph.h \
    lists/bitsetincidencelist.h \
    lists/setincidencelist.h \
    lists/incidencelist.h \
    lists/vectorincidencelist.h \
    lists/singlevertexlist.h \
    lists/emptyincidencelist.h \
    iterators/incidencelistiterator.h \
    iterators/stdincidencelistiterator.h \
    iterators/adjacencymatrixiterator.h \
    lists/consecutiveincidencelist.h \
    fabrics/defaultlistbuilder.h \
    algo/strongconnectivity.h
