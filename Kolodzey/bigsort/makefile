res/prog: bin/main.o bin/helloworld.o
	clang++ -o res/prog bin/main.o bin/helloworld.o

bin/main.o: source/helloworld.h source/main.cpp
	clang++ -c -o bin/main.o source/main.cpp

bin/helloworld.o: source/helloworld.h source/helloworld.cpp
	clang++ -c -o bin/helloworld.o source/helloworld.cpp