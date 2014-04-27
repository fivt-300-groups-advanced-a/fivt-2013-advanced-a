#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <string.h>
#include "diff.h"
#include <gtest/gtest.h>

using namespace std;

int stupid_solve(vector<int> &a, vector<int> &b, int change, int add, int rem)
{

    vector<vector<int> > dp(b.size() + 1, vector<int>(a.size() + 1));
    for (int i = 0; i <= b.size(); ++i)
        dp[i][0] = i * rem;
    for (int i = 1; i <= a.size(); ++i)
    {
        dp[0][i] = dp[0][i - 1] + add;

        for (int j = 1; j <= b.size(); ++j)
        {
            dp[j][i] = dp[j][i - 1] + add;
            dp[j][i] = min(dp[j][i], dp[j - 1][i] + rem);
            dp[j][i] = min(dp[j][i], dp[j - 1][i - 1] + (a[i - 1] == b[j - 1] ? 0 : change));
        }
    }
    return dp[b.size()][a.size()];

}

TEST (stress, some_stress_100_pack_len_100)
{
    const int maxlen = 100;
    const int maxvalue = 10;
    for (int qt = 0; qt < 100; qt++)
    {
        vector<int> a(rand() % maxlen), b(rand() % maxlen);
        for (int i = 0; i < a.size(); ++i)
            a[i] = rand() % maxvalue;
        for (int i = 0; i < b.size(); ++i)
            b[i] = rand() % maxvalue;
        diff<int> d;
        vector<int> ans = d.make_diff(a, b);
        int ai = 0, bi = 0;
        for (int i = 0; i < ans.size(); ++i)
        {
            if (ans[i] == 2)
                ai++;
            else if (ans[i] == 3)
                bi++;
            else if (ans[i] == 0 && a[ai] == b[bi])
            {
                ai++;
                bi++;
            }
            else if (ans[i] == 1 && a[ai] != b[bi])
            {
                ai++;
                bi++;
            }
            else
                EXPECT_TRUE(false);
        }
        int k = 0;
        for (int i = 0; i < ans.size(); ++i)
            if (ans[i])
                ++k;
        EXPECT_EQ(ai, a.size());
        EXPECT_EQ(bi, b.size());
        EXPECT_EQ(stupid_solve(a, b, 1, 1, 1), k);
    }
}


TEST (stress, some_stress_100_pack_len_1000)
{
    const int maxlen = 1000;
    const int maxvalue = 20;
    for (int qt = 0; qt < 1; qt++)
    {
        vector<int> a(10000), b(10000);
        for (int i = 0; i < a.size(); ++i)
            a[i] = rand() % maxvalue;
        for (int i = 0; i < b.size(); ++i)
            b[i] = rand() % maxvalue;
        diff<int> d;
        vector<int> ans = d.make_diff(a, b);
        int ai = 0, bi = 0;
        for (int i = 0; i < ans.size(); ++i)
        {
            if (ans[i] == 2)
                ai++;
            else if (ans[i] == 3)
                bi++;
            else if (ans[i] == 0 && a[ai] == b[bi])
            {
                ai++;
                bi++;
            }
            else if (ans[i] == 1 && a[ai] != b[bi])
            {
                ai++;
                bi++;
            }
            else
                EXPECT_TRUE(false);
        }
        int k = 0;
        for (int i = 0; i < ans.size(); ++i)
            if (ans[i])
                ++k;
        EXPECT_EQ(ai, a.size());
        EXPECT_EQ(bi, b.size());
        //EXPECT_EQ(stupid_solve(a, b, 1, 1, 1), k);
    }
}




int main(int argc, char **argv)
{
    //srand(70044);
    srand(time(NULL));
    testing :: InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
