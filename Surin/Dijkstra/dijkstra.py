from heapq import *


class Dijkstra:
    def __init__(self, graph,
                 start=0,
                 start_arr=None,
                 calc_path=min,
                 n=None,
                 path_rec=lambda a, b: a + b,
                 weight_func=lambda a: a[0],
                 vertex_func=lambda a: a[1],
                 init_id=0
                 ):
        """
        graph is dict from (1..n)
        to structures (by default tuples (weight, vertex))
        weight_func extracts weight, vertex_func extracts vertex
        initial distances are None
        :param weight_func: weight of edge
        :param vertex_func: target vertex of edge
        :param start: start vertex
        :param init_id: distance for start vertex
        """
        if n is None:
            n = len(graph)
        self.n = n
        if start_arr is not None: self.start = {i for i in start_arr}
        else: self.start = {start}
        self.graph = graph
        self.dists, self.prev = {i: init_id for i in self.start}, {}
        calc = set()
        queue = []
        for i in self.start:
            heappush(queue, (init_id, i))
        while len(queue) > 0:
            d, v = heappop(queue)
            if v in calc:
                continue
            calc.add(v)
            for i in self.graph[v]:
                u, w = vertex_func(i), weight_func(i)
                if u not in calc and \
                    (u not in self.dists or
                        self.dists[u] < path_rec(self.dists[u], w)):
                    self.dists[u] = path_rec(self.dists[v], w)
                    self.prev[u] = v
                    heappush(queue, (self.dists[u], u))

    def getShortestPath(self, v):
        ans = []
        while v in self.prev:
            ans += [v]
            v = self.prev[v]
        if v in self.start:
            ans += [v]
            ans.reverse()
            return ans

    def getDistance(self, v):
        return self.dists[v]

    def getDistsList(self):
        """
        works only with 0..n vertices
        """
        return [self.dists[i] for i in range(self.n)]
