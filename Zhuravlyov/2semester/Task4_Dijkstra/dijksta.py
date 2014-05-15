__author__ = 'Алексей'

class Dijkstra:
    def __init__(self, graph, start_vertex, start_value = 0,
                 cmp = lambda w1, w2: w1 < w2,
                 sum_path_edge = lambda p, e: p + e.weight,
                 destination_by_edge = lambda edge: edge.destination):
        """
        Class constructor get all needed information about graph and calculate all shortest paths
        from start vertex with complexity O(V * V + E), where V is number of vertices, E is total number of edges
        Use get_path_values and get_path_to_vertex methods to get a result in needed form
        :param graph: list of lists of edges. Edge is by default structure (weight, destination)
        :param start_vertex: number of start vertex
        :param start_value: weight of path from vertex to itself
        :param cmp: cmp(weight1, weight2) Compares weights of two paths.
                    Returns true if weight1 < weight2; false otherwise
        :param sum_path_edge: sum(path_weight, edge). Adds edge to a path
                              returns value of path_weight with added weight of edge.
                              edge is a same structure as you use in graph
        :param destination_by_edge: returns incident vertex by edge
                                    No need to override it if you use (weight, destination) structure of edge
        """
        n = len(graph)
        used = [False] * n
        self.res = [None] * n
        self.previous = [None] * n
        self.res[start_vertex] = start_value
        self.previous[start_vertex] = start_vertex
        current_vertex = start_vertex
        while not current_vertex is None:
            current_vertex = None
            min_value = None
            for i in range(n):
                if not used[i] and not self.res[i] is None:
                    if min_value is None or cmp(self.res[i], min_value):
                        min_value = self.res[i]
                        current_vertex = i
            if not current_vertex is None:
                used[current_vertex] = True
                for edge in graph[current_vertex]:
                    new_path = sum_path_edge(self.res[current_vertex], edge)
                    incident_vertex = destination_by_edge(edge)
                    if self.res[incident_vertex] is None or cmp(new_path, self.res[incident_vertex]):
                        self.res[incident_vertex] = new_path
                        self.previous[incident_vertex] = current_vertex

    def get_path_values(self):
        """
        :return: list of weights of shortest paths from start vertex to all. None for unreachable vertices
                 res[i] is value of shortest path from start vertex to vertex with number i
        """
        return self.res

    def get_path_to_vertex(self, finish_vertex):
        """
        :param finish_vertex: number of vertex you want to know shortest path to
        :return: list of vertices you need to visit from start vertex to finish vertex in right order.
                 None if there is no path
        """
        if self.res[finish_vertex] is None:
            return None
        else:
            path = []
            i = finish_vertex
            while self.previous[i] != i:
                path.append(i)
                i = self.previous[i]
            path.append(i)
            path.reverse()
            return path