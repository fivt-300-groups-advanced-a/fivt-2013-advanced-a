#include <typeinfo>
#include <iostream>
#include <fstream>
#include <set>

#include "io/inputstreamreader.h"
#include "io/outputstreamwriter.h"
#include "io/binaryfilereader.h"
#include "io/binaryfilewriter.h"

#include <gtest/gtest.h>

int main()
{
	InputStreamReader<int> intReader;
	InputStreamReader<float> floatReader;
	InputStreamReader<std::string> stringReader;

	intReader.isDelimeter(' ');
	EXPECT_EQ(2 + 2, 4);
	return 0;
}
