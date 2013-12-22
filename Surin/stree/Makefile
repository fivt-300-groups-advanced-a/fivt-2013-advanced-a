CC := g++
CXXFLAGS.DEBUG := -pg -g -std=c++11
LINKER.DEBUG := -pg -g
CXXFLAGS.RELEASE := -std=c++11 -O3
LINKER.RELEASE := -O3 -lgtest -march=native
CXXFLAGS.TEST := -std=c++11 -ggdb -pg
LINKER.TEST := -lgtest -pg
all: Release
    
Test: clean test.o
	$(CC) $(LINKER.TEST) ./test.o -o test
	ln -s test main
Debug: clean main.o.debug
	$(CC) $(LINKER.DEBUG) ./main.o -o ./main
Release: clean main.o.release
	$(CC) $(LINKER.RELEASE) ./test.o -o ./test
	ln -s test main
Fast: clean
	g++ ./test.cpp -fprofile-generate -O3 -march=native -mtune=native -fprofile-arcs -fprofile-values -o ./main -std=c++0x -lgtest
	./main >/dev/null
	g++ ./test.cpp -fprofile-use -fvpt -funroll-loops -ftracer -fbranch-probabilities -O3 -march=native -mtune=native -o ./main -std=c++0x -lgtest
clean:
	rm -f ./main
	rm -rf ./*.o
	rm -f ./gmon.out
	rm -f ./test
	rm -f ./*.gcda
main.o.debug:
	$(CC) $(CXXFLAGS.DEBUG) -c ./main.cpp
main.o.release:
	$(CC) $(CXXFLAGS.RELEASE) -c ./test.cpp
test.o:
	$(CC) $(CXXFLAGS.TEST) -c ./test.cpp
