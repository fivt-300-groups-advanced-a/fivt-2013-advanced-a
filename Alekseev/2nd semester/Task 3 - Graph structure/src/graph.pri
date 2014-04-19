INCLUDEPATH += $$PWD

QMAKE_CXXFLAGS += -std=gnu++11

HEADERS += \
    ../src/graph.h \
    ../src/adjacency.h \
    ../src/adjacencylist.h \
    ../src/adjacencybitset.h \
    ../src/emptyadjacency.h \
    ../src/singleadjacency.h

SOURCES +=