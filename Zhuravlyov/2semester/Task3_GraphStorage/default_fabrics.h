#ifndef DEFAULT_FABRIC
#define DEFAULT_FABRIC

#include <vector>
#include "ListOfInc.h"

class DefaultFabric
{
public:
	virtual std::unique_ptr<ListOfIncedents> getList(unsigned int) const = 0;
	virtual ~DefaultFabric() {};
};

class ListByDefault: public DefaultFabric
{
public:
	ListByDefault(const std::vector<std::vector<unsigned int>> &edges)
		: edges_(edges) {}

	std::unique_ptr<ListOfIncedents> getList(unsigned int i) const override
	{
		return std::move(std::unique_ptr<ListOfIncedents>(new StandartList(edges_[i])));
	}
	virtual ~ListByDefault() {}
private:
	const std::vector<std::vector<unsigned int>>& edges_;
	bool to_sort_;
};

class BitSetByDefault: public DefaultFabric
{
public:
	BitSetByDefault(const std::vector<std::vector<bool>> &matrix)
		: matrix_(matrix) {}

	std::unique_ptr<ListOfIncedents> getList(unsigned int i) const override
	{
		return std:: move(std::unique_ptr<ListOfIncedents>(new BitSetList(matrix_[i])));
	}
	virtual ~BitSetByDefault() {}
private:
	const std::vector<std::vector<bool>>& matrix_;
};

class OneVertexByDefault: public DefaultFabric
{
public:
	OneVertexByDefault(const std::vector<unsigned int>& incident_vertices)
		:incident_vertices_(incident_vertices) {}

	std::unique_ptr<ListOfIncedents> getList(unsigned int i) const override
	{
		return std::move(std::unique_ptr<ListOfIncedents>(new OneVertexList(incident_vertices_[i])));
	}
	virtual ~OneVertexByDefault() {}
private:
	const std::vector<unsigned int>& incident_vertices_;
};

class KHeapByDefault: public DefaultFabric
{
public:
	KHeapByDefault(unsigned int k, unsigned int max_vertex): k_(k), max_vertex_(max_vertex) {}
	std::unique_ptr<ListOfIncedents> getList(unsigned int i) const override
	{
		if (i * k_ > max_vertex_)
			return std::move(std::unique_ptr<ListOfIncedents>(new EmptyList()));
		else
			return std::move(std::unique_ptr<ListOfIncedents>(new KHeapList(i * k_ + 1, std::min(max_vertex_, i * k_ + k_))));
	}
private:
	/* k is heap-parametr */
	unsigned int k_, max_vertex_;
};

class FullGraphByDefault: public DefaultFabric
{
public:
	FullGraphByDefault(unsigned int number_of_vertices)
		: number_of_vertices_(number_of_vertices) {}
	std::unique_ptr<ListOfIncedents> getList(unsigned int i) const override
	{
		return std::move(std::unique_ptr<ListOfIncedents>(new KHeapList(0, number_of_vertices_ - 1)));
	}
private:
	unsigned int number_of_vertices_;
};

#endif