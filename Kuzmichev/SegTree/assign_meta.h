#ifndef META_ASSIGN_DECL
#define META_ASSIGN_DECL

struct MetaAssign
{
	bool assigned;
	int assignedValue;
	MetaAssign()
	{
		assigned = false;
		assignedValue = 0;
	}
	MetaAssign(bool _assigned, int _assignedValue) : assigned(_assigned), assignedValue(_assignedValue) {}
	void clear()
	{
		assigned = false;
		assignedValue = 0;
	}
	void push(MetaAssign & leftMeta, MetaAssign & rightMeta) const
	{
		//apply(m, p, L, R);
		//ASSERT_FALSE((m.plusedValue != 0) && m.assigned);
		if (assigned)
		{
			leftMeta = rightMeta = MetaAssign(true, assignedValue);
			//if (DEBUG2) printf("push (%d) -> (%d) (%d)\n", m.assignedValue, leftMeta.assignedValue, rightMeta.assignedValue);	
		}
	}
};

#endif