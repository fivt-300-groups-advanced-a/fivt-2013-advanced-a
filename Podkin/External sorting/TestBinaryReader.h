#include <gtest/gtest.h>
#include "BinaryReader.h"
#include <fstream>
#include <iostream>

TEST(TestBinaryReader, StringConstructor){
    std::ofstream out("binary_reader.in", std::ofstream::out | std::ofstream::binary);
    int num = 0;
    out.write((char*)&num, sizeof(int));
    out.close();
    
    BinaryReader<int> in("binary_reader.in");

    int a;
    EXPECT_TRUE(in(a));
    EXPECT_EQ(num, a);
}

TEST(TestBinaryReader, CharConstructor){
    std::ofstream out("binary_reader.in", std::ofstream::out | std::ofstream::binary);
    int num = -1;
    out.write((char*)&num, sizeof(int));
    out.close();

    BinaryReader<int> in((char*)("binary_reader.in"));

    int a;
    EXPECT_TRUE(in(a));
    EXPECT_EQ(num, a);
}

TEST(TestBinaryReader, YourStreamConstructor){
    std::ofstream out("binary_reader.in", std::ofstream::out | std::ofstream::binary);
    int num = -545;
    out.write((char*)&num, sizeof(int));
    out.close();

    std::ifstream MyStream("binary_reader.in", std::ifstream::in | std::ifstream::binary);
    BinaryReader<int> in(MyStream);

    int a;
    EXPECT_TRUE(in(a));
    EXPECT_EQ(num, a);
}
