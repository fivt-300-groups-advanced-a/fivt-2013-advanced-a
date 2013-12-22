#setting compiler
CXX = clang++

#for gtest
CPPFLAGS += -isystem ../gtest-1.7.0/include 
CXXFLAGS += -O2 -g -Wall -Wextra -pthread

#for c++11
CXXFLAGS += -stdlib=libc++ -std=c++11

#for includes from source folder
CPPFLAGS += -I ../source

all: generaltree_manual

#building gtest
GTEST_HEADERS = ../gtest-1.7.0/include/gtest/*.h \
                ../gtest-1.7.0/include/gtest/internal/*.h

GTEST_SRCS_ = ../gtest-1.7.0/src/*.cc ../gtest-1.7.0/src/*.h $(GTEST_HEADERS)

../bin/gtest-all.o: $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I ../gtest-1.7.0 $(CXXFLAGS) -c \
            ../gtest-1.7.0/src/gtest-all.cc \
            -o ../bin/gtest-all.o

../bin/gtest_main.o: $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I ../gtest-1.7.0 $(CXXFLAGS) -c \
            ../gtest-1.7.0/src/gtest_main.cc \
            -o ../bin/gtest_main.o

../bin/gtest.a: ../bin/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

../bin/gtest_main.a: ../bin/gtest-all.o ../bin/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

#building generaltree_manual

generaltree_manual: ../bin/generaltree_manual.o ../bin/gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

../bin/generaltree_manual.o: generaltree_manual.cpp access.h ../source/generaltree.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@