#include <gtest/gtest.h>
#include "io/input.h"
#include "io/output.h"
#include "sort/digital_sort.h"
#include "sort/external_sort.h"

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
