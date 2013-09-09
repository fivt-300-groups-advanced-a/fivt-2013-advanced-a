#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

const int DIGIT = 0;
const int LETTER = 1;
const int UNKNOWN = 2;

inline int get_type(char c){
    return (isdigit(c)? DIGIT : LETTER);
}

void print(vector<string> &a){
    for (int i = 0; i < (int)a.size(); i++)
        cout << a[i];
    cout << endl;
}

bool cmp(const vector<string> &a, const vector<string> &b){
    for (int i = 0; i < (int)min(a.size(), b.size()); i++)
        if (get_type(a[i][0]) != get_type(b[i][0]))
            return (get_type(a[i][0]) == LETTER? true : false);
        else if (get_type(a[i][0]) == DIGIT){
            int val_a = atoi(a[i].c_str());
            int val_b = atoi(b[i].c_str());
            if (val_a < val_b)
                return true;
            if (val_a > val_b)
                return false;
            if (a[i].length() > b[i].length())
                return true;
            if (a[i].length() < b[i].length())
                return false;
        } else {
            if (a[i] < b[i])
                return true;
            if (a[i] > b[i])
                return false;
        }
    if (a.size() < b.size())
        return true;
    else
        return false;
}

vector<string> a[10010];
int n;

int main(){
    cin >> n;
    string s;
    for (int i = 0; i < n; i++){
        cin >> s;
        string buf = "";
        int cur_type = UNKNOWN;
        for (int j = 0; j < (int)s.length(); j++){
            if (get_type(s[j]) == cur_type)
                buf += s[j];
            else {
                if (cur_type != UNKNOWN)
                    a[i].push_back(buf);
                buf = "";
                buf += s[j];
                cur_type = get_type(s[j]);
            }
        }
        a[i].push_back(buf);
    }
    
    sort(a, a + n, cmp);
    cout << endl;
    for (int i = 0; i < n; i++)
        print(a[i]);
}
