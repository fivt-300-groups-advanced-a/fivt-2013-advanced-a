TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(src/SortExt.pri)

GTEST = D:\Dropbox\gtest

DEFINES += GTEST_LINKED_AS_SHARED_LIBRARY=1
LIBS += -L$$GTEST/lib -lgtest
INCLUDEPATH += $$GTEST/include/

include(tests/tests.pri)

SOURCES += gtest_main.cc
