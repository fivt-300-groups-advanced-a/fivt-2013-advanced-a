#include "return_types.h"
#include "assign_plus_meta.h"

class MethodsConstancySegments
{
public:
	/*StructConstancySegments merge(StructConstancySegments L, StructConstancySegments R)
	{
		return L.merge(L, R);
	}*/
	
	int applyCnt;
	MethodsConstancySegments()
	{
		applyCnt = 0;
	}
	void apply(MetaPlusAssign & m, StructConstancySegments & p, Seg s)
	{
		//s: [L; R)
		applyCnt++;
		if (m.assigned)
		{
			p.answer = 1;
			p.leftest = p.rightest = m.assignedValue;
		}
		if (m.plusedValue != 0)
		{
			p.leftest += m.plusedValue;
			p.rightest += m.plusedValue;
		}
	}
	//void push(MetaAssignSumMin & m, pair <int, int> & p, MetaAssignSumMin & leftMeta, MetaAssignSumMin & rightMeta, int L, int R)
	/*void push(MetaPlusAssign & m, MetaPlusAssign & leftMeta, MetaPlusAssign & rightMeta)
	{
		m.push(leftMeta, rightMeta);
	}*/
};