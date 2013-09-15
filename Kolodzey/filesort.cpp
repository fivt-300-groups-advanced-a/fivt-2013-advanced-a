//НА ДАННЫЙ МОМЕНТ ПРОГРАММА КОРРЕКТНО СОРТИРУЕТ НЕ БОЛЕЕ 10000 СТРОК ДЛИНОЙ НЕ БОЛЕЕ 100 С БЛОКАМИ ДЛИНЫ НЕ БОЛЕЕ 20
//ВОЗМОЖНА АСИМТОТИКА NL log N из-за копирования строк во время вызовов ф-ий
//Асимптотика O(N log N log L) N = количество, L = средняя длина строк

#include <cstdio>
#include <iostream>
#include <cctype>
#include <algorithm>

//магические константы
const int MAX_L = 105; //когда-нибудь я заменю это на динамическое выделение памяти
const int MAX_N = 10005; //и это тоже

//магические хэши (интересно, можно ли их сделать неглобальными?)
const unsigned long long pPrime = 1000000007;
unsigned long long xPrime[MAX_L] = {1, 613953};
unsigned long long xOverflow[MAX_L] = {1, 1746411056956931};

struct MyStr
{
	char str[MAX_L]; //собственно строка, записана с 1-ого символа!
	unsigned long long overflowHash[MAX_L]; //тут в 0 символе записан 0
	unsigned long long primeHash[MAX_L]; //тут тоже
	int prefNumSum[MAX_L]; //сумма на префиксах в массиве, где a[i] = 0, если str[i] = число, a[i] = 1, если str[i] = символ; a[0]=0
	int prefSymSum[MAX_L]; //сумма на префиксах в массиве, где a[i] = 1, если str[i] = число, a[i] = 0, если str[i] = символ; a[0]=0
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
	int t; //тип 1 = число, -1 = строка
	int pstart; //начало в своей строке (первый символ)
	int len; //длина блока в символах
};

//--переменные-- переписать как-нибудь с локальными переменными
MyStr data[MAX_N];
int order[MAX_N]; //строки туда сюда по массиву переписывать - это нехорошо, будем сортировать номера строк в массиве data
int N; //количество строк, которые мы сравниваеи

using namespace std; //да простят меня боги

//возвращает номер первого символа, в котором строки различаются; если строки равны, или одна из них является подстрокой другой, то возвращает -1
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

int findLargestCommonPrefixofBlocks(MyBlock a, MyBlock b) //возвращает длину общего префикса блоков
{
	//for (int i = 0; i < a.len; ++i)
	//	cout << data[a.pstr].str[a.pstart + i];
	//cout << endl;
	//for (int i = 0; i < b.len; ++i)
	//	cout << data[b.pstr].str[b.pstart + i];
	//cout << endl;

	int l = 0;
	int r = min(a.len, b.len) + 1;
	while (r - l > 1)
	{
		int m = (l + r) / 2;
		if ((data[a.pstr].overflowHash[a.pstart + m - 1] - data[a.pstr].overflowHash[a.pstart - 1] * xOverflow[m] == 
			 data[b.pstr].overflowHash[b.pstart + m - 1] - data[b.pstr].overflowHash[b.pstart - 1] * xOverflow[m]) &&
			((pPrime + data[a.pstr].primeHash[a.pstart + m - 1] - (data[a.pstr].primeHash[a.pstart - 1] * xPrime[m]) % pPrime) % pPrime == 
			 (pPrime + data[b.pstr].primeHash[b.pstart + m - 1] - (data[b.pstr].primeHash[b.pstart - 1] * xPrime[m]) % pPrime) % pPrime))
			l = m;
		else
			r = m;
	//	cout << "= "<< l << " " << r << endl;
	}
	//cout << "====" << endl;
	return l;
}

