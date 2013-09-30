TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

#include(gtest/gtest.pri)

#GTESTS_DIR = ../../../gtest

#INCLUDEPATH += $$(GTESTS_DIR)

#SOURCES += \
#    $$(GTESTS_DIR)/src/gtests_main.cc \
#    $$(GTESTS_DIR)/src/gtests-all.cc

#gtests.commands = mingw32-make.exe $$_PRO_FILE_PWD_\gtests.make

#QMAKE_EXTRA_TARGETS += gtests
#PRE_TARGETDEPS += gtests

SOURCES += \
    main.cpp \
    istreamreader-test.cpp

INCLUDEPATH += ../../../gtest
INCLUDEPATH += ../../../gtest/include
INCLUDEPATH += ../../../gtest/include/internal
DEPENDPATH += ../../../gtest/src

SOURCES += ../../../gtest/src/gtest-all.cc
