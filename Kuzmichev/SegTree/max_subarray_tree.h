#include "return_types.h"

#define mp make_pair

class MetaMaxSubarray
{
	public:
	static const int inf = 1e9;

	int maxSubarray;
	int maxPrefix;
	int maxSuffix;
	MetaMaxSubarray()
	{
		maxSubarray = maxPrefix = maxSuffix = -inf;
	}
	MetaMaxSubarray(int _maxSubarray, int _maxPrefix, int _maxSuffix) : maxSubarray(_maxSubarray), maxPrefix(_maxPrefix), maxSuffix(_maxSuffix) {}
	void clear()
	{
		maxSubarray = maxPrefix = maxSuffix = -inf;
	}
};




/*class Methods
{
public:
	pair <int, int> merge(pair <int, int> L, pair <int, int> R)
	{
		return mp(L.first + R.first, min(L.second, R.second));
	}
	void apply(MetaAssignSumMin & m, pair <int, int> & p, Seg s){}
	void push(MetaAssignSumMin & m, MetaAssignSumMin & leftMeta, MetaAssignSumMin & rightMeta){}
};*/



class MethodsMaxSubarray
{
public:
	MetaMaxSubarray merge(MetaMaxSubarray L, MetaMaxSubarray R)
	{
		//printf("merge (%d %d)  (%d %d)\n", L.first, L.second, R.first, R.second);
		
		return MetaMaxSubarray(max(L.maxSubarray, max(R.maxSubarray, L.maxSuffix + R.maxPrefix)), L.maxPrefix, R.maxSuffix);
	}
	void apply(MetaMaxSubarray & m, int & p, Seg s)
	{
		
	}
	
	void push(MetaMaxSubarray & m, MetaMaxSubarray & leftMeta, MetaMaxSubarray & rightMeta)
	{
		
	}
};