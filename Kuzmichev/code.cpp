#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <ctime>

#define LL long long

using namespace std;


const int NUMBER_TYPE = 1;
const int STRING_TYPE = 2;
const int UNDEF = -1;

struct TPart
{
	static const int LESS = -1;
	static const int EQUAL = 0;
	static const int MORE = 1;
	int type;
	LL number;
	string str;

	TPart(){}
	TPart(int _type, string _str, LL _number)
	{
		type = _type;
		if (_type == STRING_TYPE)
		{
			str = _str;
			number = 0;
		}
		else
		{
			str = "";
			number = _number;
		}
	}

	int compare(TPart an) const
	{
		if (type == an.type)
		{
			if (type == NUMBER_TYPE)
			{
				if (number < an.number)
					return LESS;
				if (number == an.number)
					return EQUAL;
				return MORE;
			}
			else
			{
				if (str < an.str)
					return LESS;
				if (str == an.str)
					return EQUAL;
				return MORE;
			}
		}
		else
		{
			if (type == STRING_TYPE)
				return LESS;
			return MORE;
		}
	}
};

/*bool isNumber(char c)
{
	return c >= '0' && c <= '9';
}*/

struct TFileName
{
	string startString;
	vector <TPart> parts;
	void print()
	{
		for (int j = 0; j < parts.size(); j++)
			if (parts[j].type == NUMBER_TYPE)
				cout << parts[j].number;
			else cout << parts[j].str;
		cout << endl;
	}

	void add(int curType, string curString, int curNumber)
	{
		if (curType != UNDEF)
		{
			TPart newElem = TPart(curType, curString, curNumber);
			parts.push_back(newElem);
		}
	}

	void parse()
	{
		parts.clear();
		int curType = UNDEF;
		string curString = "";
		LL curNumber = 0;
		for (int j = 0; j < startString.length(); j++)
		{
			if (isdigit(startString[j]))
			{
				if (curType == NUMBER_TYPE)
					curNumber = curNumber * 10 + (startString[j] - '0');
				else
				{
					add(curType, curString, curNumber);
					curType = NUMBER_TYPE;
					curNumber = (startString[j] - '0');
				}
			}
			else
			{
				if (curType == STRING_TYPE)
					curString += startString[j];
				else
				{
					add(curType, curString, curNumber);
					curType = STRING_TYPE;
					curString = startString[j];
				}
			}
		}
		add(curType, curString, curNumber);
		//print(parts);
	}
};

bool operator < (const TFileName & first, const TFileName & second)
{
	for (int j = 0; j < min(first.parts.size(), second.parts.size()); j++)
	{
		int compareResult = first.parts[j].compare(second.parts[j]);
		if (compareResult == TPart.LESS)
			return true;
		if (compareResult == TPart.MORE)
			return false;
	}
	return first.parts.size() < second.parts.size();
}

const int maxn = 100;

int n;
TFileName e[maxn];

int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	scanf("%d", &n);
	for (int q = 0; q < n; q++)
	{
		cin >> e[q].startString;
		e[q].parse();
	}
	sort(e, e + n);
	for (int q = 0; q < n; q++)
		e[q].print();
}