CC = g++
FLAGS = -std=c++11 -O3
TFLAGS = -pthread -lgtest_main -lgtest

test: clean
	$(CC) $(FLAGS) $(TFLAGS) test.cpp -o test
	./test

clean: 
	rm -f test *.o
