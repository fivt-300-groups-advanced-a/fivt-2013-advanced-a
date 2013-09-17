#include "inputstreamreader.h"
#include "binaryfilereader.h"

#include <iostream>
#include <fstream>

#include "inputstreamreader.h"
#include "outputstreamwriter.h"
#include "binaryfilereader.h"
#include "binaryfilewriter.h"

int main()
{
	BinaryFileWriter<int> out("input.txt");
	out(42);
	out.unbindStream();

	BinaryFileReader<int> in("input.txt");
	int x;
	in(x);
	std::cout << x;
	in.unbindStream();
	return 0;
}
