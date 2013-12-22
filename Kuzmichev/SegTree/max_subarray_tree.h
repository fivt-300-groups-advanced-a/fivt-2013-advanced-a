#include "return_types.h"

class MethodsMaxSubarray
{
public:
	StructMaxSubarray merge(StructMaxSubarray L, StructMaxSubarray R)
	{
		return L.merge(L, R);
	}
	void apply(MetaAssign & m, StructMaxSubarray & p, Seg s)
	{
		if (!m.assigned)
			return;
		p.sum = m.assignedValue * s.getLength();
		if (m.assignedValue > 0)
		{
			p.maxSuffix = p.maxPrefix = p.maxSubarray = p.sum;
		} 
		else p.maxSuffix = p.maxPrefix = p.maxSubarray = 0;
	}
	
	void push(MetaAssign & m, MetaAssign & leftMeta, MetaAssign & rightMeta)
	{
		m.push(leftMeta, rightMeta);	
	}
};