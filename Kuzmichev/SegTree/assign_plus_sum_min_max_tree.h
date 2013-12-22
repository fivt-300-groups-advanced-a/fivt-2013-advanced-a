#include "assign_plus_meta.h"

class MethodsPlusAssignSumMinMax
{
public:
	StructSumMinMax merge(StructSumMinMax L, StructSumMinMax R)
	{
		return L.merge(L, R);
	}
	void apply(MetaPlusAssign & m, StructSumMinMax & p, Seg s)
	{
		//s: [L; R)
		if (m.assigned)
		{
			p.sum = m.assignedValue * s.getLength();
			p.min = p.max = m.assignedValue = m.assignedValue;
		}
		if (m.plusedValue != 0)
		{
			p.sum += m.plusedValue * s.getLength();
			p.min += m.plusedValue;
			p.max += m.plusedValue;
		}
	}
	//void push(MetaAssignSumMin & m, pair <int, int> & p, MetaAssignSumMin & leftMeta, MetaAssignSumMin & rightMeta, int L, int R)
	void push(MetaPlusAssign & m, MetaPlusAssign & leftMeta, MetaPlusAssign & rightMeta)
	{
		m.push(leftMeta, rightMeta);
	}
};