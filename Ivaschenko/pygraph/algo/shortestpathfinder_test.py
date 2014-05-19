__author__ = 'skird'

import unittest
from algo.shortestpathfinder import ShortestPathFinder


class ShortestPathFinderIntegration(unittest.TestCase):
    def setUp(self):
        pass

    def test_on_simple_graph(self):
        finder = ShortestPathFinder(['a', 'b', 'c', 'd'])
        finder.add_edge('a', ('b', 5))
        finder.add_edge('a', ('c', 3))
        finder.add_edges([('b', ('d', 7)), ('c', ('d', 10))])
        self.assertRaises(KeyError, finder.add_edge, 'x', ('a', 5))
        self.assertRaises(KeyError, finder.add_edge, 'b', ('x', 1))
        self.assertRaises(KeyError, finder.add_edge, 'w', ('z', 13))
        self.assertRaises(KeyError, finder.add_edges, [('b', ('a', 100)), ('z', ('a', 1))])

        weight, path = finder.find_shortest_path('a', 'd')
        self.assertEqual(weight, 12)
        self.assertListEqual(path, [('b', 5), ('d', 7)])

        weight, path = finder.find_shortest_path('a', 'a')
        self.assertEqual(weight, 0)
        self.assertListEqual(path, [])

        weight, path = finder.find_shortest_path('d', 'a')
        self.assertIsNone(weight)
        self.assertIsNone(path)

        finder.erase_edge('a', ('b', 5))
        weight, path = finder.find_shortest_path('a', 'd')
        self.assertEqual(weight, 13)
        self.assertListEqual(path, [('c', 3), ('d', 10)])

        finder.erase_edge('a', ('b', 5))  # remove edge twice (do nothing)
        weight, path = finder.find_shortest_path('a', 'd')
        self.assertEqual(weight, 13)
        self.assertListEqual(path, [('c', 3), ('d', 10)])
