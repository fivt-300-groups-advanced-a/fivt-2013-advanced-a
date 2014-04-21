#include <gtest/gtest.h>

#include "iterator_testing.h"
#include "list_of_incidents_testing.h"
#include "graph_testing.h"
#include "strong_connectivity_testing.h"

int main(int argc, char *argv[]) {
    srand(19121994);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

