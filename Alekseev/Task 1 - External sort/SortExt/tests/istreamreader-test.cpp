#include <sstream>

#include "gtest/gtest.h"

#include "src/istreamreader.h"

#include "complexdata.h"

TEST(IStreamReader, Integers)
{
    std::istringstream iss("12354 54\n\t45\n344423");
    IStreamReader<int> reader(iss);

    int x;
    EXPECT_TRUE(reader(x));
    EXPECT_EQ(12354, x);

    EXPECT_TRUE(reader(x));
    EXPECT_EQ(54, x);

    EXPECT_TRUE(reader(x));
    EXPECT_EQ(45, x);

    EXPECT_TRUE(reader(x));
    EXPECT_EQ(344423, x);

    EXPECT_FALSE(reader(x));
}

TEST(IStreamReader, Strings)
{
    std::istringstream iss("./4t 3rr\t\t\nrrr3dd");
    IStreamReader<std::string> reader(iss);

    std::string s;
    EXPECT_TRUE(reader(s));
    EXPECT_EQ("./4t", s);

    EXPECT_TRUE(reader(s));
    EXPECT_EQ("3rr", s);

    EXPECT_TRUE(reader(s));
    EXPECT_EQ("rrr3dd", s);

    EXPECT_FALSE(reader(s));
}

TEST(IStreamReader, ComplexData)
{
    std::istringstream iss("54.2 erwe wre");
    IStreamReader<ComplexData> reader(iss);

    ComplexData d;
    EXPECT_TRUE(reader(d));
    EXPECT_FLOAT_EQ(54.2, d.d);
    EXPECT_EQ(d.s, "erwe");

    EXPECT_FALSE(reader(d));
}

TEST(IStreamReader, EmptyInput)
{
    std::istringstream iss;
    IStreamReader<int> reader(iss);
    int x;
    EXPECT_FALSE(reader(x));
}
