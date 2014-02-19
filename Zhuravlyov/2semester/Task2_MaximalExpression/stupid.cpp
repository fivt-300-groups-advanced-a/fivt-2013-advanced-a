#include <vector>

long long get_result_slow(std::vector<long long>& data)
{
	int n = data.size();
	long long result = 0;
	for (int i = 0; i < n; i++)
		result += data[i];
	for (int i = 1; i < (1 << (n-1)); i++)
	{
		long long local_result = 0;
		int q = i;
		int j = 1;
		long long p = data[0];
		for (int j = 1; j < n; j++) 
		{
			if (q % 2) 
				p *= data[j];
			else {
				local_result += p;
				p = data[j];
			}
			q /= 2;
		}
		local_result += p;
		if (local_result > result) result = local_result;
	}
	return result;
}