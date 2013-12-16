#ifndef FRIENDLY_STRUCTURES
#define FRIENDLY_STRUCTURES

#include "assigmentTree.h"
#include "ass_incTree.h"
#include "increaseTree.h"
#include "NumbOfPermanenceSegmentsTree.h"
#include "maxSubsegmentTree.h"

struct SumMinMaxAssignTreeTestFriend
{
	typedef SumMinMaxAssignTree<int, 0, LONG_MAX, LONG_MIN>::ReturnType ReturnType;
	typedef SumMinMaxAssignTree<int, 0, LONG_MAX, LONG_MIN>::MetaInformation MetaInformation;
	typedef SumMinMaxAssignTree<int, 0, LONG_MAX, LONG_MIN>::Unite Unite;
	typedef SumMinMaxAssignTree<int, 0, LONG_MAX, LONG_MIN>::Push Push;
	typedef SumMinMaxAssignTree<int, 0, LONG_MAX, LONG_MIN>::Merge Merge;
};

struct SumMinMaxIncreaseTreeTestFriend
{
	typedef SumMinMaxIncreaseTree<int, 0, LONG_MAX, LONG_MIN>::ReturnType ReturnType;
	typedef SumMinMaxIncreaseTree<int, 0, LONG_MAX, LONG_MIN>::MetaInformation MetaInformation;
	typedef SumMinMaxIncreaseTree<int, 0, LONG_MAX, LONG_MIN>::Unite Unite;
	typedef SumMinMaxIncreaseTree<int, 0, LONG_MAX, LONG_MIN>::Push Push;
	typedef SumMinMaxIncreaseTree<int, 0, LONG_MAX, LONG_MIN>::Merge Merge;
};

struct SumMinMaxIncreaseAssignTreeTestFriend
{
	typedef SumMinMaxIncreaseAssignTree<int, 0, LONG_MAX, LONG_MIN>::ReturnType ReturnType;
	typedef SumMinMaxIncreaseAssignTree<int, 0, LONG_MAX, LONG_MIN>::MetaInformation MetaInformation;
	typedef SumMinMaxIncreaseAssignTree<int, 0, LONG_MAX, LONG_MIN>::Unite Unite;
	typedef SumMinMaxIncreaseAssignTree<int, 0, LONG_MAX, LONG_MIN>::Push Push;
	typedef SumMinMaxIncreaseAssignTree<int, 0, LONG_MAX, LONG_MIN>::Merge Merge;
};

struct NumberOfPermanenceSegmentsTreeTestFriend
{
	typedef NumberOfPermanenceSegmentsTree<int>::ReturnType ReturnType;
	typedef NumberOfPermanenceSegmentsTree<int>::MetaInformation MetaInformation;
	typedef NumberOfPermanenceSegmentsTree<int>::Unite Unite;
	typedef NumberOfPermanenceSegmentsTree<int>::Push Push;
	typedef NumberOfPermanenceSegmentsTree<int>::Merge Merge;
};

struct MaxSubsegmentTreeTestFriend
{
	typedef MaxSubsegmentTree<int ,0, LONG_MIN>::ReturnType ReturnType;
	typedef MaxSubsegmentTree<int ,0, LONG_MIN>::MetaInformation MetaInformation;
	typedef MaxSubsegmentTree<int ,0, LONG_MIN>::Unite Unite;
	typedef MaxSubsegmentTree<int ,0, LONG_MIN>::Push Push;
	typedef MaxSubsegmentTree<int ,0, LONG_MIN>::Merge Merge;
};

#endif