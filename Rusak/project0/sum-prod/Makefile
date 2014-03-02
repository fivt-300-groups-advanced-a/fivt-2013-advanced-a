CC = g++
FLAGS = -std=c++11 -O3
TFLAGS = -pthread -lgtest_main -lgtest


.PHONY: test

#all: test clean

test:
	$(CC) $(FLAGS) $(TFLAGS) test.cpp -o test
	./test
	rm -f test *.o
