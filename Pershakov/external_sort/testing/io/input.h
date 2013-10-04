#include <gtest/gtest.h>
#include <fstream>
#include "../../io/text_file_reader.h"
#include "../../io/binary_file_reader.h"

/*
 * TextFileReader testing
 */

TEST(TestTextFileReader, constructFromStdString){
    std::ofstream out("text_file_reader.in", std::ofstream::out);
    int a = 6;
    out << a;
    out.close();

    std::string filename = "text_file_reader.in";
    TextFileReader<int> reader(filename);

    int a_new;
    EXPECT_TRUE(reader(a_new));
    EXPECT_EQ(a, a_new);
}

TEST(TestTextFileReader, constructFromCString){
    std::ofstream out("text_file_reader.in", std::ofstream::out);
    int a = 6;
    out << a;
    out.close();

    std::string filename = "text_file_reader.in";
    TextFileReader<int> reader(filename.c_str());

    int a_new;
    EXPECT_TRUE(reader(a_new));
    EXPECT_EQ(a, a_new);
}

TEST(TestTextFileReader, constructFromStream){
    std::ofstream out("text_file_reader.in", std::ofstream::out);
    int a = 6;
    out << a;
    out.close();

    std::ifstream in("text_file_reader.in", std::ifstream::in);
    TextFileReader<int> reader(in);

    int a_new;
    EXPECT_TRUE(reader(a_new));
    EXPECT_EQ(a, a_new);
}

TEST(TestTextFileReader, successfulReading){
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

/*
 * BinaryFileReader testing
 */

TEST(TestBinaryFileReader, constructFromStdString){
    std::ofstream out("binary_file_reader.in", 
            std::ofstream::out | std::ofstream::binary);
    int a = -56;
    out.write((char*)&a, sizeof(int));
    out.close();

    std::string filename = "binary_file_reader.in";
    BinaryFileReader<int> reader(filename);
    int a_new;
    EXPECT_TRUE(reader(a_new));
    EXPECT_EQ(a, a_new);
}

TEST(TestBinaryFileReader, constructFromCString){
    std::ofstream out("binary_file_reader.in", 
            std::ofstream::out | std::ofstream::binary);
    int a = -56;
    out.write((char*)&a, sizeof(int));
    out.close();

    std::string filename = "binary_file_reader.in";
    BinaryFileReader<int> reader(filename.c_str());
    int a_new;
    EXPECT_TRUE(reader(a_new));
    EXPECT_EQ(a, a_new);
}

TEST(TestBinaryFileReader, constructFromStream){
    std::ofstream out("binary_file_reader.in", 
            std::ofstream::out | std::ofstream::binary);
    int a = -56;
    out.write((char*)&a, sizeof(int));
    out.close();

    std::ifstream in("binary_file_reader.in",
            std::ifstream::in | std::ifstream::binary);
    BinaryFileReader<int> reader(in);
    int a_new;
    EXPECT_TRUE(reader(a_new));
    EXPECT_EQ(a, a_new);
}

TEST(TestBinaryFileReader, successfulReading){
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

