#include <vector>

long long get_result(std::vector<long long>& data)
{
	std::vector<long long> prefix_result(data.size() + 1);
	prefix_result[0] = 0;
	for (int i = 0; i < data.size(); i++)
	{
		long long p = data[i];
		prefix_result[i + 1] = p + prefix_result[i];
		for (int j = i - 1; j >= 0; j--)
		{
			p *= data[j];
			prefix_result[i + 1] = std::max(prefix_result[i + 1], p + prefix_result[j]);
		}
	}
	return prefix_result[data.size()];
}