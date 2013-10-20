#include <gtest/gtest.h>
#include <fstream>
#include <iostream>
#include "TestReader.h"
#include "TestBinaryReader.h"
#include "TestWriter.h"
#include "TestBinaryWriter.h"
#include "TestExternalSorter.h"

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
