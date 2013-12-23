#ifndef META_PLUS_ASSIGN_DECL
#define META_PLUS_ASSIGN_DECL

struct MetaPlusAssign
{
	bool assigned;
	int assignedValue;
	int plusedValue;
	MetaPlusAssign()
	{
		assigned = false;
		assignedValue = plusedValue = 0;
	}
	MetaPlusAssign(bool _assigned, int _assignedValue, int _plusedValue) : assigned(_assigned), assignedValue(_assignedValue), plusedValue(_plusedValue) {}
	void clear()
	{
		assigned = false;
		assignedValue = plusedValue = 0;
	}
	void push(MetaPlusAssign & leftMeta, MetaPlusAssign & rightMeta) const
	{
		//apply(m, p, L, R);
		//ASSERT_FALSE((m.plusedValue != 0) && m.assigned);
		if (assigned)
		{
			leftMeta = rightMeta = MetaPlusAssign(true, assignedValue, 0);
		}
		if (plusedValue != 0)
		{
			leftMeta.plusedValue += plusedValue;
			rightMeta.plusedValue += plusedValue;
			//if (DEBUG2) printf("push (%d) -> (%d) (%d)\n", m.assignedValue, leftMeta.assignedValue, rightMeta.assignedValue);	
		}
	}
	bool operator == (MetaPlusAssign an ) const
	{
		return assigned == an.assigned && assignedValue == an.assignedValue && plusedValue == an.plusedValue;
	}
};

#endif