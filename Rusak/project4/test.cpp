#include "gtest/gtest.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "graph.h"
#include "strong_connectivity_augmentation.h"

typedef std::unique_ptr<ListOfIncidents> list_ptr;
typedef std::unique_ptr<ListIterator> iterator_ptr;
typedef std::unique_ptr<ListFactory> factory_ptr;

std::vector<factory_ptr>
build_factories(std::vector<std::vector<unsigned int> > &graph) {
  int size = graph.size();
  std::vector<factory_ptr> factory;
  for (int i=0;i<size;i++) factory.push_back(std::unique_ptr<ListFactory>(new CleverFactory(size)));
  for (int i=0;i<size;i++) 
    for (int j=0;j<graph[i].size();j++) {
      factory[i]->add_vertex(graph[i][j]);
    }
  return std::move(factory);
}

std::vector<factory_ptr>
build_factories(std::vector<std::vector<bool> > &graph) {
  int size = graph.size();
  std::vector<factory_ptr> factory;
  for (int i=0;i<size;i++) factory.push_back(std::unique_ptr<ListFactory>(new CleverFactory(size)));
  for (int i=0;i<size;i++) 
    for (int j=0;j<graph[i].size();j++)
      if (graph[i][j]) factory[i]->add_vertex(j);
  return std::move(factory);
}

std::pair< std::vector<unsigned int>, std::vector<bool> >
build_list(int size) {
  std::vector<bool> q_bool;
  std::vector<unsigned int> q, tmp;
  int col = size+rand()%size;
  for (int i=0;i<col;i++) {
    int ma = rand()%col;
    tmp.push_back(ma);
  }
  int max_val = *std::max_element(tmp.begin(), tmp.end());
  q_bool.resize(max_val+1);
  for (int i=0;i<col;i++) q_bool[tmp[i]] = true;
  for (int i=0;i<q_bool.size();i++) if (q_bool[i]==true) q.push_back(i);
  std::random_shuffle(q.begin(), q.end());
  return make_pair(q, q_bool);
}

void print_list(ListOfIncidents &cur) {
  auto it = cur.get_iterator();
  while (it->valid()) {
    std::cout << it->current() << " ";
    it->next();
  }
  std::cout << "\n";
}

void check_eq(std::vector<list_ptr> &list, std::vector<unsigned int> &q, std::vector<bool> &q_bool) {
  int count = list.size(); 
  std::vector<iterator_ptr> iterator(count);
  for (int i=0;i<count;i++) iterator[i] = list[i]->get_iterator();

  std::vector< std::vector<bool> > was(count);
  for (int i=0;i<count;i++) was[i].resize(q_bool.size());

  int pos = 0;
  while (iterator[0]->valid()) {
    for (int i=0;i<count;i++) {
      ASSERT_TRUE(iterator[i]->valid());
      was[i][iterator[i]->current()] = true;
      iterator[i]->next();
    }
    pos++;
  }
  for (int i=0;i<count;i++) {
    ASSERT_FALSE(iterator[i]->valid());
  }

  for (int i=0;i<q_bool.size();i++) {
    for (int j=0;j<count-1;j++) ASSERT_EQ(was[j][i], was[j+1][i]);
    ASSERT_EQ(was[0][i], find(q.begin(), q.end(), i)!=q.end());
    ASSERT_EQ(was[0][i], q_bool[i]);
  }
  ASSERT_EQ(pos, (int)q.size());
}

std::pair<std::vector<std::vector<unsigned int> >, std::vector<std::vector<bool> > > get_graph(int size, int each) {
            std::vector<std::vector<unsigned int> >  g;
            std::vector<std::vector<bool> >  g_bool;
            g.resize(size);
            g_bool.resize(size);
            for (int i=0;i<size;i++) {
              auto res = build_list(each);
              g[i] = res.first;
              g_bool[i] = res.second;
            }
            return make_pair(g, g_bool);
          }

std::pair<std::vector<std::vector<unsigned int> >,
          std::vector<std::vector<bool> > > get_full_graph(int sz) {
            return get_graph(sz, sz/3);
          }


std::pair<std::vector<std::vector<unsigned int> >,
          std::vector<std::vector<bool> > > get_sparse_graph(int sz) {
            return get_graph(sz, sz/100);
          }

