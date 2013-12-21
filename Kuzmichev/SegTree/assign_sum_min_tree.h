#include "return_types.h"
#include "assign_meta.h"

#define mp make_pair

class MethodsAssignSumMin
{
public:
	pair <int, int> merge(pair <int, int> L, pair <int, int> R)
	{
		//printf("merge (%d %d)  (%d %d)\n", L.first, L.second, R.first, R.second);
		return mp(L.first + R.first, min(L.second, R.second));
	}
	void apply(MetaAssign & m, pair <int, int> & p, Seg s)
	{
		//s: [L; R)
		if (m.assigned == false)
			return;
		
		p = mp(m.assignedValue * s.getLength(), m.assignedValue);
		if (DEBUG2) printf("apply assVal = %d p = (%d %d)\n", m.assignedValue, p.first, p.second);
	}
	//void push(MetaAssign & m, pair <int, int> & p, MetaAssign & leftMeta, MetaAssign & rightMeta, int L, int R)
	void push(MetaAssign & m, MetaAssign & leftMeta, MetaAssign & rightMeta)
	{
		//apply(m, p, L, R);
		
		if (m.assigned)
		{
			leftMeta = rightMeta = MetaAssign(true, m.assignedValue);
			if (DEBUG2) printf("push (%d) -> (%d) (%d)\n", m.assignedValue, leftMeta.assignedValue, rightMeta.assignedValue);	
		}
		//MetaAssign.clear();
	}
};

