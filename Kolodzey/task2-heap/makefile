#setting compiler
CXX = clang++

#for gtest
CPPFLAGS += -isystem gtest-1.7.0/include 
CXXFLAGS += -O2 -g -Wall -Wextra -pthread

#for c++11
CXXFLAGS += -stdlib=libc++ -std=c++11

all:  biheap_unittest informatics755bin erasetbin cmpstate pseudomerge heapsortest convexhullbin

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


#building biheap_unittest
biheap_unittest: biheap_unittest.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

biheap_unittest.o: biheap_unittest.cpp biheap.h access.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

#building informatics755bin
informatics755bin: informatics755.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

informatics755.o: informatics755.cpp biheap.h access.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

#building erasetbin
erasetbin: eraset.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

eraset.o: eraset.cpp biheap.h access.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

#building cmpstate
cmpstate: cmpstate.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

cmpstate.o: comparator-state.cpp biheap.h access.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

#building pseudomerge
pseudomerge: pseudomerge.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

pseudomerge.o: pseudomerge.cpp biheap.h access.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

#building heapsortest
heapsortest: heapsortest.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

heapsortest.o: heapsortest.cpp heapsort.h biheap.h access.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

#building convexhullbin
convexhullbin: convexhull.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

convexhull.o: convex_hull.cpp heapsort.h biheap.h access.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@