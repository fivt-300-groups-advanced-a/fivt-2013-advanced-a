#include <list>
#include <memory>

template <class T>

class BinomTree {
public:
	BinomTree(T val)
	{
		_val = val;
		_parent = NULL;
		_ch.clear();
	}
	BinomTree()
	{
		_parent = NULL;
		_ch.clear();
	}
	BinomTree(std::unique_ptr<BinomTree> &tr1,
		      std::unique_ptr<BinomTree> &tr2)
	{
		try
		{
			if (tr1->deg() != tr2->deg())
				throw "trying to merge non-equal trees";
			if ((tr1->_parent != nullptr) || (tr2->_parent != nullptr))
				throw "trying to merge trees inside another tree";
		}
		catch (const char *report)
		{
			std::cerr << report << std::endl;
		}
		tr2->_parent = *
		_ch.push_back(std::move(pOtherTree));
	}
	void set_value(T val)
	{
		_val = val;
	}
	const T& get_value() const
	{
		return _val;
	}
	int deg() const
	{
		return _ch.size();
	}
private:
	T _val;
	std::list<std::unique_ptr<BinomTree<T> > > _ch;
	BinomTree<T>* _parent;
};