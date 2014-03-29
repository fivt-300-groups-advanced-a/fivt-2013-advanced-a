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
	ListByDefault(const std::vector<std::vector<unsigned int>> &edges, bool to_sort = false)
		: edges_(edges), to_sort_(to_sort) {}

	std::unique_ptr<ListOfIncedents> getList(unsigned int i) const override
	{
		return std::move(
							std::unique_ptr<ListOfIncedents>
							(new StandartList(edges_[i], to_sort_))
						);
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
	KHeapByDefault(unsigned int k, unsigned int n): k_(k), n_(k) {}
	std::unique_ptr<ListOfIncedents> getList(unsigned int i) const override
	{
		if (i * k_ > n_)
			return std::move(std::unique_ptr<ListOfIncedents>(new EmptyList()));
		else
			return std::move(
							   std::unique_ptr<ListOfIncedents>
							   (new KHeapList(i * k_, std::min(n_, i * k_ + k_ - 1)))
							 );
	}
private:
	unsigned int k_, n_;
};

class FullGraphByDefault: public DefaultFabric
{
public:
	FullGraphByDefault(unsigned int n): n_(n) {}
	std::unique_ptr<ListOfIncedents> getList(unsigned int i) const override
	{
		return std::move( 
							std::unique_ptr<ListOfIncedents>
							(new KHeapList(0, n_))
						);
	}
private:
	unsigned int n_;
};

#endif