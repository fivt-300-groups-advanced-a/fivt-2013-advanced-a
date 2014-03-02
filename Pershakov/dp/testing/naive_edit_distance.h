#ifndef NAIVE_EDIT_DISTANCE

#define NAIVE_EDIT_DISTANCE

#include <string>
#include <algorithm>

int naiveEditDistance(const std::string &str1, const std::string &str2) {
    std::vector<std::vector<int> > dp(str1.length() + 1);
    for (int i = 0; i < (int)str1.length() + 1; i++)
        dp[i] = std::vector<int>(str2.length() + 1);
    for (int i = 0; i <= (int)str1.length(); i++)
        for (int j = 0; j <= (int)str2.length(); j++) {
            if (i == 0)
                dp[i][j] = j;
            else if (j == 0)
                dp[i][j] = i;
            else {
                dp[i][j] = std::min(dp[i - 1][j] + 1, dp[i][j - 1] + 1);
                if (str1[i - 1] == str2[j - 1])
                    dp[i][j] = std::min(dp[i][j], dp[i - 1][j - 1]);
                else
                    dp[i][j] = std::min(dp[i][j], dp[i - 1][j - 1] + 1);
            }
        }
    return dp[str1.length()][str2.length()];
}

#endif
