#include "BinomialHeap.h"
#include "VectorHeap.h"
#include "IntegrationTest.h"
#include <functional>
#include <iostream>
#include "gtest/gtest.h"
#include "MyTypeTest.h"
#include "InvariantTest.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

