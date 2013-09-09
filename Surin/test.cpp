#include "functions.h"

using namespace functions;

bool testTTgreater() {
   if (!(ttype("123") > ttype("ab"))) return false;
   if (!(ttype("ba") > ttype("ab"))) return false;
   if (!(ttype("ab") < ttype("bba"))) return false;
   return true;
}

bool testSplit() {
    if (split("123ab") != vector<string>({"123", "ab"})) return false;
    if (split("abbsd126635178bnbashdg1132788JHDSjka") != vector<string>({"abbsd", "126635178", "bnbashdg", "1132788", "JHDSjka"}))
        return false;
    if (split("ab0ab") != vector<string>({"ab", "0", "ab"})) return false;
    return true;
}

bool testTTconstructor() {
   for (int i = 0; i < 300; i++) {
       stringstream ss;
       int buf = rand();
       ss << buf;
       if (!(ttype(ss.str()) == ttype(false, buf, ""))) return false;
   }
   return true;
}

int main() {
    if (!testSplit())
        cout << "FAILED SPLIT";
    else if (!testTTconstructor())
        cout << "FAILED CONSTRUCTOR";
    else if (!testTTgreater())
        cout << "FAILED TGREATER";
    else cout << "TESTS COMPLETE";
    cout << "\n";
}
