include(gtest.pri)

QMAKE_CXXFLAGS += -std=gnu++0x -lgtest -pthread
QMAKE_LIBS += /usr/local/lib/libgtest.a

SOURCES +=

HEADERS += \
    model/generalsegmenttree.h \
    model/segmentadditiontree.h \
    model/segmentassignmenttree.h \
    model/segmentadditionassignmenttree.h \
    applications/maximalsumsubsegment.h \
    applications/constasysegments.h

