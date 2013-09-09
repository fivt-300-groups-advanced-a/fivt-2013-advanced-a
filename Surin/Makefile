main: clean
	g++ ./main.cpp -o main -pg -w
test: clean
	g++ ./test.cpp -o ./test -std=c++11
	./test
clean:
	rm -f ./main
	rm -rf ./*.o
	rm -f ./test