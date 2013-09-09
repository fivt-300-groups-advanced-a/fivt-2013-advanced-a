#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cassert>
#include <iterator>
#include <sstream>
#include <cctype>

namespace functions {
    

    using namespace std;

    #define INF 1e+9

    vector<string> arr;

    struct ttype {
        bool str;
        int nm;
        string sm;
        ttype(bool _str, int _nm, string _sm) {
            str = _str;
            nm = _nm;
            sm = _sm;
        }
        ttype(const string & s) {
            if (!s.empty() && isdigit(s[0])) {
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

    bool operator > (const ttype & a, const ttype & b) {
        if (a.str < b.str) return true;
        if (a.str > b.str) return false;
        if (a.sm > b.sm) return true;
        if (b.sm < b.sm) return false;
        if (a.nm > b.nm) return true;
        if (b.nm < b.nm) return false;
        return false;
    }

    bool operator == (const ttype & a, const ttype & b) {
        return a.str == b.str && a.nm == b.nm && a.sm == b.sm;
    }

    bool operator < (const ttype & a, const ttype & b) {
        return !((a == b) || (a > b));
    }
    
    vector<string> split(const string & s) {
        vector<string> v;
        if (s.empty()) return v;
        string ccur; ccur.push_back(s[0]);
        bool dig = isdigit(s[0]);
        for (int i = 1; i < s.length(); i++) {
            if (dig != isdigit(s[i])) {
                v.push_back(ccur);
                ccur.clear();
                dig = !dig;
            }
            ccur.push_back(s[i]);
        }
        v.push_back(ccur);
    }

    vector<ttype> mapTtype(vector<string> v) {
        vector<ttype> r;
        for (int i = 0; i < v.size(); i++)
            r.push_back(ttype(v[i]));
        return r;
    }

    vector<ttype> conv(const string & s) {
        return mapTtype(split(s));
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
}

