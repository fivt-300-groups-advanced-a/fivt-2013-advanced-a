#ifndef INFORMATICSTEST_H
#define INFORMATICSTEST_H

#include <algorithm>
#include <a.out.h>vector>

#include <iostream>
#include <string>
#include <functional>

using namespace std;

template<class RAIter, class Equal>
std::vector<std::size_t> calcPrefixes(RAIter srcBegin, RAIter srcEnd,
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

int main()
{
    string a, b;
    cin >> a >> b;

    cout << calcPrefixes(a.begin(), a.end(), b.begin(), b.end(), equal_to<char>(), true)[0] << endl;

    return 0;
}

#endif // INFORMATICSTEST_H