//возвращает блок, в котором есть str[p]
MyBlock findBordersOfBlock(int ps, int pch) //pch - символ, который точно есть в блоке, ps - позиция строки в data
{
	MyBlock ans;
	ans.pstr = ps;
	if (data[ps].prefNumSum[pch] == data[ps].prefNumSum[pch - 1])
	{
		ans.t = 1; //мы узнали тип блока, так как если бы это был символьный блок, то prefNumSum[pch - 1] было бы на единицу меньше, чем prefNumSum[pch]
		
		//нахождение позиции начала блока, инвариант (l; r], где prefNumSum[r] = prefNumSum[pch], при этом на самом-то деле, первый символ с prefNumSum[pch] в блок не входит
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
        
        //ищем позицию конца блока, l - входит в блок prefNumSum[r] > prefNumSum[l], тк r не входит в блок
        l = pch;
        r = data[ps].len + 1;
        while (r - l > 1)
        {
        	int m = (l + r) / 2;
        	if (data[ps].prefNumSum[m] <= data[ps].prefNumSum[pch])
        		l = m;
        	else
        		r = m;
        }
        ans.len = l - ans.pstart + 1;
	}
	else
	{
		ans.t = -1;

		//убрать копипаст, написать фунцией?

		//нахождение позиции начала блока, инвариант (l; r], где prefSymSum[r] = prefSymSum[pch], при этом на самом-то деле, первый символ с prefSymSum[pch] в блок не входит
		int l = -1;
		int r = pch;
		while (r - l > 1)
		{
			int m = (l + r + 1) / 2;
			if (data[ps].prefSymSum[m] < data[ps].prefSymSum[pch])
				l = m;
			else
				r = m;
		}
		ans.pstart = r + 1;
        
        //ищем позицию конца блока, l - входит в блок prefSymSum[r] > prefSymSum[l], тк r не входит в блок
        l = pch;
        r = data[ps].len + 1;
        while (r - l > 1)
        {
        	int m = (l + r) / 2;
        	if (data[ps].prefSymSum[m] <= data[ps].prefSymSum[pch])
        		l = m;
        	else
        		r = m;
        }
        ans.len = l - ans.pstart + 1;
	}
	return ans;
}
//возвращает 1, если строка, оканчивающаяся на блок а должна быть раньше, при условии, что префиксы до блоков a и b одинаковы.
bool cmpBlock(MyBlock a, MyBlock b)
{
	if (a.t < b.t)
	{
//		cout << "(a.t < b.t)" << endl;
		return 1;
	}
	if (b.t < a.t)
	{
//		cout << "(b.t < a.t)" << endl;
		return 0;
	}
	if (a.t == -1)
	{
//		cout << "(t == -1)" << endl;
		int lenOfCommonPrefix = findLargestCommonPrefixofBlocks(a, b);
		if ((a.len < b.len) && (a.len == lenOfCommonPrefix))
		{
//			cout << "((a.len < b.len) && (a.len == lenOfCommonPrefix))" << endl;
			return 1;
		}	
		if ((b.len <= a.len) && (b.len == lenOfCommonPrefix))
		{
//			cout << "((b.len <= a.len) && (b.len == lenOfCommonPrefix))";
			return 0;
		}
		return data[a.pstr].str[a.pstart + lenOfCommonPrefix] < data[b.pstr].str[b.pstart + lenOfCommonPrefix];
	}

//	cout << "(t == 1)" << endl;
	
	MyBlock zero;
	zero.len = 20; //zero - блок строки состоящей из нулей, по-хорошему его длина совпадает с максимально возможной длиной строки, но пока так
	zero.pstr = N; //строка нулей (charов) записывается в конец массива при вводе
	zero.pstart = 1;
	zero.t = 1;

	int a0len = findLargestCommonPrefixofBlocks(a, zero);
	int b0len = findLargestCommonPrefixofBlocks(b, zero);
	
	MyBlock awithout0 = a;
	awithout0.len -= a0len;
	awithout0.pstart += a0len;

	MyBlock bwithout0 = b;
	bwithout0.len -= b0len;
	bwithout0.pstart += b0len;

//	cout << "a.len " << a.len << endl;
//	cout << "a0len " << a0len << endl;

//	cout << "b.len " << b.len << endl;
//	cout << "b0len " << b0len << endl;

	if (awithout0.len < bwithout0.len)
	{
//		cout << "(awithout0.len < bwithout0.len)" << endl;
		return 1;
	}
	if (bwithout0.len < awithout0.len)
	{
//		cout << "(awithout0.len > bwithout0.len)" << endl;
		return 0;
	}

	int lenOfCommonPrefix = findLargestCommonPrefixofBlocks(awithout0, bwithout0);
	if (lenOfCommonPrefix < awithout0.len)
	{
//		cout << "сравнение чисел одинаковой длины" << endl;
		return data[awithout0.pstr].str[awithout0.pstart + lenOfCommonPrefix] < data[bwithout0.pstr].str[bwithout0.pstart + lenOfCommonPrefix];
	}

//	cout << "сравнение количества ведущих нулей" << endl;
	return a0len > b0len;
}

