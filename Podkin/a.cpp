#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

#define EPS (1e-9)
#define INF (1000000000)

#define pb push_back
#define sz(a) (int)(a).size()

using namespace std;

bool is_num(const string &word){
	char ch = word[0];
	if (ch >= '0' && ch <= '9')
		return 1;
	else
		return 0;
}

bool comp_num(const string &x, const string &y){
	int a = atoi(x.c_str()), b = atoi(y.c_str());
	if (a > b || (a == b && x.length() < y.length()))
		return 0;
	if (b > a || (a == b && y.length() < x.length()))
		return 1;
	return 0;
}

struct token{
	string word;
	bool operator < (const token &comp) const{
		if (is_num(word) != is_num(comp.word))
			return (is_num(word) ? 1 : 0);
		if (is_num(word) && is_num(comp.word))
			return comp_num(word, comp.word);
		else
			return word < comp.word;
	}
};

vector < vector <token> > words;
int n, type = INF;
string word, buffer;

void save(int x){
	token help;
	help.word = buffer;
	words[x].pb(help);
	buffer = "";
}

void tokenize(int x){
	buffer = "";
	type = INF;
	for (int j = 0; j < (int)word.length(); j++){
		if (is_num(word)){
			if (!type && buffer != "")
				save(x);
			buffer += word[j];
			type = 1;
		}
		if (!is_num(word)){
			if (type && buffer != "")
				save(x);
			buffer += word[j];
			type = 0;
		}
	}
	save(x);
}

void out(const vector <token> &out_word){
	for (int i = 0; i < sz(out_word); i++)
		cout << out_word[i].word;
	cout << endl;
}

int main(){
	cin >> n;
	words.resize(n);
	for (int i = 0; i < n; i++){
		cin >> word;
		tokenize(i);
	}
	sort(words.begin(), words.end());
	for (int i = 0; i < n; i++){
		out(words[i]);
	}
}
