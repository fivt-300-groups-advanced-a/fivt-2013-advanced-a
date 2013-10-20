include(gtest.pri)

QMAKE_CXXFLAGS += -std=gnu++0x -lgtest -pthread
QMAKE_LIBS += /usr/local/lib/libgtest.a

HEADERS += \
    io/binaryfilereader.h \
    io/inputstreamreader.h \
    io/outputstreamwriter.h \
    io/binaryfilewriter.h \
    sorters/standartsorter.h \
    sorters/standartstablesorter.h \
    externalsorter.h \
    utils/tempfileiofactory.h \
    sorters/digitalsorter.h
