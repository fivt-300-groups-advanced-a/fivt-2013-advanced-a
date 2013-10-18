#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include "gtest/gtest.h"

#include "src/io/fstreamqueue.h"

#include "complexdata.h"

TEST(FStreamQueue, Integers)
{
    FStreamQueue<int,
            typename OptimalStreamIO<int>::ReaderType,
            typename OptimalStreamIO<int>::WriterType > q0, q1, q2;

    int x;
    EXPECT_FALSE(q0.pop(x));

    static std::vector<int> v1 = {123, -43, 0, 32}, v2 = {0, 1, 2, 3};

    for (int v : v1)
        q1.push(v);

    for (size_t i = 0; i < v1.size(); ++i)
    {
        ASSERT_TRUE(q1.pop(x));
        EXPECT_EQ(v1[i], x);

        q2.push(v2[i]);
    }
    EXPECT_FALSE(q1.pop(x));

    for (int v : v2)
    {
        ASSERT_TRUE(q2.pop(x));
        EXPECT_EQ(v, x);
    }
    EXPECT_FALSE(q2.pop(x));
}

TEST(FStreamQueue, Strings)
{
    FStreamQueue<std::string,
            IStreamReader<std::string>,
            OStreamWriter<std::string>> q0, q1, q2;

    std::string x;
    EXPECT_FALSE(q0.pop(x));

    static std::vector<std::string> v1 = {"123", "-43", "0", "32"}, v2 = {"0", "1", "2", "3"};

    for (std::string v : v1)
        q1.push(v);

    for (size_t i = 0; i < v1.size(); ++i)
    {
        ASSERT_TRUE(q1.pop(x));
        EXPECT_EQ(v1[i], x);

        q2.push(v2[i]);
    }
    EXPECT_FALSE(q1.pop(x));

    for (std::string v : v2)
    {
        ASSERT_TRUE(q2.pop(x));
        EXPECT_EQ(v, x);
    }
    EXPECT_FALSE(q2.pop(x));
}

TEST(FStreamQueue, ComplexData)
{
    FStreamQueue<ComplexData,
            IStreamReader<ComplexData>,
            OStreamWriter<ComplexData>> q0, q1, q2;

    ComplexData x;
    EXPECT_FALSE(q0.pop(x));

    static std::vector<ComplexData> v1 =
    {
        {12.3, "123"},
        {-.43, "-43"},
        {0., "0"},
        {32., "32"}
    };
    static std::vector<ComplexData> v2 =
    {
        {0., "0"},
        {.1, "1"},
        {2., "2"},
        {.3, "3"}
    };

    for (ComplexData v : v1)
        q1.push(v);

    for (size_t i = 0; i < v1.size(); ++i)
    {
        ASSERT_TRUE(q1.pop(x));
        EXPECT_EQ(v1[i], x);

        q2.push(v2[i]);
    }
    EXPECT_FALSE(q1.pop(x));

    for (ComplexData v : v2)
    {
        ASSERT_TRUE(q2.pop(x));
        EXPECT_EQ(v, x);
    }
    EXPECT_FALSE(q2.pop(x));
}
