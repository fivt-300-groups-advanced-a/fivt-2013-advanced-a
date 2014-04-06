include(gtest.pri)

QMAKE_CXXFLAGS += -std=gnu++0x -lgtest -pthread
QMAKE_LIBS += /usr/local/lib/libgtest.a


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
    fabrics/defaultlistbuilder.h
