include(gtest.pri)

QMAKE_CXXFLAGS += -std=gnu++0x -lgtest -pthread
QMAKE_LIBS += /usr/local/lib/libgtest.a

#TEMPLATE = app
#CONFIG += console
#CONFIG -= app_bundle
#CONFIG -= qt

SOURCES +=

HEADERS += \
    model/binomialheap.h \
    model/binomialheapnode.h

