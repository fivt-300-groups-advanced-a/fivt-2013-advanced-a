//Так, интересно, получится ли закоммитить изменения кода прямо отсюда?
//Асимптотика O(N log N log L) N = количество, L = средняя длина строк

#include <cstdio>
#include <iostream>
#include <cctype>
#include <algorithm>

const int MAX_L = 105; //когда-нибудь я заменю это на динамическое выделение памяти
const int MAX_N = 10005; //и это тоже

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

MyStr data[MAX_N];
int order[MAX_N]; //строки туда сюда по массиву переписывать - это нехорошо, будем сортировать номера строк в массиве data

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

//возвращает блок, в котором есть str[p]
MyBlock findBordersOfBlock(int ps, int pch) //pch - символ, который точно есть в блоке, ps - позиция строки в data
{
	MyBlock ans;
	ans.pstr = ps;
	if (data[ps].prefNumSum[pch] == data[ps].prefNumSum[pch - 1])
	{
		ans.t = 1; //мы узнали тип блока, так как если бы это был символьный блок, то prefNumSum[pch - 1] было бы на единицу меньше, чем prefNumSum[pch]
		
		//нахождение позиции начала блока
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
//возвращает 1, если строка, оканчивающаяся на блок а должна быть раньше, при условии, что префиксы до блоков a и b одинаковы.
bool cmpBlock(MyBlock a, MyBlock b);

//возвращает 1, если строка, лежащая в data[ps] должна быть раньше, чем строка лежащая в data[pt]
bool cmpPStr(int ps, int pt);

int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	
	//предподсчёт множителей для хэширования
	for (int i = 2; i < MAX_L;  ++i)
	{
		xPrime[i] = (xPrime[i - 1] * xPrime[1]) % pPrime;
		xOverflow[i] = xOverflow[i - 1] * xOverflow[1];
	}
	
	int n; //исправить на считывание до конца файла
	cin >> n;
    
	//ввод и инициализация
	//data после ввода менять низзя
	for (int i = 0; i < n; ++i)
	{
		order[i] = i;
		char ch[MAX_L];
		cin >> ch;
		if (strlen(ch) > 100) //убрат, когда напишешь динамическое выделение памяти
			cout << "ВЫВОД МОЖЕТ БЫТЬ НЕКОРРЕКТЕН, ПОМЕНЯЙТЕ ВОЛШЕБНУЮ КОНСТАТНУ" << endl;
        
		data[i] = MyStr(ch);
		//тестим ввод
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
		//тестим ввод
	}
    
	//sort(order, order + n, cmpPStr);
	
	for (int i = 0; i < n; ++i)
	{
		for (int j = 1; j <= data[order[i]].len; ++j)
			cout << data[order[i]].str[j];
		cout << endl;
	}
	
	
	//тестим поиск различия в строке
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
