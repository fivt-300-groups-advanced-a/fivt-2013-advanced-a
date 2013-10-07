#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <ctime>

#define i64 long long

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
	i64 number;
	string str;

	TPart(){}
	TPart(int _type, string _str, i64 _number)
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
		i64 curNumber = 0;
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
TFileName e[maxn];

void runTest(int testNumber, int n)
{
	printf("Run test #%d (n = %d)\n", testNumber, n);
	printf("Input:\n");
	for (int q = 0; q < n; q++)
		cout << q << " string: " << e[q].startString << endl;
	printf("\n");
	for (int q = 0; q < n; q++)
	{
		//cin >> e[q].startString;
		e[q].parse();
	}
	sort(e, e + n);
	printf("Checking asserts\n");
	for (int q = 0; q < n - 1; q++)	
		assert(!(e[q + 1] < e[q]));
	printf("No assertion failed\n");
	printf("Sorted array:\n");
	for (int q = 0; q < n; q++)
	{
		printf("%d string: ", q);
		e[q].print();
	}
	printf("\n");
}

inline void init(int strNumber, const string value)
{
	e[strNumber].startString = value;
}

void UnitTests()
{
	init(0, "d3.txt4");
	init(1, "d3.txt");
	init(2, "a1.txt");
	init(3, "b5.jpg");
	init(4, "345.qwerty.546456");
	init(5, "a10.txt");
	init(6, "a2.txt");
	runTest(1, 7);

	for(int q = 0; q < 100; q++)
	{
		string tmp = "";
		int len = rand() % 10 + 1;
		for(int j = 0; j < len; j++)
		{
			char c;
			if (rand() & 1)
				c = '0' + rand() % 10;
			else c = ('a' + rand() % 26);
			tmp += c;
		}
		init(q, tmp);
	}
	runTest(2, 100);

	init(0, "123abc");
	init(1, "124abc");
	runTest(3, 2);
	
	init(0, "123abc123");
	init(1, "123abc122");
	runTest(4, 2);

	init(0, "456ghjt");
	init(1, "456ghj");
	runTest(5, 2);

	init(0, "456ert");
	init(1, "456era");
	runTest(6, 2);


}

int main()
{
	//freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	UnitTests();		
}