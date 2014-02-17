#include <sstream>

#include <cmath>

#include "gtest/gtest.h"

#include "src/io/rawostreamwriter.h"

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

TEST(RawOStreamWriter, Integers)
{
    std::initializer_list<int> data = {3, 2554, 1<<30, 1<<31, 0, 0, 43};
    std::ostringstream oss;

    do
    {
        RawOStreamWriter<int> write(oss);

        for (int x : data)
            write(x);
    } while (false);

    EXPECT_EQ(toRawString(data), oss.str());
}

TEST(RawOStreamWriter, Doubles)
{
    std::initializer_list<double> data = {M_PI, 54, 0., -1., -M_PI_4, 34e50, 0./0., 0e-3, 9e-20};
    std::ostringstream oss(toRawString(data));

    do
    {
        RawOStreamWriter<double> write(oss);

        for (double x : data)
            write(x);
    } while (false);

    EXPECT_EQ(toRawString(data), oss.str());
}
