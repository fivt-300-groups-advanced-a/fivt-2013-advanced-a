#include <fstream>
#include <set>
#include <random>
#include <iostream>

/*
const int N_REQUEST = 100500;

int main()
{
	std::ofstream out ("fresh_test.in");
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distr(0,1000000007);

	out << N_REQUEST << std::endl;
	for (int i = 0; i < N_REQUEST; ++i)
	{
		int h1 = distr(generator);
    int h2 = distr(generator);
    out << h1 << " " << h2 << std::endl;
	}
	return 0;
}
*/

int main()
{
  std::ofstream out ("fresh_test.in"); 
  out << 256 << std::endl;
  for (int i = 0; i < 16; ++i)
    for (int j = 0; j < 16; ++j)
      out << i << " " << j << std::endl;
}