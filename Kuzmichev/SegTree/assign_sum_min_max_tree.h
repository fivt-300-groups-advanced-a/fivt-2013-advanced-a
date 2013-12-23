#include "return_types.h"
#include "assign_meta.h"

#define mp make_pair

class MethodsAssignSumMinMax
{
public:
	int applyCnt;
	MethodsAssignSumMinMax()
	{
		applyCnt = 0;
	}
	/*StructSumMinMax merge(StructSumMinMax L, StructSumMinMax R)
	{
		return L.merge(L, R);
	}*/
	void apply(MetaAssign & m, StructSumMinMax & p, Seg s)
	{
		applyCnt++;
		if (m.assigned == false)
			return;
		
		p = StructSumMinMax(m.assignedValue * s.getLength(), m.assignedValue, m.assignedValue);
	}
	
	/*void push(MetaAssign & m, MetaAssign & leftMeta, MetaAssign & rightMeta)
	{
		m.push(leftMeta, rightMeta);
	}*/
};

