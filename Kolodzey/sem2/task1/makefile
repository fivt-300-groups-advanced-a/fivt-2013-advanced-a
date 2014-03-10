#setting compiler
CXX = clang++

#for gtest
CPPFLAGS += -isystem gtest-1.7.0/include 
CXXFLAGS += -O2 -g -Wall -Wextra -pthread

#for c++11
CXXFLAGS += -stdlib=libc++ -std=c++11

all: unittest integrationtest

#building gtest
GTEST_HEADERS = gtest-1.7.0/include/gtest/*.h \
                gtest-1.7.0/include/gtest/internal/*.h

GTEST_SRCS_ = gtest-1.7.0/src/*.cc gtest-1.7.0/src/*.h $(GTEST_HEADERS)

gtest-all.o: $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I gtest-1.7.0 $(CXXFLAGS) -c \
            gtest-1.7.0/src/gtest-all.cc \
            -o gtest-all.o

gtest_main.o: $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I gtest-1.7.0 $(CXXFLAGS) -c \
            gtest-1.7.0/src/gtest_main.cc \
            -o gtest_main.o

gtest.a: gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a: gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

#building unittest
unittest: unittest.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

unittest.o: unittest.cpp hirschberg.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

#building integrationtest
integrationtest: integrationtest.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

integrationtest.o: integrationtest.cpp hirschberg.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@