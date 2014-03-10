#ifndef EDITDISTANCE_H
#define EDITDISTANCE_H

#include <vector>
#include <functional>
#include <algorithm>

//#include <iostream>
//#include <string>
//using std::cerr;

namespace
{

template<class RAIter, class Equal>
std::vector<std::size_t> calcPartial(RAIter srcBegin, RAIter srcEnd,
                                      RAIter destBegin, RAIter destEnd,
                                      Equal equal, bool reverse = false)
{
    std::size_t n = srcEnd - srcBegin, m = destEnd - destBegin;

    std::vector<std::size_t> prevDp(m+1), newDp(m+1);
    std::iota(prevDp.begin(), prevDp.end(), 0);

    RAIter src = reverse ? srcEnd - 1 : srcBegin;
    for (std::size_t i = 1; i <= n; ++i, src += (reverse ? -1 : 1))
    {
        newDp[0] = i;
        RAIter dest = reverse ? destEnd - 1 : destBegin;
        for (std::size_t j = 1; j <= m; ++j, dest += (reverse ? -1 : 1))
        {
            newDp[j] = equal(*src, *dest) ?
                        prevDp[j-1] :
                        std::min(newDp[j-1], std::min(prevDp[j-1], prevDp[j])) + 1;
        }

        prevDp.swap(newDp);
    }
    if (reverse)
        std::reverse(prevDp.begin(), prevDp.end());

    return prevDp;
}

struct EditInstruction
{
    enum Type
    {
        INSERT,
        ERASE,
        REPLACE
    };

    EditInstruction(Type type, std::size_t position, std::size_t newCharPosition = 0):
        type(type), oldCharPosition(position), newCharPosition(newCharPosition)
    {}

    Type type;
    std::size_t oldCharPosition;
    std::size_t newCharPosition;
};

template<class RAIter,
         class Equal = std::equal_to<typename std::iterator_traits<RAIter>::value_type > >
std::vector< EditInstruction> editPrescription(
        RAIter srcBegin, RAIter srcEnd,
        RAIter destBegin, RAIter destEnd,
        Equal equal = std::equal_to<typename std::iterator_traits<RAIter>::value_type >())
{
    std::size_t n = srcEnd - srcBegin, m = destEnd - destBegin;

    if (n == 0)
    {
        std::vector<EditInstruction> ret;
        for (std::size_t i = m-1; i < m; --i)
            ret.emplace_back(EditInstruction::INSERT, 0, i);
        return ret;
    }

    if (n == 1)
    {
        std::vector<EditInstruction> ret;
        if (!m)
            ret.emplace_back(EditInstruction::ERASE, 0);
        else
        {
            bool saved = false;
            RAIter dest = destEnd - 1;
            for (size_t i = m-1; i < m; --i, --dest)
            {
                if (!saved && equal(*srcBegin, *dest))
                    saved = true;
                else if (!saved && !i)
                    ret.emplace_back(EditInstruction::REPLACE, 0, i);
                else
                    ret.emplace_back(EditInstruction::INSERT, saved ? 0 : 1, i);
            }
        }
        return ret;
    }

    std::size_t halfSize = n / 2;
    RAIter srcMid = srcBegin + halfSize;
    std::size_t splitPlace = 0;
    {
        std::vector<std::size_t> prefixes = calcPartial(srcBegin, srcMid, destBegin, destEnd, equal);
        std::vector<std::size_t> suffixes = calcPartial(srcMid, srcEnd, destBegin, destEnd, equal, true);

        for (std::size_t i = 1; i <= m; ++i)
            if (prefixes[i] + suffixes[i] < prefixes[splitPlace] + suffixes[splitPlace])
                splitPlace = i;
    }
//    cerr << "split place:" << splitPlace << "\n";

    RAIter destMid = destBegin + splitPlace;

    std::vector<EditInstruction> left = editPrescription(srcBegin, srcMid, destBegin, destMid, equal);
    std::vector<EditInstruction> right = editPrescription(srcMid, srcEnd, destMid, destEnd, equal);

    for (EditInstruction& i : right)
    {
        i.oldCharPosition += halfSize;
        i.newCharPosition += splitPlace;
    }
    right.insert(right.end(), left.begin(), left.end());

    return right;
}

}

#endif // EDITDISTANCE_H