TEST(Prepare, GenerateRandom) {
  srand(time(NULL));
}
/*  
TEST(ListOfIncedents, CheckCorrectness) {
  const int tests = 20;
  const int size = 1000;
  for (int te=0;te<tests;te++) {
    auto res = build_list(size);
    auto q = res.first;
    auto q_bool = res.second;
    std::vector<list_ptr> list(4);
    list[0] = list_ptr(new BitsetList(res.first));
    list[1] = list_ptr(new VectorList(res.first));
    list[2] = list_ptr(new BitsetList(res.second));
    list[3] = list_ptr(new VectorList(res.second));
    check_eq(list, q, q_bool); 
  }
}

TEST(Factories, CheckCorrectness) {
  int tests = 20;
  int vertexes = 500;
  for (int te=0;te<tests;te++) {
    std::vector<std::vector<unsigned int> > graph;
    std::vector<std::vector<bool> > graph_bool;
    int size = vertexes + rand()%vertexes;
    graph.resize(size);
    graph_bool.resize(size);
    for (int i=0;i<size;i++) {
      //test%2==0 - almost full graph
      //test%2==1 - almost empty graph
      int sz;
      if (te%2==0) {
        sz = size/3;
      }
      else {
        sz = size/100;
      }
      auto res = build_list(sz);
      graph[i] = res.first;
      graph_bool[i] = res.second;
    }
    std::vector<std::vector<factory_ptr> > factory(3);
    for (int i=0;i<size;i++) factory[0].push_back(std::unique_ptr<ListFactory>(new VectorFactory(size)));
    for (int i=0;i<size;i++) factory[1].push_back(std::unique_ptr<ListFactory>(new BitsetFactory(size)));
    for (int i=0;i<size;i++) factory[2].push_back(std::unique_ptr<ListFactory>(new CleverFactory(size)));
    for (int i=0;i<size;i++) 
      for (int j=0;j<graph[i].size();j++)
        for (int fact=0;fact<3;fact++) {
          factory[fact][i]->add_vertex(graph[i][j]);
        }
    for (int i=0;i<size;i++) {
      std::vector<list_ptr> list(3);
      for (int j=0;j<3;j++) list[j] = factory[j][i]->get_list();
      check_eq(list, graph[i], graph_bool[i]);
    }
  }
}
TEST(Graph, CheckCorrectness) {
  int tests = 20;
  int vertexes = 500;
  for (int te=0;te<tests;te++) {
    std::vector<std::vector<unsigned int> > graph;
    std::vector<std::vector<bool> > graph_bool;
    int size = vertexes + rand()%vertexes;
    //test%2==0 - almost full graph
    //test%2==1 - sparse graph
    
    std::pair<std::vector<std::vector<unsigned int> >,
              std::vector<std::vector<bool> > > result;
    if (te%2==0) result = get_full_graph(size);
    else result = get_sparse_graph(size);
    graph = result.first;
    graph_bool = result.second;

    std::vector< factory_ptr> factory, factory_bool;
    factory = build_factories(graph);
    factory_bool = build_factories(graph_bool);
    for (int i=0;i<size;i++) {
      if (te%2==0) {
        ASSERT_EQ(factory[i]->get_type(), "Clever --> Bitset");
        ASSERT_EQ(factory_bool[i]->get_type(), "Clever --> Bitset");
      }
      else {
        ASSERT_EQ(factory[i]->get_type(), "Clever --> Vector");
        ASSERT_EQ(factory_bool[i]->get_type(), "Clever --> Vector");
      }
    }
    Graph my_graph(std::move(factory));
    Graph my_graph_bool(std::move(factory_bool));

    for (int i=0;i<size;i++) {
      for (int j=0;j<size;j++) {
        bool conn;
        if (graph_bool[i].size()<=j) conn = false;
        else conn = graph_bool[i][j];
        ASSERT_EQ(my_graph.get_list(i).is_connected(j), conn);
        ASSERT_EQ(my_graph_bool.get_list(i).is_connected(j), conn);
      }
    }
  }

}

void test_speed(Graph &graph, int size) {
  for (int i=0;i<size;i++)
    for (int j=0;j<size;j++)
      graph.get_list(i).is_connected(j);
}

TEST(SparseGraph, Speed) {
  int size = 3000;
  int tests = 2;
  for (int te=0;te<tests;te++) {
    int col = size+rand()%size;
    auto gr = get_sparse_graph(col).first;
    std::vector<factory_ptr> factory;
    factory = build_factories(gr);
    Graph graph(std::move(factory));
    test_speed(graph, col);
  }
}

TEST(FullGraph, Speed) {
  int size = 3000;
  int tests = 2;
  for (int te=0;te<tests;te++) {
    int col = size+rand()%size;
    auto gr = get_full_graph(col).first;
    std::vector<factory_ptr> factory;
    factory = build_factories(gr);
    Graph graph(std::move(factory));
    test_speed(graph, col);
  }
}
*/

class TesterSCA {
  public: 
    TesterSCA(const Graph &_graph):graph(_graph), sca(_graph) {}
    
    const std::vector<std::pair<unsigned int, unsigned int> >& get_result() {
      return sca.result;
    }

    const std::vector<unsigned int>& get_colors() {
      return sca.colors;
    }

  private:
    StrongConnectivityAugmentation sca;
    const Graph &graph;
};


