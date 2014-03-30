#include <iostream>

#include <gtest/gtest.h>

#include "lists/setincidencelist.h"
#include "lists/bitsetincidencelist.h"
#include "lists/vectorincidencelist.h"
#include "lists/singlevertexlist.h"
#include "lists/emptyincidencelist.h"

#include "fabrics/vectorlistgraphfabric.h"
#include "fabrics/bitsetgraphfabric.h"
#include "fabrics/functionalgraphfabric.h"

#include "graph.h"

TEST(Graph, ConstuctionFromBitsetFabric)
{
	std::unique_ptr<graph::AbstractGraphFabric> fabric(new graph::BitsetGraphFabric({{1, 0}, {1}}, true));
	graph::Graph g(fabric, 2, true);
	std::cout << "From 0:" << std::endl;
	for (auto it = g.getEdgesFrom(1)->getIterator(); it->valid(); it->moveForward())
		std::cout << it->getVertex() << " ";
	std::cout << std::endl;
	std::cout << "To 0:" << std::endl;
	for (auto it = g.getEdgesTo(1)->getIterator(); it->valid(); it->moveForward())
		std::cout << it->getVertex() << " ";
	std::cout << std::endl;
}

TEST(Graph, ConstuctionFromFunctionalFabric)
{
	std::unique_ptr<graph::AbstractGraphFabric> fabric(new graph::FunctionalGraphFabric({1, 2, 3, 0}, true));
	graph::Graph g(fabric, 4, true);
	std::cout << "Direct edges:" << std::endl;
	for (std::size_t i = 0; i < 4; ++i)
		for (auto it = g.getEdgesFrom(i)->getIterator(); it->valid(); it->moveForward())
			std::cout << "(" << i << "; " << it->getVertex() << ")" << std::endl;
	std::cout << "Back edges:" << std::endl;
	for (std::size_t i = 0; i < 4; ++i)
		for (auto it = g.getEdgesTo(i)->getIterator(); it->valid(); it->moveForward())
			std::cout << "(" << i << "; " << it->getVertex() << ")" << std::endl;
}
