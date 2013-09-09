//����������� O(N log N log L) N = ����������, L = ������� ����� �����

#include <cstdio>
#include <iostream>
#include <cctype>
#include <algorithm>

const int MAX_L = 105; //�����-������ � ������ ��� �� ������������ ��������� ������
const int MAX_N = 10005; //� ��� ����

const unsigned long long pPrime = 1000000007;

unsigned long long xPrime[MAX_L] = {1, 613953};
unsigned long long xOverflow[MAX_L] = {1, 1746411056956931};

struct MyStr
{
	char str[MAX_L]; //���������� ������, �������� � 1-��� �������!
	unsigned long long overflowHash[MAX_L]; //��� � 0 ������� ������� 0
	unsigned long long primeHash[MAX_L]; //��� ����
	int prefNumSum[MAX_L]; //����� �� ��������� � �������, ��� a[i] = 0, ���� str[i] = �����, a[i] = 1, ���� str[i] = ������; a[0]=0
	int prefSymSum[MAX_L]; //����� �� ��������� � �������, ��� a[i] = 1, ���� str[i] = �����, a[i] = 0, ���� str[i] = ������; a[0]=0
	int len;
	
	MyStr()
	{
		len = 0;
		overflowHash[0] = 0;
		primeHash[0] = 0;
		prefNumSum[0] = 0;
		prefSymSum[0] = 0;
		str[0] = 0;
	}

	MyStr(char s[])
	{
		len = strlen(s);
		overflowHash[0] = 0;
		primeHash[0] = 0;
		prefNumSum[0] = 0;
		prefSymSum[0] = 0;
		str[0] = 0;
		for (int i = 1; i <= len; ++i)
		{
			str[i] = s[i - 1];
			if (isdigit(str[i]))
			{
				prefNumSum[i] = prefNumSum[i - 1];
				prefSymSum[i] = prefSymSum[i - 1] + 1;
			}
			else
			{
				prefNumSum[i] = prefNumSum[i - 1] + 1;
				prefSymSum[i] = prefSymSum[i - 1];
			}
			primeHash[i] = (primeHash[i - 1] * xPrime[1] + (unsigned long long) (str[i])) % pPrime;
			overflowHash[i] = overflowHash[i - 1] * xOverflow[1] + (unsigned long long) (str[i]);
		}
	}
};


struct MyBlock
{
	int pstr;
	int t; //��� 1 = �����, -1 = ������
	int pstart; //������ � ����� ������ (������ ������)
	int len; //����� ����� � ��������
};

MyStr data[MAX_N];
int order[MAX_N]; //������ ���� ���� �� ������� ������������ - ��� ��������, ����� ����������� ������ ����� � ������� data

using namespace std; //�� ������� ���� ����

//���������� ����� ������� �������, � ������� ������ �����������; ���� ������ �����, ��� ���� �� ��� �������� ���������� ������, �� ���������� -1
int findFirstDifferenceInStr(MyStr s, MyStr t)
{
	int l = 0;
	int r = min(s.len, t.len);
	if ((s.overflowHash[r] == t.overflowHash[r]) && (s.primeHash[r] == t.primeHash[r]))
		return -1;
	while ((r - l) > 1)
	{
		int m = (l + r) / 2;
		if ((s.overflowHash[m] == t.overflowHash[m]) && (s.primeHash[m] == t.primeHash[m]))
			l = m;
		else
			r = m;
	}
	return r;
}

//���������� ����, � ������� ���� str[p]
MyBlock findBordersOfBlock(int ps, int pch) //pch - ������, ������� ����� ���� � �����, ps - ������� ������ � data
{
	MyBlock ans;
	ans.pstr = ps;
	if (data[ps].prefNumSum[pch] == data[ps].prefNumSum[pch - 1])
	{
		ans.t = 1; //�� ������ ��� �����, ��� ��� ���� �� ��� ��� ���������� ����, �� prefNumSum[pch - 1] ���� �� �� ������� ������, ��� prefNumSum[pch]
		
		//���������� ������� ������ �����
		int l = -1;
		int r = pch;
		while (r - l > 1)
		{
			int m = (l + r + 1) / 2;
			if (data[ps].prefNumSum[m] < data[ps].prefNumSum[pch])
				l = m;
			else
				r = m;
		}
		ans.pstart = r + 1;


	}
	else
	{
		ans.t = -1;
	}
}
//���������� 1, ���� ������, �������������� �� ���� � ������ ���� ������, ��� �������, ��� �������� �� ������ a � b ���������.
bool cmpBlock(MyBlock a, MyBlock b);

//���������� 1, ���� ������, ������� � data[ps] ������ ���� ������, ��� ������ ������� � data[pt]
bool cmpPStr(int ps, int pt);

int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	
	//����������� ���������� ��� �����������
	for (int i = 2; i < MAX_L;  ++i)
	{
		xPrime[i] = (xPrime[i - 1] * xPrime[1]) % pPrime;
		xOverflow[i] = xOverflow[i - 1] * xOverflow[1];
	}
	
	int n; //��������� �� ���������� �� ����� �����
	cin >> n;

	//���� � �������������
	//data ����� ����� ������ �����
	for (int i = 0; i < n; ++i)
	{
		order[i] = i;
		char ch[MAX_L];
		cin >> ch;
		if (strlen(ch) > 100) //�����, ����� �������� ������������ ��������� ������
			cout << "����� ����� ���� �����������, ��������� ��������� ���������" << endl;

		data[i] = MyStr(ch);
		//������ ����
		/*
		cout << "======" << endl;

		cout << "len " << data[i].len << endl;

		cout << "str ";
		for (int j = 0; j <= data[i].len; ++j)
			cout << data[i].str[j] << " ";
		cout << endl;

		cout << "sym ";
		for (int j = 0; j <= data[i].len; ++j)
			cout << data[i].prefSymSum[j] << " ";
		cout << endl;

		cout << "num ";
		for (int j = 0; j <= data[i].len; ++j)
			cout << data[i].prefNumSum[j] << " ";
		cout << endl;

		cout << "prh ";
		for (int j = 0; j <= data[i].len; ++j)
			cout << data[i].primeHash[j] << " ";
		cout << endl;

		cout << "ovh ";
		for (int j = 0; j <= data[i].len; ++j)
			cout << data[i].overflowHash[j] << " ";
		cout << endl;

		cout << "======" << endl;
		*/
		//������ ����
	}

	//sort(order, order + n, cmpPStr);
	
	for (int i = 0; i < n; ++i)
	{
		for (int j = 1; j <= data[order[i]].len; ++j)
			cout << data[order[i]].str[j];
		cout << endl;
	}
	
	
	//������ ����� �������� � ������
	/*
	char ch[MAX_L];
	cin >> ch;
	MyStr s = MyStr(ch);
	cin >> ch;
	MyStr t = MyStr(ch);
	cout << findFirstDifferenceInStr(t, s) << endl;
	*/
	return 0;
}