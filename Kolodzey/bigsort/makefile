CXX = clang++

res/prog: bin/main.o bin/helloworld.o
	$(CXX) -o res/prog bin/main.o bin/helloworld.o

bin/main.o: source/helloworld.h source/main.cpp
	$(CXX) -c -o bin/main.o source/main.cpp

bin/helloworld.o: source/helloworld.h source/helloworld.cpp
	$(CXX) -c -o bin/helloworld.o source/helloworld.cpp