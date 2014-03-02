#ifndef SIGNS_H

#define SIGNS_H

#include <vector>
#include <algorithm>

long long getMaxSum(std::vector<long long> &numbers) {
    int sz = numbers.size();
    std::vector<long long> dp(sz + 1, 0);

    for (int i = 1; i <= sz; i++){    
        long long prod = 1;
        dp[i] = dp[i - 1] + numbers[i - 1];
        for (int j = i; j >= 1; j--){
            prod *= numbers[j - 1];
            dp[i] = std::max(dp[i], dp[j - 1] + prod);
        }
    }

    return dp[sz];
}

#endif
