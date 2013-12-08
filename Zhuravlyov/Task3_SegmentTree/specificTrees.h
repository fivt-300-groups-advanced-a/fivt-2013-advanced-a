#ifndef SPECIFIC
#define SPECIFIC

#include "advancedTree.h"


template <class Type>
struct SumAssignMetaInformation{
	Type assigned;
	SumAssignMetaInformation (Type to_assign): assigned(to_assign) 
	{}
	void merge (SumAssignMetaInformation* additional)
	{
		assigned = additional->assigned;
	}
};

template <class Type, Type neutralSumElement>
struct SumAssignReturnType{
	Type sum;
	SumAssignReturnType(): sum(neutralSumElement) {}
	SumAssignReturnType (Type element): sum(element) {}
	typedef SumAssignReturnType<Type,neutralSumElement> RT;
	typedef SumAssignMetaInformation<Type> MI;
	void push (MI* information_to_push, int len)
	{
		sum = information_to_push->assigned * len;
	}
	friend RT unite (const RT& first, const RT& second)
	{
		return (first.sum + second.sum);
	}
};

template <class Type, Type neutralSumElement,
				      Type neutralMinElement, 
					  Type neutralMaxElement>
struct SumMinMaxAssignReturnType{
	Type sum, min, max;
	SumMinMaxAssignReturnType(): sum(neutralSumElement),
								 min(neutralMinElement),
								 max(neutralMaxElement) {}
	SumMinMaxAssignReturnType(Type element): sum(element),
											 min(element),
											 max(element) {}
	typedef SumMinMaxAssignReturnType<Type, neutralSumElement, 
											neutralMinElement, 
											neutralMaxElement> RT;
	typedef SumAssignMetaInformation<Type> MI;
	void push(MI* information_to_push, int len)
	{
		sum = information_to_push->assigned * len;
		max = information_to_push->assigned;
		min = information_to_push->assigned;
	}
	friend RT unite(const RT& first, const RT& second)
	{
		RT result;
		result.sum = first.sum + second.sum;
		result.min = std::min(first.min, second.min);
		result.max = std::max(first.max, second.max);
		return result;
	}
};


typedef AdvancedSegmentTree< SumAssignReturnType<int, 0>, 
							 SumAssignMetaInformation<int> > SegmentTree_SumAssign_Int;
typedef AdvancedSegmentTree< SumMinMaxAssignReturnType<int, 0, LONG_MAX, LONG_MIN>,
							 SumAssignMetaInformation<int> > SegmentTree_SumMinMaxAssign_Int;


#endif