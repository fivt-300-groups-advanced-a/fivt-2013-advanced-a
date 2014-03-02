#include <gtest/gtest.h>
#include <cstdlib>
#include <vector>
#include <string>

#include "../signs.h"
#include "stupid_signs.h"

#include "../Hirschberg.h"
#include "naive_edit_distance.h"

TEST(SignsTesting, StressTest) {
    int sz = 18;
    int cnt_tests = 100;
    std::vector<long long> numbers(sz);
    for (int test = 0; test < cnt_tests; test++) {
        for (int i = 0; i < sz; i++)
            numbers[i] = rand() % 20 - 10;
        ASSERT_EQ(getMaxSum(numbers), stupidGetMaxSum(numbers));
    }
}

std::string randString(int len) {
    std::string ans = "";
    for (int i = 0; i < len; i++)
        ans += char('a' + rand() % 10);
    return ans;
}

bool isCorrect(const std::vector<change> &ans, 
        const std::string str1, const std::string str2) {
    int ptr1 = 0, ptr2 = 0;
    int len1 = str1.length(), len2 = str2.length();
    for (int i = 0; i < (int)ans.size(); i++) {
        switch (ans[i].type) {
            case MATCH:
                if (ptr1 == len1 || ptr2 == len2 || str1[ptr1] != str2[ptr2])
                    return false;
                ptr1++, ptr2++;
                break;
            case ERASE:
                if (ptr1 == len1)
                    return false;
                ptr1++;
                break;
            case INSERT:
                if (ptr2 == len2 || str2[ptr2] != ans[i].val)
                    return false;
                ptr2++;
                break;
            case REPLACE:
                if (ptr1 == len1 || ptr2 == len2 || str2[ptr2] != ans[i].val)
                    return false;
                ptr1++, ptr2++;
                break;
        }
    }
    return ptr1 == len1 && ptr2 == len2;
}

TEST(HirschbergTesting, StressTest) {
    int cnt_tests = 10000;
    int max_len = 100;
    for (int test = 0; test < cnt_tests; test++) {
        int len1 = rand() % max_len, len2 = rand() % max_len;
        std::string str1 = randString(len1);
        std::string str2 = randString(len2);
        int dist = naiveEditDistance(str1, str2);
        std::vector<change> ans = getEditPrescription(str1, str2);
        
        int dist_to_check = 0;
        for (int i = 0; i < (int)ans.size(); i++)
            if (ans[i].type != MATCH)
                dist_to_check++;
        ASSERT_EQ(dist, dist_to_check);

        ASSERT_TRUE(isCorrect(ans, str1, str2));
    }
}

int main(int argc, char *argv[]) {
    srand(42);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
