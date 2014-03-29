#include "ListOfInc.h"
#include "graph.h"
#include "gtest/gtest.h"
#include "default_fabrics.h"

typedef std::unique_ptr<ListOfIncedents> incedent_ptr_t;
typedef std::unique_ptr<DefaultFabric> fabric_ptr_t;

TEST(StorageTests, BitSetListTest)
{
	const unsigned int number_of_tests = 10;
	const unsigned int max_graph_size = 100;
	for (unsigned int test_case = 0; test_case < number_of_tests; test_case++)
	{
		/* generating graph */
		unsigned int graph_size = rand() % max_graph_size + 1;
		std::vector<std::vector<bool>> matrix(graph_size, std::vector<bool>(graph_size));
		for (unsigned int i = 0; i < graph_size; i++)
			for (unsigned int j = 0; j < graph_size; j++)
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

TEST(StorageTests, DefaultListTest)
{
	const unsigned int number_of_tests = 10;
	const unsigned int max_graph_size = 1000;
	for (int test_case = 0; test_case < number_of_tests; test_case++)
	{
		unsigned int graph_size = rand() % max_graph_size + 1;
		/* generating graph */
		std::vector<std::vector<unsigned int>> edges(graph_size);
		for (unsigned int i = 0; i < graph_size; i++)
			for (unsigned int j = 0; j < rand() % graph_size; j++)
				edges[i].push_back(rand() % graph_size);
		/* building graph */
		auto list_fabric = std::move(fabric_ptr_t(new ListByDefault(edges)));
		Graph graph(std::move(list_fabric), graph_size); 
		/* testing graph */
		for (unsigned int i = 0; i < graph_size; i++)
		{
			auto iterator = graph.getIncedents(i).getIterator();
			for (unsigned int j = 0; j < edges[i].size(); j++)
			{
				EXPECT_EQ(iterator->getCurrentVertex(), edges[i][j]);
				/* iterator->next() must return true after all steps exept last one */
				EXPECT_EQ(iterator->next(), j < edges[i].size() - 1);
			}
			EXPECT_FALSE(iterator->isValid());
		}
		for (unsigned int i = 0; i < graph_size; i++)
		{
			graph.getIncedents(i).sort();
			for (unsigned int j = 0; j < edges[i].size(); j++)
				EXPECT_TRUE(graph.getIncedents(i).isConnected(edges[i][j]));
		}
	}
}

TEST(StorageTests, FunctionalGraph)
{
	const unsigned int number_of_tests = 20;
	const unsigned int max_graph_size = 500;
	for (unsigned int test_case = 0; test_case  < number_of_tests; test_case++)
	{
		unsigned int graph_size = rand() % max_graph_size;
		/* generating graph */
		std::vector<unsigned int> incidents(graph_size);
		for (unsigned int i = 0; i < graph_size; i++)
			incidents[i] = rand() % graph_size;
		/* building graph */
		auto func_fabric = std::move(fabric_ptr_t(new OneVertexByDefault(incidents)));
		Graph graph(std::move(func_fabric), graph_size);
		/* testing graph */
		for (unsigned int i = 0; i < graph_size; i++)
		{
			for (unsigned int j = 0; j < graph_size; j++)
				if (j == incidents[i]) 
					EXPECT_TRUE(graph.getIncedents(i).isConnected(j));
				else
					EXPECT_FALSE(graph.getIncedents(i).isConnected(j));
			auto iterator = graph.getIncedents(i).getIterator();
			EXPECT_EQ(iterator->getCurrentVertex(), incidents[i]);
			iterator->next();
			EXPECT_FALSE(iterator->isValid());
		}
	}
}

TEST(StorageTests, KHeap)
{
	const unsigned int number_of_tests = 20;
	const unsigned int max_graph_size = 500;
	for (unsigned int test_case = 0; test_case < number_of_tests; test_case++)
	{
		/* building graph */
		unsigned int graph_size = rand() % max_graph_size;
		unsigned int k = rand() % graph_size + 2;
		auto fabric_heap = std::move(fabric_ptr_t(new KHeapByDefault(k, graph_size - 1)));
		Graph graph(std::move(fabric_heap), graph_size);
		/* testing graph */
		for (int i = 0; i < graph_size; i++)
		{
			for (int j = 0; j < graph_size; j++)
				if (j >= i * k + 1 && j <= i * k + k)
					EXPECT_TRUE(graph.getIncedents(i).isConnected(j));
				else 
					EXPECT_FALSE(graph.getIncedents(i).isConnected(j));

			auto iterator = graph.getIncedents(i).getIterator();
			for (int j = 1; j <= k; j++)
			{
				if (i * k + j >= graph_size)
					EXPECT_FALSE(iterator->isValid());
				else
				{
					EXPECT_TRUE(iterator->isValid());
					EXPECT_EQ(iterator->getCurrentVertex(), i * k + j);
					if (j < k && i * k + j < graph_size - 1) 
						EXPECT_TRUE(iterator->next());
					else
						EXPECT_FALSE(iterator->next());
				}
			}
			EXPECT_FALSE(iterator->isValid());
		}
	}
}

TEST(StorageTests, FullGraph)
{
	const unsigned int number_of_tests = 10;
	const unsigned int max_graph_size = 100;
	for (unsigned int test_case = 0; test_case < number_of_tests; test_case++)
	{
		/* building graph */
		unsigned int graph_size = rand() % max_graph_size;
		auto fabric_full = std::move(fabric_ptr_t(new FullGraphByDefault(graph_size)));
		Graph graph(std::move(fabric_full), graph_size);
		/* testing graph */
		for (int i = 0; i < graph_size; i++)
		{
			for (int j = 0; j < graph_size; j++)
				EXPECT_TRUE(graph.getIncedents(i).isConnected(j));
			auto iterator = graph.getIncedents(i).getIterator();
			for (int j = 0; j < graph_size; j++)
			{
				EXPECT_EQ(iterator->getCurrentVertex(), j);
				EXPECT_EQ(iterator->next(), j < graph_size - 1);
			}
			EXPECT_FALSE(iterator->isValid());
		}
	}
}

TEST(IntegrationTest, DifferentWaysToStorageInOneGraph)
{
	const unsigned int number_of_tests = 10;
	const unsigned int max_graph_size = 500;
	for (unsigned int test_case = 0; test_case < number_of_tests; test_case++)
	{
		unsigned int graph_size = rand() % max_graph_size + 1;
		/* generating graph */
		std::vector<std::vector<unsigned int>> edges(graph_size);
		for (unsigned int i = 0; i < graph_size; i++)
		{
			std::set<unsigned int> already_used;
			for (unsigned int j = 0; j < rand() % graph_size; j++)
			{
				unsigned int q = rand() % graph_size;
				if (already_used.find(q) == already_used.end())
				{
					edges[i].push_back(q);
					already_used.insert(q);
				}
			}
			std::sort(edges[i].begin(), edges[i].end());
		}
		/* building graph */
		std::vector<incedent_ptr_t> graph_edges(graph_size);
		for (unsigned int i = 0; i < graph_size; i++)
			if (rand() % 2 == 1)
			{
				/* Choose BitSet */
				graph_edges[i] = std::move(incedent_ptr_t(new BitSetList(edges[i], graph_size)));
			}
			else
			{
				/* Choose List */
				graph_edges[i] = std::move(incedent_ptr_t(new StandartList(edges[i], true)));
			}
		Graph graph(graph_edges);
		/* testing graph */
		for (unsigned int i = 0; i < graph_size; i++)
		{
			auto iterator = graph.getIncedents(i).getIterator();
			for (unsigned int j = 0; j < edges[i].size(); j++)
			{
				EXPECT_EQ(iterator->getCurrentVertex(), edges[i][j]);
				EXPECT_EQ(iterator->next(), j < edges[i].size() - 1);
			}
			EXPECT_FALSE(iterator->isValid());			
		}

		for (unsigned int i = 0; i < graph_size; i++)
			for (unsigned int j = 0; j < graph_size; j++)
				EXPECT_EQ(std::binary_search(edges[i].begin(), edges[i].end(), j), 
							graph.getIncedents(i).isConnected(j));
	}
}