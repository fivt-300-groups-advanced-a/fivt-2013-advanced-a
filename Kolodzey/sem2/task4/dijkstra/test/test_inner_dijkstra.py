from dijkstra.graph import Graph
#Nelly
Nelly = Graph()
Nelly.add_vertex_list(range(1, 7))
Nelly.add_edge(2, 1, 1)
Nelly.add_edge(2, 5, 4)
Nelly.add_edge(2, 4, 2)
Nelly.add_edge(3, 4, 1)
Nelly.add_edge(4, 6, 4)
#Oliver
Oliver = Graph()
Oliver.add_vertex_list("ABCDEF")
Oliver.add_edge('A', 'B', 2)
Oliver.add_edge('B', 'A', 4)
Oliver.add_edge('B', 'C', 5)
Oliver.add_edge('B', 'D', 11)
Oliver.add_edge('B', 'F', 2)
Oliver.add_edge('C', 'A', 7)
Oliver.add_edge('D', 'B', 9)
Oliver.add_edge('D', 'E', 3)
Oliver.add_edge('E', 'C', 8)
Oliver.add_edge('E', 'D', 6)
Oliver.add_edge('F', 'D', 1)
Oliver.add_edge('F', 'E', 5)
#look at graphs at http://goo.gl/Yr7f64

from collections import namedtuple

import unittest

from dijkstra.inner_dijkstra import inner_dijkstra

from dijkstra.graph import default_destination_vertex_extract
from dijkstra.graph import default_weight_extract

from dijkstra.algo import default_compare_distance
from dijkstra.algo import default_add_edge_to_distance

from dijkstra.heap import LinearPriorityQueue

FullHold = namedtuple("FullHold", "distance parent edge")
OnlyParentHold = namedtuple("OnlyParentHold", "distance parent")

class TestInnerDijkstra(unittest.TestCase):
    def test_base_launch(self):
        self.assertEqual(inner_dijkstra(graph = {'A':[('B', 1), ('C', 5)],
                                                 'B':[('C', 2)],
                                                 'C':[]},
                                        extract_destination_vertex=default_destination_vertex_extract,
                                        extract_weight=default_weight_extract,
                                        start = {'A': 0},
                                        compare_distance=default_compare_distance,
                                        add_edge_to_distance=default_add_edge_to_distance,
                                        backtrace_mode="off",
                                        heap_structure=LinearPriorityQueue),
                         {'C': 3,
                          'B': 1})

    def test_on_nelly(self):
        nelly_tests = [({1: 0}, {}),
                       ({2: 0}, {1: 1, 5: 4, 4: 2, 6: 6}),
                       ({3: 0}, {4: 1, 6: 5}),
                       ({4: 0}, {6: 4}),
                       ({5: 0}, {}),
                       ({6: 0}, {})]
        for case in nelly_tests:
            self.assertEqual(inner_dijkstra(graph=Nelly,
                                 extract_destination_vertex=default_destination_vertex_extract,
                                 extract_weight=default_weight_extract,
                                 start = case[0],
                                 compare_distance=default_compare_distance,
                                 add_edge_to_distance=default_add_edge_to_distance,
                                 backtrace_mode="off",
                                heap_structure=LinearPriorityQueue),
                             case[1])

    def test_on_oliver(self):
        oliver_tests = [({'A': 0}, {         'B': 2,  'C': 7,  'D': 5,  'E': 8,  'F': 4}),
                        ({'B': 0}, {'A': 4,           'C': 5,  'D': 3,  'E': 6,  'F': 2}),
                        ({'C': 0}, {'A': 7,  'B': 9,           'D': 12, 'E': 15, 'F': 11}),
                        ({'D': 0}, {'A': 13, 'B': 9,  'C': 11,          'E': 3,  'F': 11}),
                        ({'E': 0}, {'A': 15, 'B': 15, 'C': 8,  'D': 6,           'F': 17}),
                        ({'F': 0}, {'A': 14, 'B': 10, 'C': 12, 'D': 1,  'E': 4,         })]
        for case in oliver_tests:
            self.assertEqual(inner_dijkstra(graph=Oliver,
                                 extract_destination_vertex=default_destination_vertex_extract,
                                 extract_weight=default_weight_extract,
                                 start = case[0],
                                 compare_distance=default_compare_distance,
                                 add_edge_to_distance=default_add_edge_to_distance,
                                 backtrace_mode="off",
                                heap_structure=LinearPriorityQueue),
                             case[1])

    def test_full_backtrace_mode(self):
                    self.assertEqual(inner_dijkstra(graph=Nelly,
                                 extract_destination_vertex=default_destination_vertex_extract,
                                 extract_weight=default_weight_extract,
                                 start = {3 : 0},
                                 compare_distance=default_compare_distance,
                                 add_edge_to_distance=default_add_edge_to_distance,
                                 backtrace_mode="full",
                                heap_structure=LinearPriorityQueue),
                        {4 : FullHold(1, 3, 1), 6: FullHold(5, 4, 4)})

    def test_only_parent_backtrace_mode(self):
                    self.assertEqual(inner_dijkstra(graph=Nelly,
                                 extract_destination_vertex=default_destination_vertex_extract,
                                 extract_weight=default_weight_extract,
                                 start = {3 : 0},
                                 compare_distance=default_compare_distance,
                                 add_edge_to_distance=default_add_edge_to_distance,
                                 backtrace_mode="only_parent",
                                heap_structure=LinearPriorityQueue),
                        {4 : OnlyParentHold(1, 3), 6: OnlyParentHold(5, 4)})

if __name__ == '__main__':
    unittest.main()