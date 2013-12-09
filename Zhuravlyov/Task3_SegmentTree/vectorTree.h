#ifndef VECTOR_TREE
#define VECTOR_TREE
#include <vector>

template <class Type>
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

	void change_increase(unsigned int left,unsigned int right,int increasion)
	{
		for (unsigned int i=left; i <= right; i++)
			data[i] += increasion;
	}

	void change_assign(unsigned int left, unsigned int right, int assigment)
	{
		for (unsigned int i=left; i <= right; i++)
			data[i] = assigment;
	}
};

#endif