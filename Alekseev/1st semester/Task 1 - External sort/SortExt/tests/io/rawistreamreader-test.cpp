#include <sstream>

#include <cmath>

#include "gtest/gtest.h"

#include "src/io/rawistreamreader.h"

TEST(RawIStreamReader, EmptyInput)
{
    std::istringstream iss("");
    RawIStreamReader<int> read(iss);

    int x;
    EXPECT_FALSE(read(x));
}

namespace
{
template<class Container>
std::string toRawString(const Container &ints)
{
    std::string ret;
    for (auto x : ints)
        ret += std::string(reinterpret_cast<char*>(&x), sizeof(x));
    return ret;
}
}

TEST(RawIStreamReader, Integers)
{
    std::initializer_list<int> data = {3, 2554, 1<<30, 1<<31, 0, 0, 43};
    std::istringstream iss(toRawString(data));
    RawIStreamReader<int> read(iss);

    int x;
    for (int exp : data)
    {
        ASSERT_TRUE(read(x));
        EXPECT_EQ(exp, x);
    }
    EXPECT_FALSE(read(x));
}

TEST(RawIStreamReader, Doubles)
{
    std::initializer_list<double> data = {M_PI, 54, 0., -1., -M_PI_4, 34e50, 0./0., 0e-3, 9e-20};
    std::istringstream iss(toRawString(data));
    RawIStreamReader<double> read(iss);

    double x;
    for (double exp : data)
    {
        ASSERT_TRUE(read(x));
        if (std::isnan(exp))
            EXPECT_TRUE(std::isnan(x));
        else
            EXPECT_FLOAT_EQ(exp, x);
    }
    EXPECT_FALSE(read(x));
}
