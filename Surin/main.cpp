#include "functions.h"

using namespace functions;

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
