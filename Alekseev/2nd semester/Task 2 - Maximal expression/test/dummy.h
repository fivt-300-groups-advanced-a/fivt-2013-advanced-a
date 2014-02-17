#ifndef DUMMY_H
#define DUMMY_H

#include <algorithm>

template<class RAIter>
int dummyMaxExpression(RAIter begin, RAIter end)
{
    typedef RAIter Iter;

    int n = end - begin;

    if (n > 30)
        throw "dummy call with too long sequence";

    int best = std::accumulate(begin, end, 0);
    for (int mask = 1; mask < (1 << (n-1)); ++mask)
    {
        std::vector<int> stack;

        for (Iter it = begin; it != end; ++it)
        {
            if (it == begin || !((mask >> (it - begin - 1)) & 1))
                stack.push_back(*it);
            else
                stack.back() *= *it;
        }

        best = std::max(best, std::accumulate(stack.begin(), stack.end(), 0));
    }

    return best;
}

#endif // DUMMY_H
