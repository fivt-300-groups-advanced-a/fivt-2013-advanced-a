#ifndef SLOWMAXSUM_H
#define SLOWMAXSUM_H

#include <vector>
#include <algorithm>

long long SlowMaxSum(std::vector <long long> &mas) {
    long long ans = 0;
    int n = mas.size();
    for (int i = 0; i < n; i++) {
        ans += mas[i];
    }
    for (int i = 0; i < (1 << (n - 1)); i++) {
        long long cur_ans = 0, cur_mult = mas[0];
        for (int j = 0; j < n - 1; j++) {
            if (i & (1 << j)) {
                cur_mult *= mas[j + 1];
            }
            else {
                cur_ans += cur_mult;
                cur_mult = mas[j + 1];
            }
        }
        cur_ans += cur_mult;
        ans = std::max(ans, cur_ans);
    }
    return ans;
}

#endif
