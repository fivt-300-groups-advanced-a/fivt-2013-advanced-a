#ifndef VECTOR_TREE
#define VECTOR_TREE
#include <vector>

template <class Type, Type neutralSumElement>
class vectorTree
{
private:
	std::vector<Type> data;

	template <class Functor>
	Type get_operation(unsigned int &left,unsigned int &right, Functor function)
	{
		Type result = data[left];
		for (unsigned int i=left+1; i<=right; i++)
			result = function(result, data[i]);
		return result;
	}

public:
	template <class RandomAccessIterator>
	vectorTree(RandomAccessIterator first,
				RandomAccessIterator last)
	{
		data.resize(1);
		while (first!=last) {
			data.push_back(*first);
			first++;
		}
	}

	Type getSum(unsigned int left, unsigned int right)
	{
		return get_operation(left, right, std::plus<Type>());
	}

	Type getMin(unsigned int left, unsigned int right)
	{
		return get_operation(left, right, std::min<Type>);
	}

	Type getMax(unsigned int left, unsigned int right)
	{
		return get_operation(left, right, std::max<Type>);
	}

	unsigned int NumberOfPermSegments(unsigned int left, unsigned int right)
	{
		unsigned int k = 1;
		for (unsigned int i = left + 1; i <= right; i++)
			if (data[i] != data[i-1]) 
				k++;
		return k;
	}

	Type maxSubSum(unsigned int left, unsigned int right)
	{
		Type sum = data[left];
		Type max = data[left];
		for (unsigned int i = left + 1; i <= right; i++)
		{
			sum += data[i];
			max = std::max(sum, max);
			sum = std::max(sum, neutralSumElement);
		}
		return max;
	}

	void increase(unsigned int left,unsigned int right,Type increasion)
	{
		for (unsigned int i=left; i <= right; i++)
			data[i] += increasion;
	}

	void assign(unsigned int left, unsigned int right, Type assigment)
	{
		for (unsigned int i=left; i <= right; i++)
			data[i] = assigment;
	}
};

#endif