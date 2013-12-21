#include "return_types.h"

struct MetaPlus
{
	int plusedValue;
	explicit MetaPlus(int _plusedValue) : plusedValue(_plusedValue) {}
	MetaPlus()
	{
		plusedValue = 0;
	}
	void clear()
	{
		plusedValue = 0;
	}
};


class MethodsPlusSumMinMax
{
public:
	StructSumMinMax merge(StructSumMinMax L, StructSumMinMax R)
	{
		//printf("merge (%d %d)  (%d %d)\n", L.first, L.second, R.first, R.second);
		return StructSumMinMax(L.sum + R.sum, min(L.min, R.min), max(L.max, R.max));
	}
	void apply(MetaPlus & m, StructSumMinMax & p, Seg s)
	{
		//s: [L; R)
		if (m.plusedValue == 0)
			return;
		p.sum += m.plusedValue * s.getLength();
		p.min += m.plusedValue;
		p.max += m.plusedValue;
		//p = mp(m.assignedValue * s.getLength(), m.assignedValue);
		//if (DEBUG2) printf("apply assVal = %d p = (%d %d)\n", m.assignedValue, p.first, p.second);
	}
	//void push(MetaAssignSumMin & m, pair <int, int> & p, MetaAssignSumMin & leftMeta, MetaAssignSumMin & rightMeta, int L, int R)
	void push(MetaPlus & m, MetaPlus & leftMeta, MetaPlus & rightMeta)
	{
		//apply(m, p, L, R);
		
		if (m.plusedValue != 0)
		{
			leftMeta.plusedValue += m.plusedValue;
			rightMeta.plusedValue += m.plusedValue;
			//if (DEBUG2) printf("push (%d) -> (%d) (%d)\n", m.assignedValue, leftMeta.assignedValue, rightMeta.assignedValue);	
		}
		//MetaAssignSumMin.clear();
	}
};
