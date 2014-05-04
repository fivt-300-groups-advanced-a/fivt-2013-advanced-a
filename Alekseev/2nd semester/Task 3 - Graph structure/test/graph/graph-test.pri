include(../../src/graph.pri)

HEADERS += \
    $$PWD/utils.h

SOURCES += \
    $$PWD/adjacencylist-test.cpp \
    $$PWD/adjacencybitset-test.cpp \
    $$PWD/emptyadjacency-test.cpp \
    $$PWD/singleadjacency-test.cpp \
    $$PWD/graph-test.cpp \
    $$PWD/defaultadjacencybuilder-test.cpp \
    $$PWD/utils.cpp
