import heapq


class Dijkstra:
    """
    Dijkstra algo. Supports way trace and backtrace.

    Examples time.

    It is quite easy to call dijkstra with graph represented as a container of adjacency lists of various structure.

    List of dicts (destination: weight):
    >>> Dijkstra([{0: 3, 1: 1, 2: 4},
    ...           {0: 1, 2: 2},
    ...           {1: 6, 2: 3}],
    ...          0).paths == {0: 0, 1: 1, 2: 3}
    True

    Dict of lists of pairs (destination, weight) (note that in this case multiple edges with same begin and end are
    possible):
    >>> dij = Dijkstra({'Ignat': [( 0, 6), ('Ignat', 3)],
    ...                 0: [(-5, 1), ('Ignat', 2)],
    ...                -5: [(-5, 3), (0, 6), (0, 1), ('Ignat', 4)]},
    ...                -5, backtrace=True)
    >>> dij.paths == {-5: 0, 0: 1, 'Ignat': 3}
    True

    Now suppose we wanted to know not only distance to Ignat, but also how to get to him:
    >>> dij.trace_to('Ignat')
    [(-5, (0, 1), 0), (0, ('Ignat', 2), 1), ('Ignat', None, 3)]

    Or how to get back to -5 (memory efficient as we are not so conscious after our meeting with Ignat):
    >>> for v, e, p in dij.backtrace_from('Ignat'):
    ...     print(v)
    Ignat
    0
    -5

    And finally an adjacency matrix.
    >>> Dijkstra([[   3,    1, 4],
    ...           [   1, None, 2],
    ...           [None,    6, 3]],
    ...          0,
    ...          edge_to=None).paths == {0: 0, 1: 1, 2: 3}
    True

    Negative weighted edges are not allowed (more precisely, the situation when we have to relax an edge pointing
    at vertex that is already processed (black)).
    >>> Dijkstra([{1: -1}, {}], 0).paths == {0: 0, 1: -1}
    True
    >>> Dijkstra([{1: -1}, {0: -1}], 0)
    Traceback (most recent call last):
        ...
    AssertionError: Dijkstra: Negative weighted edge encountered

    """

    @staticmethod
    def _impl(graph_wrapper, start_v, start_p, cmp_path_lt, sum_pe, backtrace)->dict:
        """
        Dijkstra algo implementation with heapq heap.

        For information on parameters but graph_wrapper see dijkstra docs.

        Raises AssertionError in case if a negative weighted edge is encountered.

        :param graph_wrapper: graph_wrapper(v) should be a generator of tuples (destination_of_edge, edge) of all edges
        from vertex v
        """
        class HeapElement:
            """Actually just a namedtuple with custom comparator to store in heap"""

            def __init__(self, vertex, path, prev_v, last_e):
                self.vertex = vertex
                self.path = path
                if backtrace:
                    self.prev_v = prev_v
                    self.last_e = last_e

            def __lt__(self, other):
                return cmp_path_lt(self.path, other.path)

        final_paths = dict()
        bt = dict() if backtrace else None
        heap = [HeapElement(start_v, start_p, None, None)]

        while len(heap) > 0:
            top = heapq.heappop(heap)
            if top.vertex in final_paths:
                continue
            final_paths[top.vertex] = top.path
            if backtrace:
                bt[top.vertex] = top.prev_v, top.last_e

            for to, e in graph_wrapper(top.vertex):
                new_path = sum_pe(top.path, e, to)
                if new_path is None:
                    continue
                if to in final_paths:
                    if cmp_path_lt(new_path, final_paths[to]):
                        raise AssertionError('Dijkstra: Negative weighted edge encountered')
                    continue
                heapq.heappush(heap, HeapElement(to, new_path, top.vertex, e))

        return final_paths, bt

    @staticmethod
    def _prepare_impl_params(graph,
                             start_v,
                             start_p,
                             cmp_path_lt,
                             edge_to,
                             edge_w,
                             sum_pev,
                             backtrace):
        if sum_pev is None:
            sum_pev = lambda p, e, _: ((p + edge_w(e)) if e is not None else None)

        if isinstance(graph[start_v], dict):
            # dict case
            graph_wrapper = lambda v: graph[v].items()
            if edge_w is None:
                edge_w = lambda e: e
        elif edge_to is None:
            # matrix case
            graph_wrapper = lambda v: enumerate(graph[v])
            if edge_w is None:
                edge_w = lambda e: e
        else:
            # abstract iterable case
            graph_wrapper = lambda v: ((edge_to(e), e) for e in graph[v])
            if edge_w is None:
                edge_w = lambda e: e[1]
        return {'graph_wrapper': graph_wrapper,
                'start_v': start_v,
                'start_p': start_p,
                'cmp_path_lt': cmp_path_lt,
                'sum_pe': sum_pev,
                'backtrace': backtrace}

    def __init__(self,
                 graph,
                 start_v,
                 *,
                 start_p=0,
                 cmp_path_lt=lambda a, b: a < b,
                 edge_to=lambda e: e[0],
                 edge_w=None,
                 sum_pev=None,
                 backtrace=False):
        """
        The main dijkstra algo entry.
        Returns dict (Vertex->Path).

        If graph[start_v] is dict, its keys are used as destination vertices, and its values are considered to be edge
        info to pass to edge_w for retrieving its weight.
        In this case edge_w by default returns the edge as is.

        Otherwise, if edge_to is not explicitly set to None, edge_to is called with each element to determine the
        destination vertex.
        In this case edge_w defaults to (lambda e: e[1]).

        Finally, if graph[start_v] is not dict, and edge_to is explicitly set to None, the graph is considered to be an
        adjacency matrix.
        In this case the column number is used in place of edge_to. edge_w returns the edge as is.

        Note that the default implementation of sum_pe ignores None edges, and None paths are considered specially.
        See documentation on sum_pe for details.


        :param graph: subscriptable container vertex -> (dict|iterable).
        graph[i] should all be similar to graph[start_v] in terms of cooperation with edge_to and edge_w parameters.

        :param start_v: start vertex

        :param start_p: initial path.
        Default: 0.

        :param cmp_path_lt: will be used to compare two paths.
        Default: (lambda a, b: a < b).

        :param edge_to: function(edge) returns the destination vertex id of the given edge.
        If edge_to is None and graph[start_v] is not a dict instance, the given graph is considered to be an adjacency
        matrix, and column number is used instead of this function.
        Default: (lambda e: e[0]).

        :param edge_w: function(edge) will be used to fetch weight from edge in case if sum_pe is not explicitly
        provided.
        Default: (lambda e: e) in case of dict and adjacency matrix, (lambda e: e[1]) otherwise.

        :param sum_pev: function(path, edge, vertex) will be used to update path by adding edge at the end.
        The function should return either the new path or special value None if it is impossible to relax the given
        edge at the end of given path. The last parameter could be useful to implement custom backtrace functions.
        Defaults to (lambda path, edge: path + edge_w(edge) if edge is not None else None).
        """

        self._paths, self._bt = Dijkstra._impl(**Dijkstra._prepare_impl_params(graph, start_v, start_p, cmp_path_lt,
                                                                               edge_to, edge_w, sum_pev, backtrace))

    @property
    def paths(self):
        return self._paths

    def _check_backtrace_available(self):
        """Check if backtrace info is available, raise error otherwise. some
        """
        if self._bt is None:
            raise TypeError('Dijkstra: Backtrace info is not available. Construct Dijkstra with param backtrace=True.')

    def backtrace_from(self, dest):
        """
        Generates pairs of (vertex, last_edge_of_path_to_it, path_to_it) representing the optimal way to dest
        """
        self._check_backtrace_available()
        if dest not in self._bt:
            raise KeyError('Dijkstra.backtrace: There is no way to the given vertex.')
        edge = None
        while dest is not None:
            yield dest, edge, self._paths[dest]
            dest, edge = self._bt[dest]

    def trace_to(self, dest):
        """
        Returns way to dest in a list. Actually it is the reversed output of backtrace_from.
        """
        ret = list(self.backtrace_from(dest))
        ret.reverse()
        return ret


if __name__ == '__main__':
    import doctest
    doctest.testmod()
