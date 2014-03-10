#ifndef UTILS_H
#define UTILS_H

#include <random>

#include "../src/EditDistance.h"

#include "gtest/gtest.h"

namespace
{
    template<class RAIter, class Equal>
    void checkAnswer(std::size_t ans,
                     RAIter srcBegin, RAIter srcEnd,
                     RAIter destBegin, RAIter destEnd,
                     Equal equal)
    {
        ASSERT_EQ(calcPartial(srcBegin, srcEnd, destBegin, destEnd, equal).back(), ans);
    }

    template<class RAIter, class Equal>
    void checkPrescription(RAIter srcBegin, RAIter srcEnd,
                           RAIter destBegin, RAIter destEnd,
                           Equal equal)
    {
        std::vector<EditInstruction> inst = editPrescription(srcBegin, srcEnd, destBegin, destEnd, equal);
        checkAnswer(inst.size(), srcBegin, srcEnd, destBegin, destEnd, equal);

//        std::size_t srcSize = srcEnd - srcBegin;
        std::size_t destSize = destEnd - destBegin;

        std::vector<RAIter> result;
        for (RAIter it = srcBegin; it != srcEnd; ++it)
            result.push_back(it);

        for (const EditInstruction &i : inst)
        {
            switch (i.type)
            {
            case EditInstruction::INSERT:
                ASSERT_LE(i.oldCharPosition, result.size());
                ASSERT_LT(i.newCharPosition, destSize);
                result.insert(result.begin() + i.oldCharPosition, destBegin + i.newCharPosition);
                break;
            case EditInstruction::ERASE:
                ASSERT_LT(i.oldCharPosition, result.size());
                result.erase(result.begin() + i.oldCharPosition);
                break;
            case EditInstruction::REPLACE:
                ASSERT_LT(i.oldCharPosition, result.size());
                ASSERT_LT(i.newCharPosition, destSize);
                *(result.begin() + i.oldCharPosition) = destBegin + i.newCharPosition;
                break;
            }
        }

        ASSERT_EQ(result.size(), destSize);
        for (std::size_t i = 0; i < destSize; ++i)
            EXPECT_TRUE(equal(*result[i], *(destBegin + i)));
    }
}

struct TestCase
{
    TestCase()
    {}

    TestCase(const std::vector<int> &a, const std::vector<int> &b):
        src(a), dest(b)
    {}

    void checkPrescription() const
    {
        return ::checkPrescription(src.begin(), src.end(), dest.begin(), dest.end(), std::equal_to<int>());
    }

    static TestCase random(int seed, int minSize, int maxSize, int alph)
    {
        std::mt19937 gen(seed);
        return TestCase(randomSequence(gen, minSize, maxSize, alph),
                        randomSequence(gen, minSize, maxSize, alph));
    }

    static TestCase random2(int seed, int minSize, int maxSize, int alph, int minDist, int maxDist)
    {
        std::mt19937 gen(seed);

        std::vector<int> src = randomSequence(gen, minSize, maxSize, alph);
        std::vector<int> dest = src;

        int dist = minDist + gen() % (maxDist - minDist + 1);
        for (int i = 0; i < dist; ++i)
        {
            EditInstruction::Type type = static_cast<EditInstruction::Type>(gen() % 3);
            if (dest.empty())
                type = EditInstruction::INSERT;

            switch (type) {
            case EditInstruction::INSERT:
                dest.insert(dest.begin() + gen() % (dest.size() + 1), gen() % alph);
                break;
            case EditInstruction::ERASE:
                dest.erase(dest.begin() + gen() % dest.size());
                break;
            case EditInstruction::REPLACE:
                dest[gen() % dest.size()] = gen() % alph;
                break;
            }
        }

        return TestCase(src, dest);
    }

private:
    static std::vector<int> randomSequence(std::mt19937 &gen, int minSize, int maxSize, int alph)
    {
        std::vector<int> ret(minSize + gen() % (maxSize - minSize + 1));

        for (int& c : ret)
            c = gen() % alph;

        return ret;
    }

public:

    std::vector<int> src, dest;
};

#endif // UTILS_H
