#ifndef STUPID_SIGNS_H

#define STUPID_SIGNS_H

#include <vector>
#include <climits>

long long stupidGetMaxSum(std::vector<long long> &numbers) {
    int sz = numbers.size();
    long long res = LLONG_MIN;
    for (int msk = 0; msk < (1 << (sz - 1)); msk++) {
        long long cur_sum = 0;
        long long cur_prod = numbers[0];
        for (int i = 0; i < sz - 1; i++)
            if (msk & (1 << i))
                cur_prod *= numbers[i + 1];
            else {
                cur_sum += cur_prod;
                cur_prod = numbers[i + 1];
            }
        res = std::max(res, cur_sum + cur_prod);
    }
    return res;
}

#endif
