#ifndef LIST_OF_INC
#define LIST_OF_INC

#include <memory>
#include <vector>
#include <cassert>
#include <algorithm>

class ListOfIncedentsIterator
{
public:
	virtual bool next() = 0;
	virtual unsigned int getCurrentVertex() const = 0;
	virtual bool isValid() const = 0;
	virtual ~ListOfIncedentsIterator(){};
};

class ListOfIncedents
{
public:
	ListOfIncedents() {};
	virtual bool isConnected(unsigned int vertex) const = 0;
	virtual std::unique_ptr<ListOfIncedentsIterator> getIterator() const = 0;
	virtual ~ListOfIncedents(){};
};

template <typename Iterator>
class StdIterator : public ListOfIncedentsIterator
{
public:
	StdIterator(Iterator begin,
				Iterator end)
				: it_(begin), end_(end) {}
	bool next() override 
	{
		if (it_ == end_) 
			return false;
		if (++it_ != end_)
			return true;
		else
			return false;
	}

	bool isValid() const override
	{
		return it_ != end_;
	}

	unsigned int getCurrentVertex() const override 
	{
		return *it_;
	}
	virtual ~StdIterator(){}
private:
	Iterator it_;
	Iterator end_;
};

class IteratorByIsConnected: public ListOfIncedentsIterator
{
public:
	IteratorByIsConnected(const ListOfIncedents* list,
						  unsigned int lessVertex, 
						  unsigned int maxVertex) : vertexNumber_(lessVertex),
													maxVertex_(maxVertex),
													list_(list) 
	{
		while (vertexNumber_ <= maxVertex_ && !list->isConnected(vertexNumber_))
			vertexNumber_++;
	}
	bool next() override 
	{
		vertexNumber_++;
		while (vertexNumber_ <= maxVertex_ && !list_->isConnected(vertexNumber_))
			vertexNumber_++;
		if (vertexNumber_ > maxVertex_)
			return false;
		else 
			return true;
	}
	bool isValid() const override
	{
		return vertexNumber_ <= maxVertex_;
	}
	unsigned int getCurrentVertex() const override
	{
		return vertexNumber_;
	}
	virtual ~IteratorByIsConnected(){}
private:
	unsigned int vertexNumber_;
	unsigned int maxVertex_;
	const ListOfIncedents* list_;
};

class BitSetList : public ListOfIncedents
{
public:
	explicit BitSetList(const std::vector<unsigned int>& data)
	{
		for (unsigned int i = 0; i < data.size(); i++)
		{
			if (data[i] >= incidents_.size())
				incidents_.resize(data[i]);
			incidents_[data[i]] = true;
		}
	}

	explicit BitSetList(const std::vector<bool>& data)
		: incidents_(data) {}

	bool isConnected(unsigned int vertex) const	override
	{
		assert(vertex >= 0 && vertex <= incidents_.size());
		return incidents_[vertex];
	}

	std::unique_ptr<ListOfIncedentsIterator> getIterator() const override
	{
		return std::move(
						  std::unique_ptr<ListOfIncedentsIterator>
						  (new IteratorByIsConnected(this, 0, incidents_.size() - 1))
						);
	}

	virtual ~BitSetList() {}
private:
	std::vector<bool> incidents_;
};

class StandartList : public ListOfIncedents
{
public:
	StandartList(const std::vector<unsigned int>& incidents, bool to_sort = false)
		:incidents_(incidents), sorted_(to_sort)
	{
		if (to_sort)
			std::sort(incidents_.begin(), incidents_.end());
	}
	void sort()
	{
		if (!sorted_)
			std::sort(incidents_.begin(), incidents_.end());
		sorted_ = 1;
	}
	bool isConnected(unsigned int vertex) const override
	{
		assert(sorted_);
		return std::binary_search(incidents_.begin(), incidents_.end(), vertex);
	}
	std::unique_ptr<ListOfIncedentsIterator> getIterator() const override
	{
		return std::move(
						 std::unique_ptr<ListOfIncedentsIterator>
						 (new StdIterator<std::vector<unsigned int>::const_iterator>(incidents_.begin(), incidents_.end()))
						);
	}
	virtual ~StandartList(){}
private:
	std::vector<unsigned int> incidents_;
	bool sorted_;
};

class OneVertexList : public ListOfIncedents
{
public:
	OneVertexList(unsigned int vertex):
	  vertex_(vertex) {}
	bool isConnected(unsigned int vertex) const override
	{
		return vertex_ == vertex;
	}
	std::unique_ptr<ListOfIncedentsIterator> getIterator() const override
	{
		return std::move(
						  std::unique_ptr<ListOfIncedentsIterator>
						  (new IteratorByIsConnected(this, vertex_, vertex_))
						);
	}
	virtual ~OneVertexList(){}
private:
	unsigned int vertex_;
};

class KHeapList : public ListOfIncedents
{
public:
	KHeapList(unsigned int first, unsigned int last):
	  first_(first), last_(last) {}
	bool isConnected(unsigned int vertex) const override
	{
		return vertex >= first_ && vertex <= last_;
	}
	std::unique_ptr<ListOfIncedentsIterator> getIterator() const override
	{
		return std::move(
							std::unique_ptr<ListOfIncedentsIterator>
							(new IteratorByIsConnected(this, first_, last_))
						);
	}
	virtual ~KHeapList(){}
private:
	unsigned int first_, last_;
};

class EmptyList : public ListOfIncedents
{
public:
	EmptyList() {}
	bool isConnected(unsigned int vertex) const override
	{
		return false;
	}
	std::unique_ptr<ListOfIncedentsIterator> getIterator() const override
	{
		return std::move(
							std::unique_ptr<ListOfIncedentsIterator>
							(new IteratorByIsConnected(this, 1, 0))
						);
	}
};

#endif