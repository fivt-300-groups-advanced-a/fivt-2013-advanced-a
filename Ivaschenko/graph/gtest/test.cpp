#include <iostream>

#include <gtest/gtest.h>

#include "lists/setincidencelist.h"
#include "lists/bitsetincidencelist.h"
#include "lists/vectorincidencelist.h"
#include "lists/singlevertexlist.h"
#include "lists/emptyincidencelist.h"

#include "fabrics/vectorlistgraphfabric.h"
#include "fabrics/bitsetgraphfabric.h"

#include "graph.h"

TEST(Graph, ManualTest)
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
