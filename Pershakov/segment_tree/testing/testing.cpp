#include <gtest/gtest.h>
#include <cstdlib>
#include <ctime>

#include "stress_test.h"

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    srand(time(0));
    return RUN_ALL_TESTS();
}
