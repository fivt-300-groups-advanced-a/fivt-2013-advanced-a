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

class Token {

    string token;

public:

    explicit Token(const string &val){
        token = val;
    }

    bool operator < (const Token &t) const {
        if (get_type(token[0]) != get_type(t.token[0]))
            return (get_type(token[0]) == LETTER? true : false);
        if (get_type(token[0]) == DIGIT)
            return cmp_as_number(token, t.token);
        if (get_type(token[0]) == LETTER)
            return token < t.token;
        assert(false);
    }

    void print() const{
        cout << token;
    }

    string get_token(){
        return token;
    }

};

void print(const vector<Token> &a){
    for (int i = 0; i < (int)a.size(); i++)
        a[i].print();
    cout << endl;
}

vector<Token> tokenize(const string &s){
    string buf = "";
    vector<Token> r;
    int cur_type = UNKNOWN;
    for (int j = 0; j < (int)s.length(); j++){
        if (get_type(s[j]) == cur_type)
            buf += s[j];
        else {
            if (cur_type != UNKNOWN)
                r.push_back(Token(buf));
            buf = "";
            buf += s[j];
            cur_type = get_type(s[j]);
        }
    }
    r.push_back(Token(buf));
    return r;
}

void test_tokenizer(){
    cerr << "Testing tokenize\n";
    vector<Token> res;
    string s;

    s = "12dfre56gf";
    res = tokenize(s);
    if (!(res.size() == 4 && 
            res[0].get_token() == "12" && res[1].get_token() == "dfre" 
            && res[2].get_token() == "56" && res[3].get_token() == "gf")){
        cerr << "Bad tokenize #1" << endl;
        exit(0);
    }

    s = "fd45ggg22g";
    res = tokenize(s);
    if (!(res.size() == 5 && 
            res[0].get_token() == "fd" && res[1].get_token() == "45" 
            && res[2].get_token() == "ggg" && res[3].get_token() == "22" 
            && res[4].get_token() == "g")){
        cerr << "Bad tokenize #2" << endl;
        exit(0);
    }

    s = "567";
    res = tokenize(s);
    if (!(res.size() == 1 && res[0].get_token() == "567")){
        cerr << "Bad tokenize #4" << endl;
        exit(0);
    }

    s = "dsfa";
    res = tokenize(s);
    if (!(res.size() == 1 && res[0].get_token() == "dsfa")){
        cerr << "Bad tokenize #5" << endl;
        exit(0);
    }

    cerr << "Testing tokenize: OK\n";    
}

void test_cmp_as_number(){
    cerr << "Testing cmp_as_number\n";
    
    if (cmp_as_number("3", "3")){
        cerr << "Bad cmp_as_number #1\n";
        exit(0);
    }

    if (cmp_as_number("4", "3")){
        cerr << "Bad cmp_as_number #2\n";
        exit(0);
    }

    if (!cmp_as_number("3", "4")){
        cerr << "Bad cmp_as_number #3\n";
        exit(0);
    }

    if (!cmp_as_number("3", "33")){
        cerr << "Bad cmp_as_number #4\n";
        exit(0);
    }

    if (cmp_as_number("33", "3")){
        cerr << "Bad cmp_as_number #5\n";
        exit(0);
    }

    if (cmp_as_number("3", "03")){
        cerr << "Bad cmp_as_number #6\n";
        exit(0);
    }
    
    if (!cmp_as_number("03", "3")){
        cerr << "Bad cmp_as_number #7\n";
        exit(0);
    }

    cerr << "Testing cmp_as_number: OK\n";
}

void test_get_type(){
    cerr << "Testing get_type\n";

    if (get_type('3') != DIGIT){
        cerr << "Bad get_type #1\n";
        exit(0);
    }

    if (get_type('a') != LETTER){
        cerr << "Bad get_type #2\n";
        exit(0);
    }

    cerr << "Testing get_type: OK\n";
}

void test_cmp_token(){
    cerr << "Testing cmp_token\n";
    
    if (Token("ab") < Token("aa")){
        cerr << "Bad cmp_token #1\n";
        exit(0);
    }

    if (Token("3") < Token("a")){
        cerr << "Bad cmp_token #2\n";
        exit(0);
    }

    if (Token("334") < Token("33")){
        cerr << "Bad cmp_token #3\n";
        exit(0);
    }

    if (Token("abcd") < Token("abc")){
        cerr << "Bad cmp_token 4\n";
        exit(0);
    }

    if (Token("001") < Token("0001")){
        cerr << "Bad cmp_token #5\n";
        exit(0);
    }

    cerr << "Testing cmp_token: OK\n";
}

void unit_test(){
    cerr << "Start testing\n";
    test_get_type();
    test_tokenizer();
    test_cmp_as_number();
    test_cmp_token();
    cerr << "Testing: OK\n";
}

vector<vector<Token> > a;

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
