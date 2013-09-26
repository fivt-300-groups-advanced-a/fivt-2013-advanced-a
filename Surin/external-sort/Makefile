main: clean main.o
	g++ ./main.o -o main -pg -w
clean:
	rm -f ./main
	rm -rf ./*.o
	rm -f ./gmon.out
main.o:
	g++ -ggdb -c ./main.cpp -pg -w -std=c++11

