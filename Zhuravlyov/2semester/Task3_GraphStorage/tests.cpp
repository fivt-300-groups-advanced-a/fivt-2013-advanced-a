#include "ListOfInc.h"
#include "graph.h"
#include "gtest/gtest.h"
#include "default_fabrics.h"
#include "clever_fabric.h"
#include "problem.h"

typedef std::unique_ptr<ListOfIncedents> incedent_ptr_t;
typedef std::unique_ptr<DefaultFabric> fabric_ptr_t;

// Unit tests for different list of incedents

TEST(StorageTests, BitSetListTest)
{
	/* For testing memory leak checker is working */
//	ListOfIncedents* leak = new OneVertexList(0); 
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
	const unsigned int max_graph_size = 100;
	for (int test_case = 0; test_case < number_of_tests; test_case++)
	{
		unsigned int graph_size = rand() % max_graph_size + 1;
		/* generating graph */
		std::vector<std::vector<unsigned int>> edges(graph_size);
		for (unsigned int i = 0; i < graph_size; i++) 
		{
			for (unsigned int j = 0; j < rand() % graph_size; j++)
				edges[i].push_back(rand() % graph_size);
			std::sort(edges[i].begin(), edges[i].end());
		}
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
			for (unsigned int j = 0; j < edges[i].size(); j++)
				EXPECT_TRUE(graph.getIncedents(i).isConnected(edges[i][j]));
		}
	}
}

