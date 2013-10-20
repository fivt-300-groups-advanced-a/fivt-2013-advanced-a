#include <gtest/gtest.h>
#include "FileReader.h"
#include <fstream>
#include <iostream>

TEST(TestReader, StringConstructor){
    std::ofstream out("reader.in", std::ofstream::out);
    int num = 0;
    out << num;
    out.close();
    
    FileReader<int> in("reader.in");

    int a;
    EXPECT_TRUE(in(a));
    EXPECT_EQ(num, a);
}

TEST(TestReader, CharConstructor){
    std::ofstream out("reader.in", std::ofstream::out);
    int num = -1;
    out << num;
    out.close();

    FileReader<int> in((char*)("reader.in"));

    int a;
    EXPECT_TRUE(in(a));
    EXPECT_EQ(num, a);
}

TEST(TestReader, Next){
    std::ofstream out("reader.in", std::ofstream::out);
    int num = 234;
    out << num;
    out.close();
    
    FileReader<int> in("reader.in");

    EXPECT_TRUE(in.Next());
}
