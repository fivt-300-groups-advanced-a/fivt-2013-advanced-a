#setting compiler
CXX = clang++

#for c++11
CXXFLAGS += -stdlib=libc++ -std=c++11

all: gen_test

gen_test: kraskevich-convexhull.cpp
	$(CXX) $(CXXFLAGS) $< -o $@