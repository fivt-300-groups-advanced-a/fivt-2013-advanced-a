__author__ = 'pershik'

from queue import PriorityQueue


class PQEntry:
    def __init__(self, prior, value, cmp=lambda prior1, prior2: prior1 < prior2):
        self.prior = prior
        self.value = value
        self.cmp = cmp

    def __lt__(self, other):
        return self.cmp(self.prior, other.prior)


class Dijkstra:
    def __init__(self):
        self.dist = []
        self.used = []
        self.cnt_vertexes = 0
        self.pr_queue = PriorityQueue()

    def get_shortest_paths(self, graph, from_vertex, init_path_value,
                           cmp=lambda weight1, weight2: weight1 < weight2,
                           add_edge=lambda path_weight, edge: path_weight + edge.weight,
                           get_destination=lambda edge: edge.to):
        """

        :param graph: List of lists of Edges. Vertexes: from 0 to len(graph) - 1
        :param from_vertex: Vertex the algorithm should start
        :param init_path_value: Cost of path from from_vertex to itself
        :param cmp: cmp(weight1, weight2). Return true if weight1 < weight2, false - otherwise
        :param add_edge: add_edge(path_weight, edge). Return the cost of path if we add Edge to path_weight
        :param get_destination: get_edge_vertex(edge). Return the sink of the edge
        :return List with shortest paths to every vertex from from_vertex (None for unreachable vertexes)
        """
        self._init_dijkstra(graph)
        self.dist[from_vertex] = init_path_value
        self.pr_queue.put(PQEntry(init_path_value, from_vertex, cmp))

        while True:
            cur_vertex = self._get_min_vertex(cmp)
            if cur_vertex == -1:
                break
            self.used[cur_vertex] = True
            for edge in graph[cur_vertex]:
                self._relax_edge(cur_vertex, edge, cmp, add_edge, get_destination)

        return self.dist

    def _relax_edge(self, from_vertex, edge, cmp, add_edge, get_destination):
        """

        :param from_vertex: Vertex where the edge start
        :param edge: Edge which you want to relax
        :param cmp: cmp(weight1, weight2). Return true if weight1 < weight2, false - otherwise
        :param add_edge: add_edge(path_weight, edge). Return the cost of path if we add Edge to path_weight.
                         (None if it's impossible to use this edge)
        :param get_destination: get_edge_vertex(edge). Return the sink of the edge
        """
        to_vertex = get_destination(edge)
        if add_edge(self.dist[from_vertex], edge) is None:
            return
        if (self.dist[to_vertex] is None or
                cmp(add_edge(self.dist[from_vertex], edge), self.dist[to_vertex])):
            self.dist[to_vertex] = add_edge(self.dist[from_vertex], edge)
            self.pr_queue.put(PQEntry(self.dist[to_vertex], to_vertex, cmp))

    def _init_dijkstra(self, graph):
        self.cnt_vertexes = len(graph)
        self.used = [False] * self.cnt_vertexes
        self.dist = [None] * self.cnt_vertexes

    def _get_min_vertex(self, cmp):
        """

        :param cmp: cmp(weight1, weight2). Return true if weight1 < weight2, false - otherwise
        :return: Vertex with minimal current distance. -1 - if the vertex pr_queue is empty
        """
        while not self.pr_queue.empty():
            top = self.pr_queue.get()
            if (not cmp(top.prior, self.dist[top.value]) and
                    not cmp(self.dist[top.value], top.prior)):
                return top.value
        return -1