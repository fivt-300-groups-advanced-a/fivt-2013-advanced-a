#define DEBUG false
#define DEBUG2 false
#define DEBUG3 false
#include <vector>
#include "gtest/gtest.h"

using namespace std;

struct Seg
{
	//[L;R)
	int L;
	int R;
	Seg(){}
	Seg(int _L, int _R) : L(_L), R(_R) {}
	int getLength()
	{
		return R - L;
	}
	int getM()
	{
		return (L + R) / 2;
	}
};

template <class ReturnType, class Meta, class Methods>
class SegTree
{
protected:
	Methods methods;
	vector <ReturnType> tree;
	vector <Seg> segments;
	vector <Meta> metaInfo;
	int n;
	int treeSize;
	ReturnType neutral;
	int pushCnt;
	int getCnt;
	int segOperationCnt;
	int nonEmptySegOperationCnt;
	vector <ReturnType> initVector;
	void build(int i, int l, int r, bool flagInitFromVector)
	{
		segments[i] = Seg(l, r);
		if (l + 1 == r)
		{
			if (!flagInitFromVector) tree[i] = neutral;
			else tree[i] = initVector[l];
			return;
		}
		int m = (l + r) / 2;
		build(i * 2, l, m, flagInitFromVector);
		build(i * 2 + 1, m, r, flagInitFromVector);
		tree[i] = tree[i].merge(tree[i * 2], tree[i * 2 + 1]);
	}
	
	
	/*void applyAndClear(int i)
	{
		//printf("apply and clear i = %d\n", i);
		if (i >= treeSize)
			return;
		methods.apply(metaInfo[i], tree[i], segments[i]);
		metaInfo[i].clear();
	}*/

	/*inline void safeApply(int i)
	{
		if (i >= treeSize)
			return;
		methods.apply(metaInfo[i], tree[i], segments[i]);
	}*/

	inline void completePush(int i)
	{
		
		methods.apply(metaInfo[i], tree[i], segments[i]);
		if (segments[i].L + 1 != segments[i].R)
		{
			pushCnt++;
			metaInfo[i].push(metaInfo[i * 2], metaInfo[i * 2 + 1]);
		}
		metaInfo[i].clear();
	}

