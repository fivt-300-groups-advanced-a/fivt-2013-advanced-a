from copy import copy
from collections import namedtuple
import random


class Graph:
    class Edge(namedtuple('Edge', ['v', 'u', 'weight'])):
        def __hash__(self):
            return hash((self.v, self.u))

        def __eq__(self, other):
            return (self.v, self.u) == (other.v, other.u)

    def __init__(self, edge_list):
        self._vertices = {v for v, u, w in edge_list}
        self._vertices.update({u for v, u, w in edge_list})
        self._vertices = list(self._vertices)

        edge_list = {Graph.Edge(v, u, w) for v, u, w in edge_list}
        self._edge_list = edge_list

        self._adj = {v: {} for v in self._vertices}
        for v, u, w in edge_list:
            self._adj[v][u] = self.Edge(v, u, w)

        self._cached_paths = dict()

    @staticmethod
    def random(seed, vertices_count, edges_count):
        random.seed(seed)
        edges = set()
        edges_count = min(edges_count, vertices_count * (vertices_count - 1) / 2)
        while len(edges) < edges_count:
            edges.add(Graph.Edge(*[random.randint(0, vertices_count - 1) for _ in range(3)]))
        return Graph(edges)

    def __repr__(self):
        return 'vertices:\n{}\nedges:\n'.format(self.vertices) + \
               ('{}\n' * len(self.edge_list)).format(*self.edge_list)

    @property
    def vertices(self):
        return self._vertices

    @property
    def edge_list(self):
        return self._edge_list

    @property
    def adj(self):
        return self._adj

    def paths(self,
              start_v,
              start_p,
              **kwargs):
        hashable = True
        k = (start_v, start_p)
        try:
            hash(k)
        except TypeError:
            hashable = False

        if hashable:
            if k not in self._cached_paths:
                self._cached_paths[k] = self._ford_bellman(start_v, start_p, **kwargs)
            return self._cached_paths[k]
        else:
            return self._ford_bellman(start_v, start_p, **kwargs)

    def _ford_bellman(self,
                      start_v,
                      start_p,
                      *,
                      edge_w=lambda e: e.weight,
                      path_lt=lambda a, b: a < b,
                      sum_pe=None):
        if sum_pe is None:
            sum_pe = lambda p, e: p + edge_w(e)

        paths = {start_v: start_p}
        for i in range(len(self.vertices)):
            tmp = copy(paths)
            for e in self.edge_list:
                if not e.v in paths:
                        continue
                new_path = sum_pe(paths[e.v], e)
                if not e.u in tmp or path_lt(new_path, tmp[e.u]):
                    tmp[e.u] = new_path
            paths = tmp
        return paths
