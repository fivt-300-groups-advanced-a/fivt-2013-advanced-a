#ifndef UTILS_H
#define UTILS_H

#include <algorithm>

#include "gtest/gtest.h"

#include "adjacency.h"

class AdjacencyTest : public ::testing::Test
{
protected:
    void checkEquivalence(Adjacency *adj, std::vector<std::size_t> src)
    {
        ASSERT_EQ(src.size(), adj->size());

        std::sort(src.begin(), src.end());

        std::unique_ptr<AdjacencyIterator> it = adj->makeIterator();

        for (std::size_t value : src)
        {
            ASSERT_TRUE(it->isValid());
            ASSERT_EQ(value, it->destination());
            ASSERT_TRUE(it->advance());
        }
        ASSERT_FALSE(it->isValid());
    }
};

#endif // UTILS_H
