#ifndef SPECIFIC
#define SPECIFIC

#include "advancedTree.h"


template <class Type>
struct SumMinMaxAssignMetaInformation{
	Type assigned;
	SumMinMaxAssignMetaInformation (Type to_assign): assigned(to_assign) 
	{}

	void merge (SumMinMaxAssignMetaInformation* additional)
	{
		assigned = additional->assigned;
	}

};

template <class Type>
struct SumMinMaxIncreaseMetaInformation{
	Type increased;
	SumMinMaxIncreaseMetaInformation (Type to_increase): increased(to_increase) {}

	void merge(SumMinMaxIncreaseMetaInformation* additional)
	{
		increased += additional->increased;
	}
};

template <class Type>
struct SumMinMaxIncreaseAssignMetaInformation{
	bool operation; // 0 - assign, 1- increase
	Type changed;

	SumMinMaxIncreaseAssignMetaInformation (bool operation, Type change): 
	operation(operation),changed(change) {}

	void merge(SumMinMaxIncreaseAssignMetaInformation* additional){
		if (operation) // here is increasing
		{ 
			if (additional->operation) //merge with increasing
				changed += additional->changed;
			else //merge with assign
			{
				operation=0;
				changed = additional -> changed;
			}
		} else // here is assigning
		{
			if (additional->operation) //merge with increasing
				changed += additional->changed;
			else //merge with assigning
			{
				changed = additional->changed;
			}
		}
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
	typedef SumMinMaxAssignMetaInformation<Type> MetaAssign;
	typedef SumMinMaxIncreaseMetaInformation<Type> MetaIncrease;
	typedef SumMinMaxIncreaseAssignMetaInformation<Type> MetaAssignIncrease;

	friend void push(RT& return_type_to_change, 
					 MetaAssign* information_to_push, const int& len)
	{
		return_type_to_change.sum = information_to_push->assigned * len;
		return_type_to_change.max = information_to_push->assigned;
		return_type_to_change.min = information_to_push->assigned;
	}

	friend void push(RT& return_type_to_change,
					 MetaIncrease* information_to_push, const int& len)
	{
		return_type_to_change.sum += information_to_push->increased * len;
		return_type_to_change.max += information_to_push->increased;
		return_type_to_change.min += information_to_push->increased;
	}

	friend void push(RT& return_type_to_change,
					MetaAssignIncrease* information_to_push, const int& len)
	{
		if (information_to_push->operation) //increase
		{
			return_type_to_change.sum += information_to_push->changed * len;
			return_type_to_change.max += information_to_push->changed;
			return_type_to_change.min += information_to_push->changed;
		} else // assign
		{
			return_type_to_change.sum = information_to_push->changed * len;
			return_type_to_change.max = information_to_push->changed;
			return_type_to_change.min = information_to_push->changed;
		}
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



typedef AdvancedSegmentTree< SumMinMaxAssignReturnType<int, 0, LONG_MAX, LONG_MIN>,
							 SumMinMaxAssignMetaInformation<int> > SegmentTree_SumMinMaxAssign_Int;

typedef AdvancedSegmentTree< SumMinMaxAssignReturnType<int, 0, LONG_MAX, LONG_MIN>,
							 SumMinMaxIncreaseMetaInformation<int> > SegmentTree_SumMinMaxIncrease_Int;

typedef AdvancedSegmentTree< SumMinMaxAssignReturnType<int, 0, LONG_MAX, LONG_MIN>,
							 SumMinMaxIncreaseAssignMetaInformation<int> > SegmentTree_SumMinMaxIncreaseAssign_Int;
typedef SumMinMaxIncreaseAssignMetaInformation<int> MakeChange_IncAss;
enum operations {assign, increase};


#endif