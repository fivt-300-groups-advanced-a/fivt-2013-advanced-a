#include <iostream>
#include <fstream>
#include <set>

#include "io/inputstreamreader.h"
#include "io/outputstreamwriter.h"
#include "io/binaryfilereader.h"
#include "io/binaryfilewriter.h"

int main()
{
	InputStreamReader<int> reader;
	std::vector<int> data(3, 0);
	reader.addDelimeter('{');
	reader.addDelimeter('}');
	reader.addDelimeter(',');
	reader(data.begin(), data.end());
	for (int i = 0; i < 3; i++) std::cout << data[i] << std::endl;
	return 0;
}
