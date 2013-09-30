#include <gtest/gtest.h>
#include "../../io/text_file_writer.h"
#include "../../io/text_file_reader.h"

/*
 * TextFileWriter testing
 * it needs TextFileReader tested
 */

TEST(TestTextFileWriter, constructFromStdString){
    std::string filename = "text_file_writer.out";
    TextFileWriter<int> writer(filename);
    int a = 46;
    EXPECT_TRUE(writer(a));
    writer.close();

    TextFileReader<int> reader(filename);
    int new_a;
    EXPECT_TRUE(reader(new_a));
    EXPECT_EQ(a, new_a);
    reader.close();
}

TEST(TestTextFileWriter, constructFromCString){
    std::string filename = "text_file_writer.out";
    TextFileWriter<int> writer(filename.c_str());
    int a = 35;
    EXPECT_TRUE(writer(a));
    writer.close(); 

    TextFileReader<int> reader(filename.c_str());
    int new_a;
    EXPECT_TRUE(reader(new_a));
    EXPECT_EQ(a, new_a);
    reader.close();
}

TEST(TestTextFileWriter, constructFromConstString){
    TextFileWriter<int> writer("text_file_writer.out");
    int a = 36;
    EXPECT_TRUE(writer(a));
    writer.close();

    TextFileReader<int> reader("text_file_writer.out");
    int new_a;
    EXPECT_TRUE(reader(new_a));
    EXPECT_EQ(a, new_a);
    reader.close();
}

TEST(TestTextFileWriter, constructFromStream){
    std::ofstream out("text_file_writer.out");
    TextFileWriter<int> writer(out);
    int a = 37;
    EXPECT_TRUE(writer(a));
    writer.close();

    std::ifstream in("text_file_writer.out");
    TextFileReader<int> reader(in);
    int new_a;
    EXPECT_TRUE(reader(new_a));
    EXPECT_EQ(a, new_a);
    reader.close();
}

TEST(TestTextFileWriter, successfulWriting){
    std::ofstream out("text_file_writer.out");
    TextFileWriter<int> writer(out);
    int a = -1, b = 232, c = -2323;
    EXPECT_TRUE(writer(a));
    EXPECT_TRUE(writer(b));
    EXPECT_TRUE(writer(c));
    writer.close();

    std::ifstream in("text_file_writer.out");
    TextFileReader<int> reader(in);
    int new_a, new_b, new_c;
    EXPECT_TRUE(reader(new_a));
    EXPECT_TRUE(reader(new_b));
    EXPECT_TRUE(reader(new_c));
    EXPECT_EQ(a, new_a);
    EXPECT_EQ(b, new_b);
    EXPECT_EQ(c, new_c);
    reader.close();
}

TEST(TestTextFileWriter, failedWriting){
    std::ofstream out("text_file_writer.out");
    TextFileWriter<int> writer(out);
    int a = -4242, b = 44232;;
    EXPECT_TRUE(writer(a));
    EXPECT_TRUE(writer(b));
    writer.close();

    std::ifstream in("text_file_writer.out");
    TextFileReader<int> reader(in);
    int new_a, new_b, new_c;
    EXPECT_TRUE(reader(new_a));
    EXPECT_TRUE(reader(new_b));
    EXPECT_FALSE(reader(new_c));
    EXPECT_EQ(a, new_a);
    EXPECT_EQ(b, new_b);
    reader.close();
}
