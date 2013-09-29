#include <gtest/gtest.h>
#include <fstream>
#include "../input/text_file_reader.h"
#include "../input/binary_file_reader.h"

TEST(TestTextFileReader, succesfulReading){
    std::ofstream out("text_file_reader.in", std::ofstream::out);
    int a = 56, b = -533324, c = 3232;
    out << a << " " << b << " " << c << std::endl;
    out.close();
    std::ifstream in("text_file_reader.in", std::ifstream::in);

    TextFileReader<int> reader(in);
    int a_new, b_new, c_new;
    EXPECT_TRUE(reader(a_new));
    EXPECT_TRUE(reader(b_new));
    EXPECT_TRUE(reader(c_new));
    EXPECT_EQ(a, a_new);
    EXPECT_EQ(b, b_new);
    EXPECT_EQ(c, c_new);
}

TEST(TestTextFileReader, failedReading){
    std::ofstream out("text_file_reader.in", std::ifstream::out);
    int a = 23, b = 0;
    out << a << " " << b << std::endl;
    out.close();
    std::ifstream in("text_file_reader.in", std::ifstream::in);
    
    TextFileReader<int> reader(in);
    int a_new, b_new, c_new;
    EXPECT_TRUE(reader(a_new));
    EXPECT_TRUE(reader(b_new));
    EXPECT_FALSE(reader(c_new));
    EXPECT_EQ(a, a_new);
    EXPECT_EQ(b, b_new);
}

TEST(TestBinaryFileReader, succesfulReading){
    std::ofstream out("binary_file_reader.in", 
            std::ofstream::out | std::ofstream::binary);
    int a = -5643, b = 1212;
    out.write((char*)&a, sizeof(int));
    out.write((char*)&b, sizeof(int));
    out.close();

    std::ifstream in("binary_file_reader.in",
            std::ifstream::in | std::ifstream::binary);
    BinaryFileReader<int> reader(in);
    int a_new, b_new;
    EXPECT_TRUE(reader(a_new));
    EXPECT_TRUE(reader(b_new));
    EXPECT_EQ(a, a_new);
    EXPECT_EQ(b, b_new);
}

TEST(TestBinaryFileReader, failedReading){
    std::ofstream out("binary_file_reader.in", 
            std::ofstream::out | std::ofstream::binary);
    int a = -323, b = 14;
    out.write((char*)&a, sizeof(int));
    out.write((char*)&b, sizeof(int));
    out.close();

    std::ifstream in("binary_file_reader.in",
            std::ifstream::in | std::ifstream::binary);
    BinaryFileReader<int> reader(in);
    int a_new, b_new, c_new;
    EXPECT_TRUE(reader(a_new));
    EXPECT_TRUE(reader(b_new));
    EXPECT_FALSE(reader(c_new));
    EXPECT_EQ(a, a_new);
    EXPECT_EQ(b, b_new);
}


int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
