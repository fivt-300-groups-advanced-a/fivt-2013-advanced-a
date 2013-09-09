#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

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
	token (const string &buffer){
		word = buffer;
	}
	bool operator < (const token &comp) const{
		if (isdigit(word[0]) != isdigit(comp.word[0]))
			return (isdigit(word[0]) ? 1 : 0);
		if (isdigit(word[0]) && isdigit(comp.word[0]))
			return comp_num(word, comp.word);
		else
			return word < comp.word;
	}
};

vector < vector <token> > words;
string word;

void tokenize(vector <token> &tokens){
	string buffer = "";
	int type = 2;
	for (int j = 0; j < (int)word.length(); j++){
		if (isdigit(word[j])){
			if (type != 1 && buffer != ""){
				tokens.push_back(token(buffer));
				buffer = "";
			}
			buffer += word[j];
			type = 1;
		}
		if (!isdigit(word[j])){
			if (type != 0 && buffer != ""){
				tokens.push_back(token(buffer));
				buffer = "";
			}
			buffer += word[j];
			type = 0;
		}
	}
	tokens.push_back(token(buffer));
}

void out(const vector <token> &out_word){
	for (int i = 0; i < (int)out_word.size(); i++)
		cout << out_word[i].word;
	cout << endl;
}

bool test_tokenize(){
	cout << "Test test_tokenize:" << endl;
	bool cnt = 1;
	word = "a";
	vector <token> test;
	tokenize(test);
	if (test.size() != 1 || test[0].word != "a"){
		cout << word << endl;
		cnt = 0;
	}
	test.clear();
	word = "1";
	tokenize(test);
	if (test.size() != 1 || test[0].word != "1"){
		cout << word << endl;
		cnt = 0;
	}
	test.clear();
	word = "as1fv0";
	tokenize(test);
	if (test.size() != 4 || test[0].word != "as" || test[1].word != "1" || test[2].word != "fv" || test[3].word != "0"){
		for (int i = 0; i < (int)test.size(); i++)
			cout << test[i].word << " ";
		cout << endl;
		cout << word << endl;
		cnt = 0;
	}
	test.clear();
	word = "1fv0a1";
	tokenize(test);
	if (test.size() != 5 || test[0].word != "1" || test[1].word != "fv" || test[2].word != "0" || test[3].word != "a" || test[4].word != "1"){
		cout << word << endl;
		cnt = 0;
	}
	return cnt;
}

bool test_token(){
	cout << "Test test_token:" << endl;
	bool cnt = 1;
	if (token("sdf") < token("asd")){
		cout << "  sdf asd" << endl;
		cnt = 0;
	}
	if (token("123") < token("34")){
		cout << "  123 34" << endl;
		cnt = 0;
	}
	if (token("1") < token("001")){
		cout << "  1 001" << endl;
		cnt = 0;
	}
	if (token("aaa") < token("aa")){
		cout << "  aaa aa" << endl;
		cnt = 0;
	}
	return cnt;
}

bool test_comp_num(){
	cout << "Test comp_num:" << endl;
	bool cnt = 1;
	if (comp_num("213", "123")){
		cout << "  213 123" << endl;
		cnt = 0;
	}
	if (comp_num("1", "001")){
		cout << "  1 001" << endl;
		cnt = 0;
	}
	return cnt;
}

void unit_tests(){
	if (test_tokenize())
		cout << "Everything is ok!" << endl;
	if (test_token())
		cout << "Everything is ok!" << endl;
	if (test_comp_num())
		cout << "Everything is ok!" << endl;
}

int main(){
	unit_tests();
	int n;
	cin >> n;
	words.resize(n);
	for (int i = 0; i < n; i++){
		cin >> word;
		tokenize(words[i]);
	}
	sort(words.begin(), words.end());
	for (int i = 0; i < n; i++){
		out(words[i]);
	}
}
