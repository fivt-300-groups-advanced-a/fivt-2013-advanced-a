#ifndef ISTREAMREADERTEST_H
#define ISTREAMREADERTEST_H

#include <sstream>

#include "gtest/gtest.h"

#include "../SortExt/io/istreamreader.h"

TEST(IStreamReader, Integers)
{
    std::istringstream iss("12354 54\n\t45\n344423");
    IStreamReader<int> reader(iss);

    int x;
    EXPECT_TRUE(reader(x));
    EXPECT_EQ(x, 12354);

    EXPECT_TRUE(reader(x));
    EXPECT_EQ(x, 54);
    EXPECT_TRUE(reader(x));
    EXPECT_EQ(x, 45);
    EXPECT_TRUE(reader(x));
    EXPECT_EQ(x, 344423);
    EXPECT_FALSE(reader(x));
}

TEST(IStreamReader, EmptyInput)
{
    std::istringstream iss;
    IStreamReader<int> reader(iss);
    int x;
    EXPECT_FALSE(reader(x));
}

#endif // ISTREAMREADERTEST_H
