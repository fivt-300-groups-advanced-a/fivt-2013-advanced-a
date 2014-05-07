#setting compiler
CXX = clang++

#for gtest
CPPFLAGS += -isystem gtest-1.7.0/include 
CXXFLAGS += -ggdb -Wall -Wextra -pthread

#for c++11
CXXFLAGS += -stdlib=libc++ -std=c++11

all: test testfortest

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

#graph binaries
func.o: func.cpp graph.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

#building test
test.o: test.cpp graph.h helpers.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

test: test.o func.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

#building testfortest
testfortest.o: testfortest.cpp graph.h helpers.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

testfortest: testfortest.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@
