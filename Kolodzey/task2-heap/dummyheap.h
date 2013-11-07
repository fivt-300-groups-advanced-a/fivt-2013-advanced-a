#include <iterator>
#include <functional>
#include <vector>
#include <memory>
#include <iostream>

template <class T,
		  class Compare = std::less<T> >

class DummyHeap
{
public:
	DummyHeap()
	{
		_pv.clear();
		_cmp = std::less<T>();
	}
	
	DummyHeap(Compare cmp)
	{
		_pv.clear();
		_cmp = cmp;
	}

	DummyHeap(DummyHeap &h1, DummyHeap &h2)
	{
		_pv.clear();
		_pv.insert(_pv.begin(), h1.begin(), h1.end());
		h1.clear();
		_pv.insert(_pv.begin(), h2.begin(), h2.end());
		h2.clear();
		std::sort(_pv.begin(), _pv.end(), h1._cmp);
		_cmp = h1._cmp;
	}

	//Итераторы
	typedef typename std::vector<T>::iterator iterator;

	iterator begin()
	{
		return _pv.begin();
	}

	iterator end()
	{
		return _pv.end();
	}

	//Размер
	bool empty()
	{
		return _pv.empty();
	}

	size_t size() const
	{
		return _pv.size();
	}

	//Изменения
	void clear()
	{
		_pv.clear();
	}

	void erase(iterator position)
	{
		_pv.erase(position);
	}

	void insert(T val)
	{
		_pv.push_back(val);
		std::sort(_pv.begin(), _pv.end(), _cmp);
	}

	void absorb(DummyHeap &child_heap)
	{
		_pv.insert(_pv.begin(), child_heap.begin(), child_heap.end());
		child_heap.clear();
		std::sort(_pv.begin(), _pv.end(), _cmp);
	}

	//calling top() on empty container causes undefined behavior
	const T& top() const
	{
		return _pv.front();
	}

	void pop_top()
	{
		try
		{
			if (_pv.size() == 0)
				throw "nothing to pop_top";
		}
		catch (const char *report)
		{
			std::cerr << report << std::endl;
		}
		_pv.erase(_pv.begin());
	}

private:
    std::vector <T> _pv;
    Compare _cmp;
};
