#setting compiler
CXX = clang++

#for gtest
CPPFLAGS += -isystem gtest-1.7.0/include 
CXXFLAGS += -O2 -g -Wall -Wextra

#for c++11
CXXFLAGS += -stdlib=libc++ -std=c++11

#for my headers
CXXFLAGS += -I source/algo
CXXFLAGS += -I source/graph
CXXFLAGS += -I testsource/testtools/

all: implementations graph factory algo testtools

#clean
clean:
	rm -rf testbin bin

#testsets
implementations: testbin/test_implementations
graph: testbin/test_graph
factory: testbin/test_factory
algo: testbin/test_algo
testtools: testbin/test_testtools

#test_implementations
testbin/test_implementations: bin/test_implementations.o \
															bin/gtest_main.a
	mkdir -p testbin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

bin/test_implementations.o: testsource/test/test_implementations.cpp \
														source/graph/graph.h                     \
														source/graph/implementations.h           \
														$(GTEST_HEADERS)
	mkdir -p bin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ 

#test_graph
testbin/test_graph: bin/test_graph.o \
										bin/gen.o        \
										bin/gtest_main.a
	mkdir -p testbin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

bin/test_graph.o: testsource/test/test_graph.cpp \
								  testsource/testtools/gen.h     \
								  source/graph/graph.h           \
								  source/graph/implementations.h \
								  $(GTEST_HEADERS)
	mkdir -p bin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ 

#test_factory
testbin/test_factory: bin/test_factory.o \
											bin/gtest_main.a
	mkdir -p testbin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^  -o $@

bin/test_factory.o: testsource/test/test_factory.cpp \
						 	      source/graph/graph.h             \
						        source/graph/implementations.h   \
						        source/graph/factory.h           \
						        $(GTEST_HEADERS)
	mkdir -p bin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

#test_algo
testbin/test_algo: bin/test_algo.o \
									 bin/algo.o      \
									 bin/gen.o       \
									 bin/dummyalgo.o \
									 bin/gtest_main.a
	mkdir -p testbin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^  -o $@

bin/test_algo.o: testsource/test/test_algo.cpp     \
								 testsource/testtools/gen.h        \
								 testsource/testtools/dummyalgo.h  \
								 source/algo/algo.h                \
								 source/algo/coloring.h            \
								 source/graph/graph.h              \
								 $(GTEST_HEADERS)
	mkdir -p bin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

#test_testtools
testbin/test_testtools: bin/test_testtools.o \
										 	 	bin/gen.o            \
											  bin/dummyalgo.o      \
											  bin/gtest_main.a
	mkdir -p testbin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^  -o $@

bin/test_testtools.o: testsource/test/test_testtools.cpp \
								      testsource/testtools/gen.h         \
								      testsource/testtools/dummyalgo.h   \
								      source/algo/coloring.h             \
										  source/graph/graph.h               \
										  $(GTEST_HEADERS)
	mkdir -p bin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

#more binaries
bin/algo.o: source/algo/algo.cpp \
						source/graph/graph.h \
						source/graph/factory.h
	mkdir -p bin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

bin/gen.o: testsource/testtools/gen.cpp \
					 source/graph/graph.h         \
					 source/graph/implementations.h
	mkdir -p bin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@	     

bin/dummyalgo.o: testsource/testtools/dummyalgo.cpp \
								 testsource/testtools/gen.h         \
								 source/graph/graph.h
	mkdir -p bin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@	 

#building gtest
GTEST_HEADERS = gtest-1.7.0/include/gtest/*.h \
                gtest-1.7.0/include/gtest/internal/*.h

GTEST_SRCS_ = gtest-1.7.0/src/*.cc gtest-1.7.0/src/*.h $(GTEST_HEADERS)

bin/gtest-all.o: $(GTEST_SRCS_)
	mkdir -p bin
	$(CXX) $(CPPFLAGS) -I gtest-1.7.0 $(CXXFLAGS) -c \
            gtest-1.7.0/src/gtest-all.cc \
            -o bin/gtest-all.o

bin/gtest_main.o: $(GTEST_SRCS_)
	mkdir -p bin
	$(CXX) $(CPPFLAGS) -I gtest-1.7.0 $(CXXFLAGS) -c \
            gtest-1.7.0/src/gtest_main.cc \
            -o bin/gtest_main.o

bin/gtest.a: .bin/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

bin/gtest_main.a: bin/gtest-all.o bin/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^