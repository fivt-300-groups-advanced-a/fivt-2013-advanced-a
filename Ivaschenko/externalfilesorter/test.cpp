#include "inputstreamreader.h"
#include "binaryfilereader.h"

#include <iostream>
#include <fstream>

int main()
{
	std::ofstream stream("input.txt", std::ios_base::binary);
	int q = 42;
	stream.write((char *) &q, 4);
	stream.close();

	BinaryFileReader<int> reader("input.txt");
	int x;
	reader(x);
	std::cout << x << std::endl;
	return 0;
}
