#ifndef GRAPH
#define GRAPH

#include "ListOfInc.h"
#include "default_fabrics.h"

class Graph
{
public:
	Graph(std::vector< std::unique_ptr<ListOfIncedents> > &&edges) 
	{
		edges_ = std::move(edges);
	}

	Graph(std::vector< std::unique_ptr<ListOfIncedents> > &edges,
		  std::vector< std::unique_ptr<ListOfIncedents> > &back_edges)
	{
		edges_ = std::move(edges);
		back_edges_ = std::move(back_edges);
	}

	Graph(std::unique_ptr<DefaultFabric> fabric, unsigned int number_of_vertices)
	{
		edges_.resize(number_of_vertices);
		for (unsigned int i = 0; i < number_of_vertices; i++)
			edges_[i] = std::move( fabric->getList(i) );
	}

	Graph(std::unique_ptr<DefaultFabric> fabric, std::unique_ptr<DefaultFabric> back_edges_fabric, unsigned int number_of_vertices)
	{
		edges_.resize(number_of_vertices);
		back_edges_.resize(number_of_vertices);
		for (unsigned int i = 0; i < number_of_vertices; i++)
		{
			edges_[i] = std::move( fabric->getList(i) );
			back_edges_[i] = std::move( back_edges_fabric->getList(i) );
		}
	}

	const ListOfIncedents& getIncedents(unsigned int i) const
	{
		return *edges_[i];
	}

	const ListOfIncedents& getBackEdges(unsigned int i) const
	{
		return *back_edges_[i];
	}

	unsigned int getsize() const
	{
		return edges_.size();
	}
private:
	std::vector< std::unique_ptr<ListOfIncedents> > edges_;
	std::vector< std::unique_ptr<ListOfIncedents> > back_edges_;
};

#endif