TEST(StorageTests, FunctionalGraph)
{
	const unsigned int number_of_tests = 20;
	const unsigned int max_graph_size = 100;
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
	const unsigned int max_graph_size = 100;
	for (unsigned int test_case = 0; test_case < number_of_tests; test_case++)
	{
		/* building graph */
		unsigned int graph_size = rand() % max_graph_size;
		unsigned int k = rand() % graph_size + 2;
		auto fabric_heap = std::move(fabric_ptr_t(new KHeapByDefault(k, graph_size - 1)));
		Graph graph(std::move(fabric_heap), graph_size);
		/* testing graph */
		for (unsigned int i = 0; i < graph_size; i++)
		{
			for (unsigned int j = 0; j < graph_size; j++)
				if (j >= i * k + 1 && j <= i * k + k)
					EXPECT_TRUE(graph.getIncedents(i).isConnected(j));
				else 
					EXPECT_FALSE(graph.getIncedents(i).isConnected(j));

			auto iterator = graph.getIncedents(i).getIterator();
			for (unsigned int j = 1; j <= k; j++)
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
		for (unsigned int i = 0; i < graph_size; i++)
		{
			for (unsigned int j = 0; j < graph_size; j++)
				EXPECT_TRUE(graph.getIncedents(i).isConnected(j));
			auto iterator = graph.getIncedents(i).getIterator();
			for (unsigned int j = 0; j < graph_size; j++)
			{
				EXPECT_EQ(iterator->getCurrentVertex(), j);
				EXPECT_EQ(iterator->next(), j < graph_size - 1);
			}
			EXPECT_FALSE(iterator->isValid());
		}
	}
}

TEST(IntegrationTests, DifferentWaysToStorageInOneGraph)
{
	const unsigned int number_of_tests = 10;
	const unsigned int max_graph_size = 100;
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
				graph_edges[i] = std::move(incedent_ptr_t(new StandartList(edges[i])));
			}
		Graph graph(std::move(graph_edges));
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

TEST(FactoryTests, RebuildingFactoryTest)
{
	RebuildingFactory fact(100);
	EXPECT_EQ(fact.getList()->getTypeName(), "Empty");

	fact.addEdge(1);
	auto ptr = fact.getList();
	EXPECT_EQ(ptr->getTypeName(), "OneVertex");
	EXPECT_TRUE(ptr->isConnected(1));
	EXPECT_EQ(fact.getList()->getTypeName(), "Empty");

	fact.addEdge(1);
	fact.addEdge(2);
	fact.addEdge(3);
	ptr = fact.getList();
	EXPECT_EQ(ptr->getTypeName(), "List");
	for (int i = 0; i < 100; i++)
		EXPECT_EQ(ptr->isConnected(i), i > 0 && i < 4);
	EXPECT_EQ(fact.getList()->getTypeName(), "Empty");

	for (int i = 0; i < 50; i++)
		fact.addEdge(i);
	ptr = fact.getList();
	EXPECT_EQ(ptr->getTypeName(), "BitSet");
	for (int i = 0; i < 100; i++)
		EXPECT_EQ(ptr->isConnected(i), i < 50);
	EXPECT_EQ(fact.getList()->getTypeName(), "Empty");

}

TEST(IntegrationTests, GraphWithCleverFactoryTest)
{
	const int number_of_tests = 10;
	const int max_graph_size = 100;
	for (int test_case = 0; test_case < number_of_tests; test_case++)
	{
		unsigned int graph_size = rand() % max_graph_size;
		std::vector<std::vector<bool>> matrix(graph_size, std::vector<bool>(graph_size));
		for (unsigned int i = 0; i < graph_size; i++)
			for (unsigned int j = 0; j < graph_size; j++)
				if (rand() % 100 < 10)
					matrix[i][j] = 1;
				else
					matrix[i][j] = 0;
		RebuildingFactory factory(graph_size);
		std::vector<std::unique_ptr<ListOfIncedents>> incidents(graph_size);
		for (unsigned int i = 0; i < graph_size; i++)
		{
			for (unsigned int j = 0; j  < graph_size; j++)
				if (matrix[i][j])
					factory.addEdge(j);
			incidents[i] = factory.getList();
		}
		Graph graph(std::move(incidents));
		for (unsigned int i = 0; i < graph_size; i++) {
			for (unsigned int j = 0; j < graph_size; j++)
				EXPECT_EQ(matrix[i][j], graph.getIncedents(i).isConnected(j));
		}
		for (unsigned int i = 0; i < graph_size; i++)
		{
			unsigned int j = 0;
			auto iterator = graph.getIncedents(i).getIterator();
			while (j < graph_size)
			{
				while (j < graph_size && !matrix[i][j])
					j++;
				if (j < graph_size)
				{
					EXPECT_EQ(j, iterator->getCurrentVertex());
					iterator->next();
					j++;
				}
			}
			EXPECT_FALSE(iterator->isValid());
		}
	}
}

TEST(ManualTests, StrongConnectivityAugmentationTest1)
{
	std::vector<std::vector<unsigned int>> edges(4);
	edges[0].push_back(1);
	edges[1].push_back(0);
	edges[2].push_back(3);
	edges[3].push_back(2);
	auto list_fabric = std::move(fabric_ptr_t(new ListByDefault(edges)));
	Graph graph(std::move(list_fabric), 4);
	std::vector<std::pair<unsigned int, unsigned int>> answer;
	StrongConnectivityAugmentation(graph, answer);
	EXPECT_EQ(answer.size(), 2);
	edges[answer[0].first].push_back(answer[0].second);
	edges[answer[1].first].push_back(answer[1].second);
	answer.clear();
	auto new_list_fabric = std::move(fabric_ptr_t(new ListByDefault(edges)));
	Graph new_graph(std::move(new_list_fabric), 4);
	StrongConnectivityAugmentation(new_graph, answer);
	EXPECT_EQ(answer.size(), 0);
}

TEST(ManualTests, StrongConnectivityAugmentationTest2)
{
	std::vector<std::vector<unsigned int>> edges(5);
	for (int i = 0; i < 4; i++)
		edges[i].push_back(i + 1);
	auto func_fabric = std::move(fabric_ptr_t(new ListByDefault(edges)));
	Graph graph(std::move(func_fabric), 5);
	std::vector<std::pair<unsigned int, unsigned int>> answer;
	StrongConnectivityAugmentation(graph, answer);
	EXPECT_EQ(answer.size(), 1);
	EXPECT_EQ(answer[0], std::make_pair((unsigned int)4, (unsigned int)0));
}


TEST(ManualTests, StrongConnectivityAugmentationTest3)
{
	std::vector<std::vector<unsigned int>> edges(5);
	edges[0].push_back(4);
	edges[0].push_back(1);
	edges[3].push_back(1);
	auto func_fabric = std::move(fabric_ptr_t(new ListByDefault(edges)));
	Graph graph(std::move(func_fabric), 5);
	std::vector<std::pair<unsigned int, unsigned int>> answer;
	StrongConnectivityAugmentation(graph, answer);
	EXPECT_EQ(answer.size(), 3);

	for (unsigned int i = 0; i < answer.size(); i++)
		edges[answer[i].first].push_back(answer[i].second);
	auto new_list_fabric = std::move(fabric_ptr_t(new ListByDefault(edges)));
	Graph new_graph(std::move(new_list_fabric), 5);

	std::vector<unsigned int> colors;
	unsigned int maxcolor;
	GetStronglyConnectedComponents(new_graph, colors, maxcolor);
	EXPECT_EQ(maxcolor, 2);
}

TEST(ManualTests, StrongConnectivityAugmentationTest4)
{
	std::vector<std::vector<unsigned int>> edges(8);
	edges[1].push_back(0);
	edges[0].push_back(4);
	edges[2].push_back(4);
	edges[1].push_back(3);
	edges[3].push_back(5);
	edges[5].push_back(3);
	auto func_fabric = std::move(fabric_ptr_t(new ListByDefault(edges)));
	Graph graph(std::move(func_fabric), 8);
	std::vector<std::pair<unsigned int, unsigned int>> answer;
	StrongConnectivityAugmentation(graph, answer);
	EXPECT_EQ(answer.size(), 4);

	for (unsigned int i = 0; i < answer.size(); i++)
		edges[answer[i].first].push_back(answer[i].second);
	auto new_list_fabric = std::move(fabric_ptr_t(new ListByDefault(edges)));
	Graph new_graph(std::move(new_list_fabric), 8);

	std::vector<unsigned int> colors;
	unsigned int maxcolor;
	GetStronglyConnectedComponents(new_graph, colors, maxcolor);
	EXPECT_EQ(maxcolor, 2);
}

TEST(UnitTest, GettingStronglyConnectedComponents)
{
	const int graph_size = 1000;
	const int size_of_component = 10;
	std::vector<std::vector<unsigned int>> edges(graph_size);
	for (int i = 0; i < graph_size; i++)
	{
		if ((i + 1) % size_of_component == 0)
			edges[i].push_back(i + 1 - size_of_component);
		else
			edges[i].push_back(i + 1);
	}
	for (int i = 1; i < graph_size; i++)
		if (i % size_of_component == 1 && i + size_of_component < graph_size)
			edges[i].push_back(i + size_of_component);
	auto list_fabric = std::move(fabric_ptr_t(new ListByDefault(edges)));
	Graph graph(std::move(list_fabric), graph_size); 
	std::vector<unsigned int> colors;
	unsigned int lastcolor;
	GetStronglyConnectedComponents(graph, colors, lastcolor);
	EXPECT_EQ(lastcolor, graph_size / size_of_component + 1);
	for (int i = 0; i < graph_size; i++)
		for (int j = i; j < graph_size; j++)
			EXPECT_EQ(i / size_of_component == j / size_of_component, colors[i] == colors[j]);
}

TEST(IntegrationTests, StrongConnectivityAugmentationStressTest)
{
	const int number_of_tests = 5;
	const int number_of_verteces = 1000;
	const int number_of_edges = 1000;
	for (int test_case = 0; test_case < number_of_tests; test_case++)
	{
		std::vector<std::vector<unsigned int>> edges(number_of_verteces, std::vector<unsigned int>());
		for (int i = 0; i < number_of_edges; i++)
		{
			unsigned int left = rand() % number_of_verteces;
			unsigned int right = rand() % number_of_verteces;
			edges[left].push_back(right);
		}
		auto list_fabric = std::move(fabric_ptr_t(new ListByDefault(edges)));
		Graph graph(std::move(list_fabric), number_of_verteces);
		std::vector<std::pair<unsigned int, unsigned int>> answer;
		StrongConnectivityAugmentation(graph, answer);
		// Check if answer is correct

		for (unsigned int i = 0; i < answer.size(); i++)
			edges[answer[i].first].push_back(answer[i].second);
		auto new_list_fabric = std::move(fabric_ptr_t(new ListByDefault(edges)));
		Graph new_graph(std::move(new_list_fabric), number_of_verteces);

		std::vector<unsigned int> colors;
		unsigned int maxcolor;
		GetStronglyConnectedComponents(new_graph, colors, maxcolor);
		EXPECT_EQ(maxcolor, 2);
		//Check if answer is optimal
		colors.clear();
		GetStronglyConnectedComponents(graph, colors, maxcolor);
			std::vector<char> has_input(maxcolor, 0), has_output(maxcolor, 0);
		for (unsigned int i = 0; i < graph.getsize(); i++)
		{
			auto it = graph.getIncedents(i).getIterator();
			while (it->isValid())
			{
				unsigned int incedent_vertex = it->getCurrentVertex();
				if (colors[i] != colors[incedent_vertex])
					has_output[colors[i]] = has_input[colors[incedent_vertex]] = 1;
				it->next();
			}
		}
		int sources = 0, sinks = 0;
		for (unsigned int i = 1; i < maxcolor; i++)
		{
			if (!has_input[i])
				sources++;
			if (!has_output[i])
				sinks++;
		}
		if (maxcolor == 2)
			EXPECT_EQ(answer.size(), 0);
		else
			EXPECT_EQ(answer.size(), std::max(sources, sinks));
	}
}