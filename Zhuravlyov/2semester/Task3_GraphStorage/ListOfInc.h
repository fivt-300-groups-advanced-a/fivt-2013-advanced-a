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
	virtual ~ListOfIncedentsIterator(){};
};

class ListOfIncedents
{
public:
	virtual bool isConnected(unsigned int vertex) const = 0;
	virtual std::unique_ptr<ListOfIncedentsIterator> getIterator() const = 0;
	virtual ~ListOfIncedents(){};
};

class IteratorByIsConnected: public ListOfIncedentsIterator
{
public:
	IteratorByIsConnected(const ListOfIncedents* list,
						  unsigned int lessVertex, 
						  unsigned int maxVertex) : vertexNumber_(lessVertex),
													maxVertex_(maxVertex),
													list_(list) {}
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
						  (new IteratorByIsConnected(this, 0, incidents_.size()))
						);
	}
	virtual ~BitSetList() {}
private:
	std::vector<bool> incidents_;
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

	unsigned int getCurrentVertex() const override 
	{
		return *it_;
	}
	virtual ~StdContainerIterator(){}
private:
	Iterator it_;
	Iterator end_;
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
	;
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

#endif