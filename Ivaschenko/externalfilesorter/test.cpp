#include <typeinfo>
#include <iostream>
#include <fstream>
#include <set>

#include "io/inputstreamreader.h"
#include "io/outputstreamwriter.h"
#include "io/binaryfilereader.h"
#include "io/binaryfilewriter.h"

int main()
{
	InputStreamReader<int> intReader;
	InputStreamReader<float> floatReader;
	InputStreamReader<std::string> stringReader;

	intReader.isDelimeter(' ');
	return 0;
}
