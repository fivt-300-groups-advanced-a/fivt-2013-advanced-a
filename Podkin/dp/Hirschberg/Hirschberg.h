#ifndef HIRSCHBERG_H
#define HIRSCHBERG_H

#include <string>
#include <vector>
#include <math.h>
#include <algorithm>
#include <string.h>

struct change {
    char val, type;
    change(){}
    
    explicit change(char new_type) {
        type = new_type;
    }
    
    explicit change(char new_type, char new_val) {
        val = new_val;
        type = new_type;
    }
};

void insert(std::string &s, int start, int finish, std::vector<change>& ans) {
    for (int i = start; i < finish; i++) {
        ans.push_back(change('I', s[i]));
    }
}

void erase(std::string &s, int start, int finish, std::vector<change>& ans) {
    for (int i = start; i < finish; i++) {
        ans.push_back(change('E', s[i]));
    }
}

void getAns(std::string &s1, int s1_s, int s1_f, std::string &s2, int s2_s, int s2_f, std::vector<change> &ans) {
    if (s2_s == s2_f) {
        erase(s1, s1_s, s1_f, ans);
        return;
    }
    
    if (s1_s == s1_f) {
        insert(s2, s2_s, s2_f, ans);
        return;
    }
    
    if (s1_f - s1_s == 1) {
        for (int i = s2_s; i < s2_f; i++) {
            if (s1[s1_s] == s2[i]) {
                insert(s2, s2_s, i, ans);
                ans.push_back(change('M', s1[s1_s]));
                insert(s2, i + 1, s2_f, ans);
                return;
            }
        }
        ans.push_back(change('R', s2[s2_s]));
        insert(s2, s2_s + 1, s2_f, ans);
        return;
    }
    
    int m1 = (s1_f - s1_s + 1) / 2, m2 = 0, l1 = s1_f - s1_s, l2 = s2_f - s2_s;
    std::vector<int> first_side(l2 + 1);
    std::vector<int> second_side(l2 + 1);
    std::vector<int> last(l2 + 1);

    for (int i = 0; i < l2 + 1; i++)
        last[i] = i;
    for (int i = 1; i <= m1; i++) {
        first_side[0] = i;
        for (int j = 1; j < l2 + 1; j++) {
            first_side[j] = std::min(first_side[j - 1], last[j]) + 1;
            if (s1[s1_s + i - 1] == s2[s2_s + j - 1]) {
                first_side[j] = std::min(first_side[j], last[j - 1]);
            }
            else {
                first_side[j] = std::min(first_side[j], last[j - 1] + 1);
            }
        }
        if (i < m1)
            last.swap(first_side);
    }

    for (int i = l2; i >= 0; i--) {
        last[i] = l2 - i;
    }
    for (int i = l1; i > m1; i--) {
        second_side[l2] = l1 - i + 1;
        for (int j = l2 - 1; j >= 0; j--) {
            second_side[j] = std::min(second_side[j + 1], last[j]) + 1;
            if (s1[s1_s + i - 1] == s2[s2_s + j]) {
                second_side[j] = std::min(second_side[j], last[j + 1]);
            }
            else {
                second_side[j] = std::min(second_side[j], last[j + 1] + 1);
            }
        }
        if (i > m1 + 1)
            last.swap(second_side);
    }

    for (int i = 0; i <= l2; i++) {
        if (first_side[i] + second_side[i] < first_side[m2] + second_side[m2]) {
            m2 = i;
        }
    }
    first_side.clear();
    second_side.clear();
    last.clear();

    getAns(s1, s1_s, s1_s + m1, s2, s2_s, s2_s + m2, ans);
    getAns(s1, s1_s + m1, s1_f, s2, s2_s + m2, s2_f, ans);
}

void getChanges(std::string &first_str, std::string &second_str, std::vector<change> &ans) {
    ans.clear();
    getAns(first_str, 0, first_str.length(), second_str, 0, second_str.length(), ans);
}

#endif
