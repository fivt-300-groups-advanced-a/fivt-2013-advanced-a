#include "FileWriter.h"
#include <gtest/gtest.h>
#include <fstream>
#include <iostream>

TEST(TestWriter, StringConstructor){
    FileWriter<int> out("writer.in");

    int a = 0;
    EXPECT_TRUE(out(a, "", ""));
    out.Close();

    std::ifstream in("writer.in", std::ifstream::in);
    int num;
    in >> num;
    EXPECT_EQ(num, a);
}

TEST(TestWriter, CharConstructor){
    FileWriter<int> out((char*)("writer.in"));

    int a = 0;
    EXPECT_TRUE(out(a, "", ""));
    out.Close();

    std::ifstream in("writer.in", std::ifstream::in);
    int num;
    in >> num;
    EXPECT_EQ(num, a);
}

TEST(TestWriter, Ready){
    FileWriter<int> out("writer.in");

    EXPECT_TRUE(out.Ready());
}
