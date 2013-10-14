#include "BinaryWriter.h"
#include <gtest/gtest.h>
#include <fstream>
#include <iostream>

TEST(TestBinaryWriter, StringConstructor){
    BinaryWriter<int> out("binary_writer.in");

    int a = 0;
    EXPECT_TRUE(out(a));
    out.Close();

    std::ifstream in("binary_writer.in", std::ifstream::in | std::ifstream::binary);
    int num;
    in.read((char*)&num, sizeof(int));
    EXPECT_EQ(num, a);
}

TEST(TestBinaryWriter, CharConstructor){
    BinaryWriter<int> out((char*)("binary_writer.in"));

    int a = -13;
    EXPECT_TRUE(out(a));
    out.Close();

    std::ifstream in("binary_writer.in", std::ifstream::in | std::ifstream::binary);
    int num;
    in.read((char*)&num, sizeof(int));
    EXPECT_EQ(num, a);
}

TEST(TestBinaryWriter, YourStreamConstructor){
    std::ofstream MyStream("binary_writer.in", std::ofstream::out | std::ofstream::binary);
    BinaryWriter<int> out(MyStream);

    int a = 0;
    EXPECT_TRUE(out(a));
    out.Close();

    std::ifstream in("binary_writer.in", std::ifstream::in | std::ifstream::binary);
    int num;
    in.read((char*)&num, sizeof(int));
    EXPECT_EQ(num, a);
}

TEST(TestBinaryWriter, Ready){
    BinaryWriter<int> out("binary_writer.in");

    EXPECT_TRUE(out.Ready());
}
