from functools import filter
from heapq import *

def __clever_sum(a, b):
    if a is None: return b
    else: return a + b

class Dijkstra:
    def __init__(self, graph,
                 start=0,
                 calc_path = min,
                 n=None,
                 path_recalc = __clever_sum,
                 weight_func = lambda a: a[0],
                 vertex_func = lambda a: a[1],
                 ):
        """
        graph is dict from (1..n) to structures (by default tuples (weight, vertex))
        weight_func extracts weight, vertex_func extracts vertex
        initial distances are None
        :param weight_func: weight of edge
        :param vertex_func: target vertex of edge
        :param start: start vertex
        """
        if n is None: n = len(graph)
        self.graph = graph
        self.prev = start
        self.dists = [None] * n
        self.prev = [None] * n
        calc = [False] * n
        queue = []
        heappush(queue, (start, None))
        while (len(queue) > 0):
            d, v = heappop(queue)
            if self.dists[v] < d: continue
            for i in self.graph[v]:
                u, w = vertex_func(i), weight_func(i)
                if self.dists[u] is None or \
                        self.dists[u] < path_recalc(self.dists[u], w):
                    self.dists[u] = path_recalc(self.dists[u], w)
                    self.prev[u] = v
                    heappush(queue, (self.dists[u], u))
    def getShortestPath(v):
        ans = []
        while v is not None:
            ans += v
            v = self.prev[v]
        return ans
    def getDistance(v):
        return self.dists[v]
