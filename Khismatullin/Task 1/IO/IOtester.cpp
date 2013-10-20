//#include <iostream>
#include "reader.h"
#include "breader.h"
#include "writer.h"
#include "bwriter.h"

using namespace std;

int main() {
	{
	Reader<int> in("input.txt");
	Writer<int> out("output.txt");
	BinReader<int> bin("bin.txt");
	BinWriter<int> bout("bin.txt");
	int n;
	in(n);
	bout(n);
	bin(n);
	out(n);
	}
	remove("bin.txt");
	
	
	return 0;
}