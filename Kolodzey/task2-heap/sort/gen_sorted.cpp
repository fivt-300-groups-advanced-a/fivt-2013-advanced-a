#include <cstdio>
#include <fstream>
#include <algorithm>

const int MAX_N = 100500;
int main()
{
	int a[MAX_N];
	std::ifstream in;
	std::ofstream out;
	for (int i = 1; i <= 23; ++i)
	{
		char ch[7] = {0, 0, 0, 0, 0, 0, 0};
		sprintf(ch, "%02d.in", i);
		in.open(ch);
		sprintf(ch, "%02d.out", i);
		out.open(ch);
		int N;
		in >> N;
		for (int j = 0; j < N; ++j)
		{
			in >> a[j];
		}
		std::sort(a, a + N);
		for (int j = 0; j < N; ++j)
		{
			out << a[j] << " ";
		}
		in.close();
		out.close();
	}

}