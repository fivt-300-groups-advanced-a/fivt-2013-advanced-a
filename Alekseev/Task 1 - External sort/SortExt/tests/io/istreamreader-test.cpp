#include <sstream>

#include "gtest/gtest.h"

#include "src/io/istreamreader.h"

#include "../complexdata.h"

TEST(IStreamReader, Integers)
{
    std::istringstream iss("12354 54\n\t45\n344423");
    IStreamReader<int> reader(iss);

    int x;
    for (int exp : {12354, 54, 45, 344423})
    {
        EXPECT_TRUE(reader(x));
        EXPECT_EQ(exp, x);
    }
    EXPECT_FALSE(reader(x));
}

TEST(IStreamReader, Strings)
{
    std::istringstream iss("./4t 3rr\t\t\nrrr3dd");
    IStreamReader<std::string> reader(iss);

    std::string s;
    for (std::string exp : {"./4t", "3rr", "rrr3dd"})
    {
        EXPECT_TRUE(reader(s));
        EXPECT_EQ(exp, s);
    }
    EXPECT_FALSE(reader(s));
}

TEST(IStreamReader, ComplexData)
{
    std::istringstream iss("54.2 erwe .1415 wre\tqoe");
    IStreamReader<ComplexData> reader(iss);

    ComplexData d;
    EXPECT_TRUE(reader(d));
    EXPECT_FLOAT_EQ(54.2, d.d);
    EXPECT_EQ("erwe", d.s);

    EXPECT_TRUE(reader(d));
    EXPECT_FLOAT_EQ(.1415, d.d);
    EXPECT_EQ("wre", d.s);

    EXPECT_FALSE(reader(d));
}

TEST(IStreamReader, EmptyInput)
{
    std::istringstream iss;
    IStreamReader<int> reader(iss);
    int x;
    EXPECT_FALSE(reader(x));
}
