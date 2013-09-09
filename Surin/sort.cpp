#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cassert>
#include <iterator>
#include <sstream>
#include <cctype>

using namespace std;

#define INF 1e+9

vector<string> arr;

struct ttype {
    bool str;
    int nm;
    string sm;
    ttype(string & s) {
        if (isdigit(s[0])) {
            stringstream ss;
            int nnm;
            ss << s;
            ss >> nm;
            sm = "";
            str = false;
            return;
        }
        sm = s;
        nm = 0;
        str = true;
    }
    ttype(int s) {
        nm = s;
        sm = "";
        str = false;
    }
};

bool operator > (const ttype a, const ttype & b) {
    if (a.str < b.str) return true;
    if (a.str > b.str) return false;
    if (a.sm > b.sm) return true;
    if (b.sm < b.sm) return false;
    if (a.nm > b.nm) return true;
    if (b.nm < b.nm) return false;
    return false;
}

bool operator == (const ttype a, const ttype b) {
    return a.str == b.str && a.nm == b.nm && a.sm == b.sm;
}

bool operator < (const ttype a, const ttype b) {
    return !((a == b) || (a > b));
}

vector<ttype> conv(string s) {
    vector<ttype> v;
    if (s.empty()) return v;
    string ccur; ccur.push_back(s[0]);
    bool dig = isdigit(s[0]);
    for (int i = 1; i < s.length(); i++) {
        if (dig != isdigit(s[i]) && !(!dig && s[i] == '0')) {
            v.push_back(ttype(ccur));
            ccur.clear();
            dig = !dig;
        }
        ccur.push_back(s[i]);
    }
    v.push_back(ttype(ccur));
}

vector<vector<ttype> > sseq;

ostream & operator << (ostream & outp, const ttype x) {
    if (x.str) {
        outp << x.sm;
        return outp;
    } else {
        outp << x.nm;
        return outp;
    }
}

ostream & operator << (ostream & outp, const vector<ttype> x) {
   copy(x.begin(), x.end(), ostream_iterator<ttype>(outp, ""));
   return outp;
}

int main() {
    ios_base::sync_with_stdio(false);
    freopen("inp", "r", stdin);
    freopen("outp", "w", stdout);
    do {
        string s;
        getline(cin, s);
        if (cin)
        sseq.push_back(conv(s));
    } while (!cin.eof());
    sort(sseq.begin(), sseq.end());
    copy(sseq.begin(), sseq.end(), ostream_iterator<vector<ttype> >(cout, "\n"));
}
