#ifndef MAXEXPRESSION_H
#define MAXEXPRESSION_H

#include <vector>

template<class BidIter>
int maxExpression(BidIter begin, BidIter end)
{
    typedef BidIter Iter;

    if (begin == end)
        return 0;

    std::vector<int> maxPrefixValue(1, 0);

    for (Iter it = begin; it != end; ++it)
    {
        int best = maxPrefixValue.back() + *it;

        int mul = 1;

        std::vector<int>::reverse_iterator prefIt = maxPrefixValue.rbegin();
        for (Iter suffIt = it; prefIt != maxPrefixValue.rend(); --suffIt, ++prefIt)
        {
            mul *= *suffIt;
            best = std::max(best, mul + *prefIt);
        }

        maxPrefixValue.push_back(best);
    }

    return maxPrefixValue.back();
}

#endif // MAXEXPRESSION_H