//возвращает 1, если строка, лежащая в data[ps] должна быть раньше, чем строка лежащая в data[pt]
bool cmpPStr(int ps, int pt)
{
	int firstDiff = findFirstDifferenceInStr(data[ps], data[pt]); //да-да-да, неэффективный вызов с копированием
	if (firstDiff == -1) //одна строка — префикс другой
	{
		int lessLen;
		if (data[ps].len < data[pt].len)
		{
			if (cmpBlock(findBordersOfBlock(pt, data[ps].len), findBordersOfBlock(ps, data[ps].len)))
				return 0;
			return 1;
		}
		else
		{
			if (cmpBlock(findBordersOfBlock(ps, data[pt].len), findBordersOfBlock(pt, data[pt].len)))
				return 1;
			return 0;
		}
	}
    return cmpBlock(findBordersOfBlock(ps, firstDiff), findBordersOfBlock(pt, firstDiff));
}

//предподсчёт множителей для хэширования
void initHash()
{
	for (int i = 2; i < MAX_L;  ++i)
	{
		xPrime[i] = (xPrime[i - 1] * xPrime[1]) % pPrime;
		xOverflow[i] = xOverflow[i - 1] * xOverflow[1];
	}
}
//ввод и инициализация
//data после ввода менять низзя
void readData()
{
	cin >> N; //исправить на считывание до конца файла


	for (int i = 0; i < N; ++i)
	{
		order[i] = i;
		char ch[MAX_L];
		cin >> ch;
		if (strlen(ch) > 100) //убрат, когда напишешь динамическое выделение памяти
			cout << "ВЫВОД МОЖЕТ БЫТЬ НЕКОРРЕКТЕН, ПОМЕНЯЙТЕ ВОЛШЕБНУЮ КОНСТАТНУ" << endl;
        
		data[i] = MyStr(ch);
	}

	char ch[MAX_L] = "00000000000000000000000000";
	data[N] = MyStr(ch); //тут будет строка из нулей длины максимальной введённой строки
}

void dbgoutReadData()
{
	for (int i = 0; i < N; ++i)
	{
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
	}
}
void dbgoutFindFirstDifferenceInStr()
{
	 char ch[MAX_L];
     cin >> ch;
     MyStr s = MyStr(ch);
     cin >> ch;
     MyStr t = MyStr(ch);
     cout << findFirstDifferenceInStr(t, s) << endl;
}
void dbgoutFindBordersOfBlock()
{
	readData();
	int m;
	cin >> m;

	for (int i = 0; i < m; ++i)
	{
		int ps, pch;
		cin >> ps >> pch;
		MyBlock ans = findBordersOfBlock(ps, pch);
		if (ans.t == -1)
			cout << "str ";
		else
			cout << "num ";
		cout << ans.pstart << " " << ans.len << endl;
	}
	/*
	5
	absc
	132abcd2
	acaklj1214
	123123ffsfd
	12414324
	8
	0 1
	0 4
	0 3
	1 1
	1 3
	1 4
	1 8
	4 5
	*/
}
void dbgoutFindLargestCommonPrefixofBlocks()
{
	readData();
	int m;
	cin >> m;
	for (int i = 0; i < m; ++i)
	{
		int ps, pch;
		cin >> ps >> pch;
		MyBlock a = findBordersOfBlock(ps, pch);

		cin >> ps >> pch;
		MyBlock b = findBordersOfBlock(ps, pch);

		cout << findLargestCommonPrefixofBlocks(a, b) << endl;
	}

	/*
	5
	absc
	132abcd2
	acaklj1214
	123123ffsfd
	12414324

	3
	0 1
	1 4

	1 1
	2 8

	4 1
	2 1
    */
}
void dbgoutCmpBlocks()
{
	readData();
	int m;
	cin >> m;
	for (int i = 0; i < m; ++i)
	{
		int ps, pch;
		cin >> ps >> pch;
		MyBlock a = findBordersOfBlock(ps, pch);

		cin >> ps >> pch;
		MyBlock b = findBordersOfBlock(ps, pch);

		cout << "=====" << endl;
		cout << cmpBlock(a, b) << endl;
	}
	/*
	5
	absc
	132abcd2
	acaklj1214
	123123ffsfd
	12414324

	3
	0 1
	1 4

	1 1
	2 8

	4 1
	2 1
    */
    /*
    2
	023
	23

	1
	0 1
	1 1
    */
}
int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	
	initHash();
	readData();

	sort(order, order + N, cmpPStr);
	
	for (int i = 0; i < N; ++i)
	{
		for (int j = 1; j <= data[order[i]].len; ++j)
			cout << data[order[i]].str[j];
		cout << endl;
	}
	
	/*
	13
	absc
	132abcd2
	acaklj1214
	123123ffsfd
	1241432
	a.00.a
	a.0
	a.23
	a.b
	a.023
	a.13
	bdfgd
	a.00
	*/
	return 0;
}
