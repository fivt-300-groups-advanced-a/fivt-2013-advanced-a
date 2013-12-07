#setting compiler
CXX = clang++

#for c++11
CXXFLAGS += -stdlib=libc++ -std=c++11

all: gen_sorted

gen_sorted: gen_sorted.cpp
	$(CXX) $(CXXFLAGS) $< -o $@