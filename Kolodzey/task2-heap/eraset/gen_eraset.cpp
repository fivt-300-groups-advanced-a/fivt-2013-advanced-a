#include <fstream>
#include <set>
#include <random>
#include <iostream>

const int N_REQUEST = 100500;
const int MAX_N_OF_INSERTIONS = 100500;

int main()
{
	std::ofstream out ("fresh_test2.in");
	std::default_random_engine generator;
	std::uniform_int_distribution<int> cmddistrib(0,3);
	std::uniform_int_distribution<int> valdistrib(0,100500100);

	std::set<int> aviable;
	int nInserted = 0;

	out << N_REQUEST << std::endl;
	for (int i = 0; i < N_REQUEST; ++i)
	{
		if ((i % 5000) == 0)
			std::cerr << "gened " << i << std::endl;
		//std::cerr << "cmd #" << i << std::endl;
		int cmd = cmddistrib(generator);
		if ((cmd < 3) || (aviable.size() == 0))
		{
			out << 0 << " ";
			int val = valdistrib(generator);
			out << val << std::endl;
			aviable.insert(nInserted);
			++nInserted;
		}
		else
		{
			out << 1 << " ";
			std::uniform_int_distribution<int> inddistrib(0,aviable.size() - 1);
			int ind = inddistrib(generator);
			std::set<int>::iterator it = aviable.begin();
			for (int j = 0; j < ind; ++j)
				++it;
			ind = *it;
			out << ind << std::endl;
			aviable.erase(ind);
		}
	}
	out.close();
	return 0;
}