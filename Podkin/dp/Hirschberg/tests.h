#ifndef TESTS_H
#define TESTS_H

#include <gtest/gtest.h>
#include <string>
#include "Hirschberg.h"
#include <stdlib.h>
#include <iostream>
#include "slowHirschberg.h"

bool check(std::string &s1, std::string &s2, std::vector<change> &ans) {
    int ind1 = 0, ind2 = 0, l1 = s1.length(), l2 = s2.length();
    for (int i = 0; i < (int)ans.size(); i++) {
        if (ans[i].type == 'M') {
            if (ind1 == l1 || ind2 == l2 || s1[ind1] != s2[ind2]) {
                return false;
            }
            ind1++, ind2++;
        }
        if (ans[i].type == 'I') {
            if (ind2 == l2 || s2[ind2] != ans[i].val) {
                return false;
            }
            ind2++;
        }
        if (ans[i].type == 'E') {
            if (ind1 == l1 || s1[ind1] != ans[i].val) {
                return false;
            }
            ind1++;
        }
        if (ans[i].type == 'R') {
            if (ind1 == l1 || ind2 == l2 || s2[ind2] != ans[i].val) {
                return false;
            }
            ind1++, ind2++;
        }
    }
    if (ind1 != l1 || ind2 != l2) {
        return false;
    }
    return true;
}

TEST(Tests, StressTest) {
    srand(42);
    int len = 10, numOfTests = 10000, i = 0;
    while (i < numOfTests) { 
        std::string s1 = "", s2 = "";
        for (int j = 0; j < len; j++) {
            s1 += rand() % 26 + 'a';
            s2 += rand() % 26 + 'a';
        }
        i++;
        std::vector<change> ans;
        getChanges(s1, s2, ans);
        int t = 0;
        for (int i = 0; i < (int)ans.size(); i++)
            if (ans[i].type != 'M')
                t++;
        ASSERT_EQ(t, getNum(s1, s2));
        ASSERT_TRUE(check(s1, s2, ans));
    }
}

#endif
