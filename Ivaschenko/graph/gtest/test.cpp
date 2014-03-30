#include <iostream>

#include <gtest/gtest.h>

#include "lists/setincidencelist.h"
#include "lists/bitsetincidencelist.h"
#include "lists/vectorincidencelist.h"
#include "lists/singlevertexlist.h"
#include "lists/emptyincidencelist.h"

TEST(Graph, ManualTest)
{
	graph::EmptyIncidenceList lst;
	for (auto it = lst.getIterator(); it->valid(); it = it->next())
		std::cout << it->getVertex() << std::endl;
}
