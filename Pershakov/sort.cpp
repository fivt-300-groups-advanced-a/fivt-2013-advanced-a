#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

const int DIGIT = 0;
const int LETTER = 1;
const int UNKNOWN = 2;

inline int get_type(char c){
    return (isdigit(c)? DIGIT : LETTER);
}

bool cmp_as_number(const string &a, const string &b){
    int val_a = atoi(a.c_str());
    int val_b = atoi(b.c_str());
    if (val_a < val_b)
        return true;
    if (val_a > val_b)
        return false;
    return a.length() > b.length();
}

struct token {
    string s;

    token(const string &x){
        s = x;
    }

    bool operator < (const token &t) const {
        if (get_type(s[0]) != get_type(t.s[0]))
            return (get_type(s[0]) == LETTER? true : false);
        if (get_type(s[0]) == DIGIT)
            return cmp_as_number(s, t.s);
        if (get_type(s[0]) == LETTER)
            return s < t.s;
        assert(false);
    }

};

void print(vector<token> &a){
    for (int i = 0; i < (int)a.size(); i++)
        cout << a[i].s;
    cout << endl;
}

vector<token> tokenize(string &s){
    string buf = "";
    vector<token> r;
    int cur_type = UNKNOWN;
    for (int j = 0; j < (int)s.length(); j++){
        if (get_type(s[j]) == cur_type)
            buf += s[j];
        else {
            if (cur_type != UNKNOWN)
                r.push_back(token(buf));
            buf = "";
            buf += s[j];
            cur_type = get_type(s[j]);
        }
    }
    r.push_back(token(buf));
    return r;
}

void test_tokenizer(){
    cout << "Testing tokenize\n";
    vector<token> res;
    string s;

    s = "12dfre56gf";
    res = tokenize(s);
    if (!(res.size() == 4 && 
            res[0].s == "12" && res[1].s == "dfre" && res[2].s == "56" &&
            res[3].s == "gf")){
        cout << "Bad tokenize #1" << endl;
        exit(0);
    }

    s = "fd45ggg22g";
    res = tokenize(s);
    if (!(res.size() == 5 && 
            res[0].s == "fd" && res[1].s == "45" && res[2].s == "ggg" &&
            res[3].s == "22") && res[4].s == "g"){
        cout << "Bad tokenize #2" << endl;
        exit(0);
    }

    s = "567";
    res = tokenize(s);
    if (!(res.size() == 1 && res[0].s == "567")){
        cout << "Bad tokenize #4" << endl;
        exit(0);
    }

    s = "dsfa";
    res = tokenize(s);
    if (!(res.size() == 1 && res[0].s == "dsfa")){
        cout << "Bad tokenize #5" << endl;
        exit(0);
    }

    cout << "Testing tokenize: OK\n";    
}

void test_cmp_as_number(){
    cout << "Testing cmp_as_number\n";
    
    if (cmp_as_number("3", "3")){
        cout << "Bad cmp_as_number #1\n";
        exit(0);
    }

    if (cmp_as_number("4", "3")){
        cout << "Bad cmp_as_number #2\n";
        exit(0);
    }

    if (!cmp_as_number("3", "4")){
        cout << "Bad cmp_as_number #3\n";
        exit(0);
    }

    if (!cmp_as_number("3", "33")){
        cout << "Bad cmp_as_number #4\n";
        exit(0);
    }

    if (cmp_as_number("33", "3")){
        cout << "Bad cmp_as_number #5\n";
        exit(0);
    }

    if (cmp_as_number("3", "03")){
        cout << "Bad cmp_as_number #6\n";
        exit(0);
    }
    
    if (!cmp_as_number("03", "3")){
        cout << "Bad cmp_as_number #7\n";
        exit(0);
    }

    cout << "Testing cmp_as_number: OK\n";
}

void test_get_type(){
    cout << "Testing get_type\n";

    if (get_type('3') != DIGIT){
        cout << "Bad get_type #1\n";
        exit(0);
    }

    if (get_type('a') != LETTER){
        cout << "Bad get_type #2\n";
        exit(0);
    }

    cout << "Testing get_type: OK\n";
}

void test_cmp_token(){
    cout << "Testing cmp_token\n";
}

void unit_test(){
    test_get_type();
    test_tokenizer();
    test_cmp_as_number();
    test_cmp_token();
}

vector<vector<token> > a;

int main(){
    unit_test();
    int cnt;
    cin >> cnt;
    string s;
    for (int i = 0; i < cnt; i++){
        cin >> s;
        a.push_back(tokenize(s));
    }
    
    sort(a.begin(), a.end());
    cout << endl;
    for (int i = 0; i < cnt; i++)
        print(a[i]);
}
