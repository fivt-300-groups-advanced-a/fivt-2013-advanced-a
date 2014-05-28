from dijkstra.heap import LinearPriorityQueue
from collections import namedtuple
import unittest

FullEdgeInfo = namedtuple("FullEdgeInfo", "parent edge")
OnlyParentEdgeInfo = namedtuple("OnlyParentEdgeInfo", "parent")


class TestLinearPriorityQueue(unittest.TestCase):

    def test_full_backtrace(self):
        queue = LinearPriorityQueue(lambda x, y: (x < y), "full")
        queue.update_distance('A', 3, 'C', 1)
        self.assertEqual(queue.pop_nearest(), ['A', 3, FullEdgeInfo('C', 1)])

    def test_backtrace_only_parent(self):
        queue = LinearPriorityQueue(lambda x, y: (x < y), "only_parent")
        queue.update_distance('A', 3, 'C', 1)
        self.assertEqual(queue.pop_nearest(), ['A', 3, OnlyParentEdgeInfo('C')])

    def test_backtrace_off(self):
        queue = LinearPriorityQueue(lambda x, y: (x < y), "off")
        queue.update_distance('A', 3, 'C', 1)
        self.assertEqual(queue.pop_nearest(), ['A', 3, None])


if __name__ == '__main__':
    unittest.main()