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
	void push(MetaPlus & leftMeta, MetaPlus & rightMeta)
	{
		//apply(m, p, L, R);
		
		if (plusedValue != 0)
		{
			leftMeta.plusedValue += plusedValue;
			rightMeta.plusedValue += plusedValue;
			//if (DEBUG2) printf("push (%d) -> (%d) (%d)\n", m.assignedValue, leftMeta.assignedValue, rightMeta.assignedValue);	
		}
		//MetaAssignSumMin.clear();
	}
};


class MethodsPlusSumMinMax
{
public:
	int applyCnt;
	/*StructSumMinMax merge(StructSumMinMax L, StructSumMinMax R)
	{
		return L.merge(L, R);
	}*/
	MethodsPlusSumMinMax()
	{
		applyCnt = 0;
	}
	void apply(MetaPlus & m, StructSumMinMax & p, Seg s)
	{
		//s: [L; R)
		applyCnt++;
		if (m.plusedValue == 0)
			return;
		p.sum += m.plusedValue * s.getLength();
		p.min += m.plusedValue;
		p.max += m.plusedValue;
		//p = mp(m.assignedValue * s.getLength(), m.assignedValue);
		//if (DEBUG2) printf("apply assVal = %d p = (%d %d)\n", m.assignedValue, p.first, p.second);
	}

	/*void push(MetaPlus & m, MetaPlus & leftMeta, MetaPlus & rightMeta)
	{
		m.push(leftMeta, rightMeta);
	}*/	
};
