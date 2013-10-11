#include <gtest/gtest.h>
#include "Reader.h"
#include <fstream>
#include <iostream>

TEST(TestReader, StringConstructor){
    std::ofstream out("reader.in", std::ofstream::out);
    int num = 0;
    out << num;
    out.close();
    
    Reader<int> in("reader.in");

    int a;
    EXPECT_TRUE(in(a));
    EXPECT_EQ(num, a);
}

TEST(TestReader, CharConstructor){
    std::ofstream out("reader.in", std::ofstream::out);
    int num = -1;
    out << num;
    out.close();

    Reader<int> in((char*)("reader.in"));

    int a;
    EXPECT_TRUE(in(a));
    EXPECT_EQ(num, a);
}

TEST(TestReader, YourStreamConstructor){
    std::ofstream out("reader.in", std::ofstream::out);
    int num = -545;
    out << num;
    out.close();

    std::ifstream MyStream("reader.in", std::ifstream::in);
    Reader<int> in(MyStream);

    int a;
    EXPECT_TRUE(in(a));
    EXPECT_EQ(num, a);
}

TEST(TestReader, Next){
    std::ofstream out("reader.in", std::ofstream::out);
    int num = 234;
    out << num;
    out.close();
    
    Reader<int> in("reader.in");

    EXPECT_TRUE(in.Next());
}
