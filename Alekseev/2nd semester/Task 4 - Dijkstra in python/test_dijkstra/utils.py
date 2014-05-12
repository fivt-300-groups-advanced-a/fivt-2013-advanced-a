from copy import copy
from collections import namedtuple
import random

__author__ = 'moskupols'


class Graph:
    class Edge(namedtuple('Edge', ['source', 'to', 'weight'])):
        def __hash__(self):
            return hash((self.source, self.to))
        def __eq__(self, other):
            return (self.source, self.to) == (other.source, other.to)

    def __init__(self, edge_list):
        self._vertices = {source for source, to, w in edge_list}
        self._vertices.update({to for source, to, w in edge_list})
        self._vertices = list(self._vertices)

        edge_list = {Graph.Edge(source, to, w) for source, to, w in edge_list}
        self._edge_list = edge_list

        self._adj = {v: {} for v in self._vertices}
        for source, to, w in edge_list:
            self._adj[source][to] = self.Edge(source, to, w)

    @staticmethod
    def random(seed, vertices_count, edges_count):
        random.seed(seed)
        edges = set()
        edges_count = min(edges_count, vertices_count * (vertices_count - 1) / 2)
        while len(edges) < edges_count:
            edges.add(Graph.Edge(*[random.randint(0, vertices_count - 1) for i in range(3)]))
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


def ford_bellman(graph,
                 start_v,
                 start_p,
                 *,
                 edge_w=lambda e: e.weight,
                 cmp_path=lambda a, b: a < b,
                 sum_pe=None):
    if sum_pe is None:
        sum_pe = lambda p, e: p + edge_w(e)

    paths = {start_v: start_p}
    for i in range(len(graph.vertices)):
        tmp = copy(paths)
        for e in graph.edge_list:
            if not e.source in paths:
                    continue
            new_path = sum_pe(paths[e.source], e)
            if not e.to in tmp or cmp_path(new_path, tmp[e.to]):
                tmp[e.to] = new_path
        paths = tmp
    return paths
