#include <gtest/gtest.h>
#include <cstdlib>
#include <ctime>

#include "stress_test.h"
#include "manual_test.h"

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    srand(19121994);
    return RUN_ALL_TESTS();
}
