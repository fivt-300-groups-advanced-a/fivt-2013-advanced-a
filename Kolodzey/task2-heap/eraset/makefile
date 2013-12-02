#setting compiler
CXX = clang++

#for c++11
CXXFLAGS += -stdlib=libc++ -std=c++11

all: gen_test

gen_test: gen_eraset.cpp
	$(CXX) $(CXXFLAGS) $< -o $@