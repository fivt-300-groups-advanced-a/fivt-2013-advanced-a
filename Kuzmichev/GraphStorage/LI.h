#ifndef LI
#define LI

#include <memory>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

typedef unsigned int uint;
typedef vector <bool> ::iterator vectorBoolIter;

class AbstractIter
{
public:
	virtual bool next() = 0;
	virtual const uint getCurrentVertex() = 0;
	virtual const bool isEnd() = 0;
	virtual ~AbstractIter(){}
};

typedef unique_ptr <AbstractIter> IterPtr;

/*template <typename Iterator>*/
class StandardIter : AbstractIter
{
public:
	//StandardIter(Iterator _cur, Iterator _end) : cur(_cur), end(_end) {}
	StandardIter(vector <uint> ::iterator _cur, vector <uint> ::iterator _end) : cur(_cur), end(_end) {}
	bool next() override
	{
		if (cur == end)
			return false;
		cur++;
		return (cur != end);
	}
	const uint getCurrentVertex() override
	{
		return *cur;
	}
	const bool isEnd() override
	{
		return cur == end;
	}
private:
	vector <uint> :: iterator cur, end;
	//Iterator cur, end;
};

class BitsetIter : AbstractIter
{
public:
	BitsetIter(vectorBoolIter _begin, vectorBoolIter _end) : begin(_begin), end(_end)
	{
		cur = begin;
		while (cur != end && *cur == false)
			cur++;
	}
	bool next() override
	{
		if (cur == end)
			return false;
		do
		{
			cur++;
		} while (cur != end && *cur == false);
		return cur != end;
	}
	const uint getCurrentVertex() override
	{
		return cur - begin;
	}
	const bool isEnd() override
	{
		return cur == end;
	}
private:
	vectorBoolIter cur, begin, end;
};

class FuncIter : AbstractIter
{
public:
	explicit FuncIter(uint _to) : to(_to), finished(false) {}
	bool next() override
	{
		finished = true;
		return false;
	}
	const uint getCurrentVertex() override
	{
		assert(!finished);
		return to;
	}
	const bool isEnd() override
	{
		return finished;
	}
private:
	bool finished;
	uint to;
};

class EmptyIter : AbstractIter
{
public:
	EmptyIter(){}
	bool next() override
	{
		return false;
	}
	const uint getCurrentVertex() override
	{
		assert(false);
		return 42;
	}
	const bool isEnd() override
	{
		return true;
	}
};

class AbstractLI
{
public:
	//AbstractLI(){}
	virtual const bool isConnected(const uint u) = 0;
	virtual const IterPtr getIterator() = 0;
	const string getClass()
	{
		return typeid(*this).name();
	}
	virtual ~AbstractLI(){}
};

typedef unique_ptr <AbstractLI> LIPtr;
typedef vector < LIPtr > vectorLI;


class FuncLI : AbstractLI
{
public:
	explicit FuncLI(const int _v) : v(_v) {}
	const bool isConnected(const uint u) override
	{
		return u == v;
	}
	const IterPtr getIterator() override
	{
		return IterPtr((AbstractIter *) new FuncIter(v));
	}
private:
	int v;
};

class StandardLI : AbstractLI
{
public:
	//StandardLI(vector<uint> & _v, bool _sorted) : v(_v), sorted(_sorted) {}
	StandardLI(vector<uint> & _v, bool sorted) : v(_v)
	{
		if (!sorted)
			sort(v.begin(), v.end());
	}
	explicit StandardLI(vector<uint> & _v) : v(_v)
	{
		sort(v.begin(), v.end());
	}
	const bool isConnected(const uint u) override
	{
		return binary_search(v.begin(), v.end(), u);
	}
	const IterPtr getIterator() override
	{
		return IterPtr((AbstractIter *)new StandardIter (v.begin(), v.end()));
	}
private:
	vector <uint> v;
};

class BitsetLI : AbstractLI
{
public:
	explicit BitsetLI(vector <bool> & _bitset) : bitset(_bitset) {}
	const bool isConnected(const uint u) override
	{
		return bitset[u];
	}
	const IterPtr getIterator() override
	{
		return IterPtr((AbstractIter *) new BitsetIter(bitset.begin(), bitset.end()));
	}
private:
	vector <bool> bitset;
};

class EmptyLI : AbstractLI
{
public:
	EmptyLI(){}
	const bool isConnected(const uint u) override
	{
		return false;
	}
	const IterPtr getIterator() override
	{
		return IterPtr((AbstractIter *) new EmptyIter());
	}
};

#endif