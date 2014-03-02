#ifndef MAXSUM_H
#define MAXSUM_H

#include <vector>
#include <algorithm>

long long MaxSum(std::vector<long long> &mas) {
    int n = mas.size();
    std::vector<long long> ans(n + 1);
    for (int i = 0; i <= n; i++)
        ans[i] = 0;
    ans[1] = mas[0];
    long long mult;
    for (int i = 1; i <= n; i++) {
        mult = 1;
        ans[i] = ans[i - 1] + mas[i - 1];
        for (int j = i; j > 0; j--) {
            mult *= mas[j - 1];
            ans[i] = std::max(ans[i], ans[j - 1] + mult);
        }
    }
    return ans[n];
}

#endif
