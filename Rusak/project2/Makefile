CC = g++
FLAGS = -std=c++11 -O3
TFLAGS = -pthread -lgtest_main -lgtest

main: clean
	$(CC) $(FLAGS) main.cpp -o main
	./main

test: clean
	$(CC) $(FLAGS) $(TFLAGS) test.cpp -o test
	./test

clean: 
	rm -f test main *.o
