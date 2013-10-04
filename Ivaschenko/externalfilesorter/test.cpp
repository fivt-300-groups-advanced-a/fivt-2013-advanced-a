#include <typeinfo>
#include <iostream>
#include <fstream>
#include <set>

#include "io/inputstreamreader.h"
#include "io/outputstreamwriter.h"
#include "io/binaryfilereader.h"
#include "io/binaryfilewriter.h"

#include "externalfilesorter.h"

#include <gtest/gtest.h>

int main(int argc, char **argv)
{
	assert(!remove("someFile"));
	ExternalFileSorter<int> sorter;
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
