CXX := g++
CXXFLAGS := -std=c++11 -pg -ggdb

main: clean main.o
	$(CXX) $(CXXFLAGS) ./main.o -o main
clean:
	rm -f ./main
	rm -rf ./*.o
	rm -f ./gmon.out
	rm -f ./test
main.o:
	$(CXX) $(CXXFLAGS) -c ./main.cpp
test: clean
	$(CXX) $(CXXFLAGS) ./test.cpp -lgtest -o ./test
