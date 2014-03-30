#include "gtest/gtest.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main(int argc, char** argv)
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); //for testing memory leaks
    ::testing::InitGoogleTest(&argc,argv);
    RUN_ALL_TESTS();
    std::getchar();
        return 0;
}