TEST(UnitTests, StrongConnectivityAugmentationManual1) {
	std::vector<std::vector<unsigned int> > edges(4);
	edges[0].push_back(1);
	edges[1].push_back(0);
	edges[2].push_back(3);
	edges[3].push_back(2);
  auto factory = build_factories(edges);

  Graph graph(std::move(factory));
	auto tester = TesterSCA(graph);
  auto answer = tester.get_result();
	EXPECT_EQ(answer.size(), 2);
  for (int i=0;i<answer.size();i++) {
    edges[answer[i].first].push_back(answer[i].second);
  }

  auto factory1 = build_factories(edges);
  Graph graph1 = Graph(std::move(factory1));
	auto tester1 = TesterSCA(graph1);
  auto answer1 = tester1.get_result();
	EXPECT_EQ(answer1.size(), 0);
}

TEST(UnitTests, StrongConnectivityAugmentationManual2) {
	std::vector<std::vector<unsigned int>> edges(5);
	for (int i = 0; i < 4; i++)
		edges[i].push_back(i+1);
  auto factory = build_factories(edges);
	Graph graph(std::move(factory));
	TesterSCA tester(graph);
  auto answer = tester.get_result();
	EXPECT_EQ(answer.size(), 1);
	EXPECT_EQ(answer[0], std::make_pair((unsigned int)4, (unsigned int)0));
}


TEST(UnitTest, GettingStronglyConnectedComponents) {
	const int graph_size = 1000;
	const int size_of_component = 10;
	std::vector<std::vector<unsigned int> > edges(graph_size);
	for (int i=0;i<graph_size;i++) {
		if ((i+1)%size_of_component==0)
			edges[i].push_back(i+1-size_of_component);
		else
			edges[i].push_back(i+1);
	}
	for (int i = 1; i < graph_size; i++)
		if (i%size_of_component==1 && i+size_of_component<graph_size)
			edges[i].push_back(i+size_of_component);
  auto factory = build_factories(edges);
	Graph graph(std::move(factory));
	TesterSCA tester(graph);
  auto colors = tester.get_colors();
	unsigned int lastcolor = 0;
  for (int i=0;i<colors.size();i++) lastcolor = std::max(lastcolor, colors[i]);
	EXPECT_EQ(lastcolor, graph_size/size_of_component-1);
	for (int i=0;i<graph_size;i++)
		for (int j=i;j<graph_size;j++)
			EXPECT_EQ(i/size_of_component == j/size_of_component, colors[i]==colors[j]);
}

TEST(IntegrationTests, StrongConnectivityAugmentationStressTest) {
	const unsigned int number_of_tests=5;
	const unsigned int number_of_verteces=1000;
	const unsigned int number_of_edges=1000;
	for (int test_case=0;test_case<number_of_tests;test_case++) {
		std::vector<std::vector<unsigned int> > edges(number_of_verteces, std::vector<unsigned int>());
		for (int i=0;i<number_of_edges;i++) {
			unsigned int left = rand()%number_of_verteces;
			unsigned int right = rand()%number_of_verteces;
			edges[left].push_back(right);
		}
    auto factory = build_factories(edges);
    Graph graph(std::move(factory));
    TesterSCA tester(graph);
    auto answer = tester.get_result();

    //Check correctness 
		for (unsigned int i=0;i<answer.size();i++)
			edges[answer[i].first].push_back(answer[i].second);
    auto factory1 = build_factories(edges);
    Graph graph1(std::move(factory1));
    TesterSCA tester1(graph1);
    auto colors = tester1.get_colors();
    unsigned int max_color = 0;
    for (int i=0;i<colors.size();i++) max_color = std::max(max_color, colors[i]);
    for (int i=0;i<colors.size();i++) std::cout << colors[i] << "\n";
		EXPECT_EQ(max_color, 0);
    exit(0);
		//Check if optimal
    colors = tester.get_colors();
    max_color = 0;
    for (int i=0;i<colors.size();i++) max_color = std::max(max_color, colors[i]);

    std::vector<char> has_input(max_color+1, 0), has_output(max_color+1, 0);
		for (unsigned int i=0;i<graph.size();i++) {
			auto it = graph.get_list(i).get_iterator();
			while (it->valid()) {
				if (colors[i] != colors[it->current()])
					has_output[colors[i]] = has_input[colors[it->current()]] = 1;
				it->next();
			}
		}
		int sources = 0, sinks = 0;
		for (unsigned int i=1;i<=max_color;i++) {
			if (!has_input[i])
				sources++;
			if (!has_output[i])
				sinks++;
		}
		if (max_color==0)
			EXPECT_EQ(answer.size(), 0);
		else
			EXPECT_EQ(answer.size(), std::max(sources, sinks));
	}
}
