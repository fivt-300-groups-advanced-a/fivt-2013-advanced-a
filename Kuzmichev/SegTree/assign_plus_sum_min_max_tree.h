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
};

class MethodsPlusAssignSumMinMax
{
public:
	StructSumMinMax merge(StructSumMinMax L, StructSumMinMax R)
	{
		//printf("merge (%d %d)  (%d %d)\n", L.first, L.second, R.first, R.second);
		return StructSumMinMax(L.sum + R.sum, min(L.min, R.min), max(L.max, R.max));
	}
	void apply(MetaPlusAssign & m, StructSumMinMax & p, Seg s)
	{
		//s: [L; R)
		//ASSERT_FALSE((m.plusedValue != 0) && m.assigned);
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
		//apply(m, p, L, R);
		//ASSERT_FALSE((m.plusedValue != 0) && m.assigned);
		if (m.assigned)
		{
			leftMeta = rightMeta = MetaPlusAssign(true, m.assignedValue, 0);
		}
		if (m.plusedValue != 0)
		{
			leftMeta.plusedValue += m.plusedValue;
			rightMeta.plusedValue += m.plusedValue;
			//if (DEBUG2) printf("push (%d) -> (%d) (%d)\n", m.assignedValue, leftMeta.assignedValue, rightMeta.assignedValue);	
		}
	}
};