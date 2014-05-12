class Dijkstra:
    @staticmethod
    def find_shortest_paths(graph, from_vertices, *,
                            initial_path_weight=0,
                            comparator=lambda w1, w2: w1 < w2,
                            append_edge=lambda path, edge: path + edge,
                            edge_destination=lambda edge: edge) -> dict:
        """

            :param graph: Iterable, indexable container each element of which is iterable list of edges
            :param from_vertices: List of vertices to find path from
            :param initial_path_weight: Initial value of weight for starting vertices.
                   It is considered by neutral element for append_edge function
            :param comparator: Function used to compare two paths' weights
            :param append_edge: Function evaluating weight of path with one appended edge
            :param edge_destination: Function used to determine destination of edge. Should return valid index in graph.
                   By default it is assumed that edge is destination itself
            :return Dict mapping each vertex to its shortest path if it exists
            :rtype : dict

             For weight function there are following restrictions:
                - There are no negative edges, so weight of append_edge(path, edge) is not less then weight of path
                - All shortest path are equally good, so if weight of path1 is equal to weight of path2 then
                  append_edge(path1, edge) is equal append_edge(path2, edge)
            """
        gray = list(from_vertices)
        dist = dict()
        for v in from_vertices:
            dist[v] = initial_path_weight
        while len(gray) > 0:
            v = Dijkstra.__find_nearest(gray, dist, comparator)
            gray.remove(v)
            for edge in graph[v]:
                Dijkstra.__relax_edge(v, edge, gray, dist, comparator, edge_destination, append_edge)
        return dist

    @staticmethod
    def __find_nearest(gray, dist, comparator):
        (min_dist, ans) = (None, None)
        for v in gray:
            if min_dist is None or comparator(dist[v], min_dist):
                (min_dist, ans) = (dist[v], v)
        return ans

    @staticmethod
    def __relax_edge(v, edge, gray, dist, comparator, edge_destination, append_edge):
        to = edge_destination(edge)
        w = append_edge(dist[v], edge)
        if to not in dist:
            gray.append(to)
            dist[to] = w
        else:
            if comparator(w, dist[to]):
                dist[to] = w