#include <typeinfo>
#include <iostream>
#include <fstream>
#include <set>

#include "io/inputstreamreader.h"
#include "io/outputstreamwriter.h"

#include "sorters/standartsorter.h"
#include "sorters/digitalsorter.h"
#include "utils/integerbitblockextractor.h"
#include "externalsorter.h"

#include <gtest/gtest.h>

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
