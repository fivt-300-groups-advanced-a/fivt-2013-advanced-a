#ifndef CLEVER_FABRIC
#define CLEVER_FABRIC

#include <vector>
#include <memory>
#include <cassert>
#include "ListOfInc.h"

class RebuildingFactory
{
public:
	explicit RebuildingFactory(const unsigned int total_number_of_verteces)
		: total_number_of_verteces_(total_number_of_verteces), type_('e') {}

	void addEdge(unsigned int incident_vertex)
	{
		assert(incident_vertex < total_number_of_verteces_);
		if (type_ == 'e')
		{
			vertex_ = incident_vertex;
			type_ = 'u';
		}
		else if (type_ == 'u')
		{
			list_.push_back(vertex_);
			list_.push_back(incident_vertex);
			type_ = 'l';
		}
		else if (type_ == 'l')
		{
			list_.push_back(incident_vertex);
			if (list_.size() * 32 > total_number_of_verteces_)
			{
				bitset_.resize(total_number_of_verteces_);
				for (unsigned int i = 0; i < list_.size(); i++)
					bitset_[list_[i]] = true;
				list_.clear();
				type_ = 'b';
			}
		}
		else if (type_ == 'b')
			bitset_[incident_vertex] = true;
	}

	std::unique_ptr<ListOfIncedents> getList()
	{
		if (type_ == 'e')
			return std::move(std::unique_ptr<ListOfIncedents>(new EmptyList()));
		else if (type_ == 'u')
		{
			type_ = 'e';
			return std::move(std::unique_ptr<ListOfIncedents>(new OneVertexList(vertex_)));
		}
		else if (type_ == 'l')
		{
			type_ = 'e';
			auto result = std::unique_ptr<ListOfIncedents>(new StandartList(list_));
			list_.clear();
			return std::move(result);
		}
		else
		{
			type_ = 'e';
			auto result = std::unique_ptr<ListOfIncedents>(new BitSetList(bitset_));
			bitset_.clear();
			return std::move(result);
		}
	}
private:
	std::vector<unsigned int> list_;
	std::vector<bool> bitset_;
	unsigned int vertex_;
	char type_; // 'u' - unique, 'e' - empty, 'l' - list, 'b' - bitset
	unsigned int total_number_of_verteces_;
};

#endif