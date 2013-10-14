include(gtest.pri)

QMAKE_CXXFLAGS += -std=gnu++0x -lgtest -pthread
QMAKE_LIBS += /usr/local/lib/libgtest.a

HEADERS += \
    io/binaryfilereader.h \
    io/abstractreader.h \
    io/inputstreamreader.h \
    io/abstractwriter.h \
    io/outputstreamwriter.h \
    io/binaryfilewriter.h \
    sorters/standartsorter.h \
    sorters/standartstablesorter.h \
    utils/binaryheap.h \
    utils/stablebinaryheap.h \
    externalsorter.h
