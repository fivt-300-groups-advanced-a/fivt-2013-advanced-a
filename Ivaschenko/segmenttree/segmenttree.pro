QMAKE_CXXFLAGS += -std=gnu++0x -lgtest -pthread
QMAKE_LIBS += /usr/local/lib/libgtest.a

SOURCES += main.cpp

HEADERS += \
    model/generalsegmenttree.h

