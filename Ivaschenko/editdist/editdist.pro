QMAKE_CXXFLAGS += -std=gnu++0x
QMAKE_LIBS += /usr/local/lib/libgtest.a

SOURCES += main.cpp \
    gtest/editdistunittest.cpp
HEADERS += \
    editdist.h

