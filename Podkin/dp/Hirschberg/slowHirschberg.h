#ifndef SLOWHIRSCHBERG_H
#define SLOWHIRSCHBERG_H

#include <string>
#include <algorithm>

int getNum(std::string &s1, std::string &s2) {
    std::vector <std::vector<int> > dp(s1.length() + 1);
    for (int i = 0; i < (int)s1.length() + 1; i++) {
        dp[i] = std::vector<int>(s2.length() + 1);
    }
    for (int i = 0; i <= (int)s1.length(); i++) {
        dp[i][0] = i;
    }
    for (int i = 0; i <= (int)s2.length(); i++) {
        dp[0][i] = i;
    }
    for (int i = 1; i <= (int)s1.length(); i++) {
        for (int j = 1; j <= (int)s2.length(); j++) {
            dp[i][j] = std::min(dp[i - 1][j], dp[i][j - 1]) + 1;
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = std::min(dp[i][j], dp[i - 1][j - 1]);
            }
            else {
                dp[i][j] = std::min(dp[i][j], dp[i - 1][j - 1] + 1);
            }
        }
    }
    return dp[(int)s1.length()][(int)s2.length()];
}

#endif
