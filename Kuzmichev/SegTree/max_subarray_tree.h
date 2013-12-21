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


/*class MethodsPlusAssignSumMinMax
{
public:
	StructSumMinMax merge(StructSumMinMax L, StructSumMinMax R)
	{
		//printf("merge (%d %d)  (%d %d)\n", L.first, L.second, R.first, R.second);
		return StructSumMinMax(L.sum + R.sum, min(L.min, R.min), max(L.max, R.max));
	}
	void apply(MetaPlusAssignSumMinMax & m, StructSumMinMax & p, Seg s)
	{
		//s: [L; R)
		ASSERT_FALSE((m.plusedValue != 0) && m.assigned);
		if (m.plusedValue != 0)
		{
			p.sum += m.plusedValue * s.getLength();
			p.min += m.plusedValue;
			p.max += m.plusedValue;
		}
		else if (m.assigned)
		{
			p.sum = m.assignedValue * s.getLength();
			p.min = p.max = m.assignedValue = m.assignedValue;
		}
	}
	//void push(MetaAssignSumMin & m, pair <int, int> & p, MetaAssignSumMin & leftMeta, MetaAssignSumMin & rightMeta, int L, int R)
	void push(MetaPlusSumMinMax & m, MetaPlusSumMinMax & leftMeta, MetaPlusSumMinMax & rightMeta)
	{
		//apply(m, p, L, R);
		ASSERT_FALSE((m.plusedValue != 0) && m.assigned);
		if (m.plusedValue != 0)
		{
			leftMeta.plusedValue += m.plusedValue;
			rightMeta.plusedValue += m.plusedValue;
			//if (DEBUG2) printf("push (%d) -> (%d) (%d)\n", m.assignedValue, leftMeta.assignedValue, rightMeta.assignedValue);	
		}
		else if (m.assigned)
		{
			leftMeta = rightMeta = MetaPlusAssignSumMin(true, m.assignedValue);
		}
		//MetaAssignSumMin.clear();
	}
};*/

class Methods
{
public:
	pair <int, int> merge(pair <int, int> L, pair <int, int> R)
	{
		return mp(L.first + R.first, min(L.second, R.second));
	}
	void apply(MetaAssignSumMin & m, pair <int, int> & p, Seg s){}
	void push(MetaAssignSumMin & m, MetaAssignSumMin & leftMeta, MetaAssignSumMin & rightMeta){}
};



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