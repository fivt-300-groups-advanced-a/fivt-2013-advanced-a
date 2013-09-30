QMAKE_CXXFLAGS += -std=gnu++0x -lgtest -pthread
QMAKE_LIBS += /usr/local/lib/libgtest.a

HEADERS += \
    io/binaryfilereader.h \
    io/abstractreader.h \
    io/inputstreamreader.h \
    io/abstractwriter.h \
    io/outputstreamwriter.h \
    io/binaryfilewriter.h

SOURCES += \
    test.cpp
