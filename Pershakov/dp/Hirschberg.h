#ifndef HIRSCHBERG

#define HIRSCHBERG

#include <string>
#include <vector>

enum changeType {
    INSERT, ERASE, REPLACE, MATCH
};

struct change {
    changeType type;
    char val;

    change() { }

    change(const changeType &new_type) {
        type = new_type;
    }

    change(const changeType &new_type, char new_val) {
        type = new_type;
        val = new_val;
    }
};

void initDP(std::vector<int> &dp) {
    for (int i = 0; i < (int)dp.size(); i++)
        dp[i] = i;
}

void initRevDP(std::vector<int> &dp) {
    for (int i = 0; i < (int)dp.size(); i++)
        dp[i] = dp.size() - i - 1;
}

void addToVector(std::vector<change> &to, const std::vector<change> &from) {
    for (int i = 0; i < (int)from.size(); i++)
        to.push_back(from[i]);
}

std::vector<change> insertString(const std::string &str, int l, int r) {
    std::vector<change> ans(std::max(r - l, 0));
    for (int i = l; i < r; i++) {
        ans[i - l] = change(INSERT, str[i]);
    }
    return ans;
}

std::vector<change> eraseString(const std::string &str, int l, int r) {
    std::vector<change> ans(std::max(r - l, 0));
    for (int i = l; i < r; i++)
        ans[i - l] = change(ERASE);
    return ans;
}

std::vector<change> buildPrescription(const std::string &str1, int l1, int r1, 
                                      const std::string &str2, int l2, int r2) {
    if (l1 == r1) {
        return insertString(str2, l2, r2);
    }
    if (l2 == r2) {
        return eraseString(str1, l1, r1);
    }
    if (l1 + 1 == r1) {
        for (int i = l2; i < r2; i++)
            if (str2[i] == str1[l1]) {
                std::vector<change> ans = insertString(str2, l2, i);
                ans.push_back(change(MATCH));
                addToVector(ans, insertString(str2, i + 1, r2));
                return ans;
            }
        std::vector<change> ans;
        ans.push_back(change(REPLACE, str2[l2]));
        addToVector(ans, insertString(str2, l2 + 1, r2));
        return ans;
    }

    int len1 = r1 - l1, len2 = r2 - l2;
    int mid1 = (len1 + 1) / 2;
    std::vector<int> pref(len2 + 1);
    std::vector<int> suf(len2 + 1);
    std::vector<int> prev(len2 + 1);

    // for prefixes
    initDP(prev);
    for (int i = 1; i <= mid1; i++) {
        pref[0] = i;
        for (int j = 1; j < len2 + 1; j++) {
            pref[j] = std::min(pref[j - 1], prev[j]) + 1;
            if (str1[i + l1 - 1] == str2[j + l2 - 1])
                pref[j] = std::min(pref[j], prev[j - 1]);
            else
                pref[j] = std::min(pref[j], prev[j - 1] + 1);
        }
        pref.swap(prev);
    }
    pref = prev;

    // for suffixes
    initRevDP(prev);
    for (int i = len1; i > mid1; i--) {
        suf[len2] = len1 - i + 1;
        for (int j = len2 - 1; j >= 0; j--) {
            suf[j] = std::min(suf[j + 1], prev[j]) + 1;
            if (str1[i + l1 - 1] == str2[j + l2])
                suf[j] = std::min(suf[j], prev[j + 1]);
            else
                suf[j] = std::min(suf[j], prev[j + 1] + 1);
        }
        suf.swap(prev);
    }
    suf = prev;

    // merging
    int mid2 = 0;
    for (int i = 0; i < len2 + 1; i++)
        if (pref[i] + suf[i] < pref[mid2] + suf[mid2])
            mid2 = i;
    suf.clear(), pref.clear(), prev.clear();

    std::vector<change> res1 = buildPrescription(str1, l1, l1 + mid1, 
                                                 str2, l2, l2 + mid2);
    std::vector<change> res2 = buildPrescription(str1, l1 + mid1, r1, 
                                                 str2, l2 + mid2, r2);
    addToVector(res1, res2);
    return res1;
}

std::vector<change> getEditPrescription(const std::string &str1, 
                                        const std::string &str2) {
    return buildPrescription(str1, 0, str1.size(), 
                                str2, 0, str2.size()); 
}

#endif
