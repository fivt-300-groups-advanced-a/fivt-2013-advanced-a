#include "return_types.h"

#define mp make_pair

struct MetaAssignSumMin
{
	bool assigned;
	int assignedValue;
	MetaAssignSumMin()
	{
		assigned = false;
		assignedValue = 0;
	}
	MetaAssignSumMin (bool _assigned, int _assignedValue) : assigned(_assigned), assignedValue(_assignedValue) {}
	void clear()
	{
		assigned = false;
		assignedValue = 0;
	}
};

class MethodsAssignSumMin
{
public:
	pair <int, int> merge(pair <int, int> L, pair <int, int> R)
	{
		//printf("merge (%d %d)  (%d %d)\n", L.first, L.second, R.first, R.second);
		return mp(L.first + R.first, min(L.second, R.second));
	}
	void apply(MetaAssignSumMin & m, pair <int, int> & p, Seg s)
	{
		//s: [L; R)
		if (m.assigned == false)
			return;
		
		p = mp(m.assignedValue * s.getLength(), m.assignedValue);
		if (DEBUG2) printf("apply assVal = %d p = (%d %d)\n", m.assignedValue, p.first, p.second);
	}
	//void push(MetaAssignSumMin & m, pair <int, int> & p, MetaAssignSumMin & leftMeta, MetaAssignSumMin & rightMeta, int L, int R)
	void push(MetaAssignSumMin & m, MetaAssignSumMin & leftMeta, MetaAssignSumMin & rightMeta)
	{
		//apply(m, p, L, R);
		
		if (m.assigned)
		{
			leftMeta = rightMeta = MetaAssignSumMin(true, m.assignedValue);
			if (DEBUG2) printf("push (%d) -> (%d) (%d)\n", m.assignedValue, leftMeta.assignedValue, rightMeta.assignedValue);	
		}
		//MetaAssignSumMin.clear();
	}
};

