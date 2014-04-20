#include "Graph.h"

class AbstractFactory
{
public:
	virtual bool addEdge(uint u) = 0;
	virtual const LIPtr getLI() = 0;
	virtual ~AbstractFactory(){};
};

class StandardFactory : AbstractFactory
{
public:
	StandardFactory(uint _n) : flagBitset(false), n(_n) {}
	bool addEdge(uint u) override //returns false if u is invalid
	{
		if (u >= n)
			return false;
		if (flagBitset)
		{
			bitset[u] = 1;
			return true;
		}
		v.push_back(u);
		if (v.size() > 1 && (int)v.size() > n / 32)
		{
			flagBitset = true;
			bitset.assign(n, false);
			for (uint j = 0; j < v.size(); j++)
				bitset[v[j]] = true;
			v.clear();
		}
		return true;
	}
	const LIPtr getLI() override
	{
		if (v.empty() && !flagBitset)
			return LIPtr((AbstractLI *) new EmptyLI());
		if (v.size() == 1)
			return LIPtr((AbstractLI *) new FuncLI(v[0]));
		if (flagBitset)
			return LIPtr((AbstractLI *) new BitsetLI(bitset));
		else return LIPtr((AbstractLI *) new StandardLI(v));
	}
private:
	uint n;
	bool flagBitset;
	vector <uint> v;
	vector <bool> bitset;
};