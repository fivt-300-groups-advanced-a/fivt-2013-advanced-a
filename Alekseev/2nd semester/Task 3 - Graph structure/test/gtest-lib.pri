DEFINES += GT

unix:{
    LIBS += -lpthread
    QMAKE_LIBS += /usr/local/lib/gtest.a
}

win32:{
    GTEST = D:\Dropbox\gtest
    DEFINES += GTEST_LINKED_AS_SHARED_LIBRARY=1
    LIBS += -L$$GTEST/lib -lgtest
    INCLUDEPATH += $$GTEST/include/
}

SOURCES += \
    gtest_main.cc
