#include "ListOfInc.h"
#include "graph.h"
#include "gtest/gtest.h"
#include "default_fabrics.h"

typedef std::unique_ptr<ListOfIncedents> incident_ptr_t;
typedef std::unique_ptr<DefaultFabric> fabric_ptr_t;

TEST(StorageTests, BitSetListTest)
{
	const unsigned int number_of_tests = 10;
	const unsigned int graph_size = 100;
	for (unsigned int test_case = 0; test_case < number_of_tests; test_case++)
	{
		/* generating graph */
		std::vector<std::vector<bool>> matrix(graph_size, std::vector<bool>(graph_size));
		for (int i = 0; i < graph_size; i++)
			for (int j = 0; j < graph_size; j++)
				matrix[i][j] = rand() % 2;
		/* building graph */
		fabric_ptr_t bitset_fabric = std::move(fabric_ptr_t(new BitSetByDefault(matrix)));
		Graph graph(std::move(bitset_fabric), graph_size);
		/* testing graph */
		for (unsigned int i = 0; i < graph_size; i++)
			for (unsigned int j = 0; j < graph_size; j++)
				EXPECT_TRUE(graph.getIncedents(i).isConnected(j) == matrix[i][j]);
		for (unsigned int i = 0; i < graph_size; i++)
		{
			unsigned int j = 0;
			auto iterator = graph.getIncedents(i).getIterator();
			while (j < graph_size)
			{
				while (j < graph_size && !matrix[i][j])
					j++;
				EXPECT_EQ(j, iterator->getCurrentVertex());
				iterator->next();
				j++;
			}
			EXPECT_FALSE(iterator->isValid());
		}
	}
}