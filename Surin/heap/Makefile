CC := g++
CXXFLAGS.DEBUG := -pg -g -std=c++11
LINKER.DEBUG := -pg -g
CXXFLAGS.RELEASE := -std=c++11 -O3
LINKER.RELEASE := -O3
CXXFLAGS.TEST := -std=c++11 -O3 #-ggdb -pg
LINKER.TEST := -lgtest

Test: clean test.o
	$(CC) $(LINKER.TEST) ./test.o -o test
	ln -s test main
Debug: clean main.o.debug
	$(CC) $(LINKER.DEBUG) ./main.o -o ./main
Release: clean main.o.release
	$(CC) $(LINKER.RELEASE) ./main.o -o ./main

clean:
	rm -f ./main
	rm -rf ./*.o
	rm -f ./gmon.out
	rm -f ./test
main.o.debug:
	$(CC) $(CXXFLAGS.DEBUG) -c ./main.cpp
main.o.release:
	$(CC) $(CXXFLAGS.RELEASE) -c ./main.cpp
test.o:
	$(CC) $(CXXFLAGS.TEST) -c ./test.cpp
