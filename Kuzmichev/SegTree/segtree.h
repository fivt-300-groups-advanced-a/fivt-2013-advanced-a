#define DEBUG false
#define DEBUG2 false
#define DEBUG3 false
#include <vector>

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
	void build(int i, int l, int r)
	{
		segments[i] = Seg(l, r);
		if (l + 1 == r)
		{
			tree[i] = neutral;
			return;
		}
		int m = (l + r) / 2;
		build(i * 2, l, m);
		build(i * 2 + 1, m, r);
		tree[i] = methods.merge(tree[i], tree[i * 2]);
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

	inline void safePush(int i)
	{
		if (i >= treeSize)
			return;
		methods.apply(metaInfo[i], tree[i], segments[i]);
		if (segments[i].L + 1 != segments[i].R)
		{
			methods.push(metaInfo[i], metaInfo[i * 2], metaInfo[i * 2 + 1]);
		}
		metaInfo[i].clear();
	}

	//void completePush(int i, Meta & m, ReturnType & p, Meta & leftMeta, Meta & rightMeta)
	void completePush(int i)
	{
		if (DEBUG2)
			printf("complete push i = %d\n", i);
		safePush(i);
		safePush(i * 2);
		safePush(i * 2 + 1);
		//critical change!
		/*safeApply(i * 2);
		safeApply(i * 2 + 1);*/

	}

	/*void applyAndPush()
	{
		methods.push(metaInfo[i], metaInfo[i * 2], metaInfo[i * 2 + 1]);
	}*/

	ReturnType innerGet(int i, int queryL, int queryR)
	{
		
		if (queryL >= queryR)
			return neutral;
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
		return methods.merge(leftResult, rightResult);
	}
	void innerUpdateInPosition(int i,  int pos, ReturnType & newValue)
	{
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
	}
	void innerSegOperation(int i, int queryL, int queryR, Meta segOperationMeta)
	{
		
		if (queryL >= queryR)
			return;
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
		completePush(i);
		innerSegOperation(i * 2, queryL, min(m, queryR), segOperationMeta);
		innerSegOperation(i * 2 + 1, max(m, queryL), queryR, segOperationMeta);
		tree[i] = methods.merge(tree[i * 2], tree[i * 2 + 1]);
	}
public:
	SegTree(Methods _methods, int _n, ReturnType _neutral) : methods(_methods), n(_n), neutral(_neutral)
	{
		int tmp = 1;
		while(tmp < n)
			tmp *= 2;
		n = tmp;
		
		treeSize = n * 2;
		if (DEBUG2)
			printf("n = %d treeSize = %d\n", n, treeSize);
		tree.resize(treeSize);
		segments.resize(treeSize);
		metaInfo.resize(treeSize);
		build(1, 0, n);
	}
	ReturnType get(int l, int r)
	{
		return innerGet(1, l, r + 1);
	}
	void updateInPosition(int position, ReturnType newValue)
	{
		innerUpdateInPosition(1, position, newValue);
	}
	void segOperation(int l, int r, Meta segOperationMeta)
	{
		innerSegOperation(1, l, r + 1, segOperationMeta);
	}

};