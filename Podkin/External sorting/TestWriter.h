#include "Writer.h"
#include <gtest/gtest.h>
#include <fstream>
#include <iostream>

TEST(TestWriter, StringConstructor){
    Writer<int> out("writer.in");

    int a = 0;
    EXPECT_TRUE(out(a, "", ""));
    out.Close();

    std::ifstream in("writer.in", std::ifstream::in);
    int num;
    in >> num;
    EXPECT_EQ(num, a);
}

TEST(TestWriter, CharConstructor){
    Writer<int> out((char*)("writer.in"));

    int a = 0;
    EXPECT_TRUE(out(a, "", ""));
    out.Close();

    std::ifstream in("writer.in", std::ifstream::in);
    int num;
    in >> num;
    EXPECT_EQ(num, a);
}

TEST(TestWriter, YourStreamConstructor){
    std::ofstream MyStream("writer.in", std::ofstream::out);
    Writer<int> out(MyStream);

    int a = 0;
    EXPECT_TRUE(out(a, "", ""));
    out.Close();

    std::ifstream in("writer.in", std::ifstream::in);
    int num;
    in >> num;
    EXPECT_EQ(num, a);
}

TEST(TestWriter, Ready){
    Writer<int> out("writer.in");

    EXPECT_TRUE(out.Ready());
}