	ReturnType innerGet(int i, int queryL, int queryR)
	{
		getCnt++;
		EXPECT_TRUE(queryL < queryR);
		if (DEBUG2)
			printf("get i = %d\n", i);
		completePush(i);
		if (segments[i].L == queryL && segments[i].R == queryR)
			return tree[i];
		
			//return methods.convert(tree[i]);
		int m = segments[i].getM();
		if (queryR <= m)
			return innerGet(i * 2, queryL, queryR);
		if (queryL >= m)
			return innerGet(i * 2 + 1, queryL, queryR);
		ReturnType leftResult = innerGet(i * 2,queryL, min(m, queryR));
		ReturnType rightResult = innerGet(i * 2 + 1, max(m, queryL), queryR);
		return leftResult.merge(leftResult, rightResult);
	}
	/*void innerUpdateInPosition(int i,  int pos, ReturnType & newValue)
	{
		methods.push(i);
		if (segments[i].L == pos && segments[i].R == pos + 1)
		{
			tree[i] = newValue;
			return;
		}
		int m = (segments[i].L + segments[i].R) / 2;
		if (pos < m)
			innerUpdateInPosition(i * 2, pos, newValue);
		else innerUpdateInPosition(i * 2 + 1, pos, newValue);
		tree[i] = methods.merge(tree[i * 2], tree[i * 2 + 1]);
	}*/
	void innerSegOperation(int i, int queryL, int queryR, Meta segOperationMeta)
	{
		segOperationCnt++;
		completePush(i);
		if (queryL >= queryR)
			return;
		nonEmptySegOperationCnt++;
		if (DEBUG2)
			printf("segOperation i = %d query = (%d %d)\n", i, queryL, queryR);
		
		if (segments[i].L == queryL && segments[i].R == queryR)
		{
			metaInfo[i] = segOperationMeta;
			//printf("innersegOperation i = %d %d %d\n", i, segments[i].L, segments[i].R);
			//applyAndClear(i);
			completePush(i);
			return;
		}
		int m = segments[i].getM();
		innerSegOperation(i * 2, queryL, min(m, queryR), segOperationMeta);
		innerSegOperation(i * 2 + 1, max(m, queryL), queryR, segOperationMeta);
		tree[i] = tree[i * 2].merge(tree[i * 2], tree[i * 2 + 1]);
	}
	void init()
	{
		pushCnt = getCnt = segOperationCnt = nonEmptySegOperationCnt = 0;
		int tmp = 1;
		while(tmp < n)
			tmp *= 2;
		n = tmp;
		
		treeSize = n * 2;
		tree.resize(treeSize);
		segments.resize(treeSize);
		metaInfo.resize(treeSize);
		
	}
public:
	SegTree(Methods _methods, int _n, ReturnType _neutral) : methods(_methods), n(_n), neutral(_neutral)
	{
		init();
		build(1, 0, n, false);
	}
	SegTree(Methods _methods, int _n, ReturnType _neutral, vector<ReturnType> & _initVector) : methods(_methods), n(_n), neutral(_neutral), initVector(_initVector)
	{
		init();
		for (int j = initVector.size(); j < treeSize; j++)
			initVector.push_back(neutral);
		build(1, 0, n, true);
	}
	ReturnType get(int l, int r)
	{
		return innerGet(1, l, r + 1);
	}
	/*void updateInPosition(int position, ReturnType newValue)
	{
		innerUpdateInPosition(1, position, newValue);
	}*/
	void segOperation(int l, int r, Meta segOperationMeta)
	{
		innerSegOperation(1, l, r + 1, segOperationMeta);
	}
	void printCounters(int opNumber)
	{
		printf("tree size = %d\n", treeSize);
		int depth = 1;
		while((1 << depth) != treeSize)
		{
			depth++;
		}
		printf("depth %d\n", depth);
		printf("applyCnt = %d\n", methods.applyCnt);
		ASSERT_LE(methods.applyCnt, depth * 4 * opNumber);
		printf("pushCnt = %d\n", pushCnt);
		ASSERT_LE(pushCnt, depth * 4 * opNumber);
		printf("getCnt = %d\n", getCnt);
		ASSERT_LE(getCnt, depth * 3 * opNumber);
		printf("segOperationCnt = %d (non-empty %d)\n", segOperationCnt, nonEmptySegOperationCnt);
		ASSERT_LE(segOperationCnt, depth * 3 * opNumber);
	}

	template <class ReturnType, class Meta, class Methods>
	friend class SegTreeChecker;

};

template <class ReturnType, class Meta, class Methods>
class SegTreeChecker
{
private:
	SegTree <ReturnType, Meta, Methods> * segTree;
public:
	SegTreeChecker(SegTree <ReturnType, Meta, Methods> * _segTree) : segTree(_segTree) {}
	/*void checkApplyCnt(int rightVal)
	{
		ASSERT_EQ(segTree->applyCnt, rightVal);
	}*/
	void checkPushCnt(int rightVal)
	{
		ASSERT_EQ(segTree->pushCnt, rightVal);
	}
	void checkGetCnt(int rightVal)
	{
		ASSERT_EQ(segTree->getCnt, rightVal);
	}
	void checkSegOperationCnt(int rightVal)
	{
		ASSERT_EQ(segTree->segOperationCnt, rightVal);
	}
	void checkNonEmptyCnt(int rightVal)
	{
		ASSERT_EQ(segTree->nonEmptySegOperationCnt, rightVal);
	}
	void checkAll(int push, int get, int segOp, int nonEmpty)
	{
		checkPushCnt(push);
		checkGetCnt(get);
		checkSegOperationCnt(segOp);
		checkNonEmptyCnt(nonEmpty);
	}
	void checkTreeElem(int number, ReturnType rightVal)
	{
		ASSERT_EQ(segTree->tree[number], rightVal);
	}
	void checkMeta(int number, Meta rightVal)
	{
		ASSERT_EQ(segTree->metaInfo[number], rightVal);
	}
};