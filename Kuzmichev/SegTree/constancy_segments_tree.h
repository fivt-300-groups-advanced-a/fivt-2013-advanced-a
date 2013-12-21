#include "return_types.h"
#include "assign_meta.h"

class MethodsConstancySegments
{
public:
	StructConstancySegments merge(StructConstancySegments L, StructConstancySegments R)
	{
		//printf("merge (%d %d)  (%d %d)\n", L.first, L.second, R.first, R.second);
		int newAnswer = L.answer + R.answer;
		if (L.rightest == R.leftest)
			newAnswer--;
		return StructConstancySegments(newAnswer, L.leftest, R.rightest);
	}
	void apply(MetaAssign & m, StructConstancySegments & p, Seg s)
	{
		//s: [L; R)
		if (!m.assigned)
			return;
		p.answer = 1;
		p.leftest = p.rightest = m.assignedValue;
	}
	//void push(MetaAssignSumMin & m, pair <int, int> & p, MetaAssignSumMin & leftMeta, MetaAssignSumMin & rightMeta, int L, int R)
	void push(MetaAssign & m, MetaAssign & leftMeta, MetaAssign & rightMeta)
	{
		//apply(m, p, L, R);
		//ASSERT_FALSE((m.plusedValue != 0) && m.assigned);
		if (m.assigned)
		{
			leftMeta = rightMeta = MetaAssign(true, m.assignedValue);
			if (DEBUG2) printf("push (%d) -> (%d) (%d)\n", m.assignedValue, leftMeta.assignedValue, rightMeta.assignedValue);	
		}
	}
};