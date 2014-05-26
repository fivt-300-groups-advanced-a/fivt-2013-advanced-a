import heapq


def _dijkstra_internal(graph_wrapper, start_v, start_p, path_lt, sum_pe):
    class HeapElement:
        def __init__(self, vertex, path):
            self.vertex = vertex
            self.path = path

        def __lt__(self, other):
            return path_lt(self.path, other.path)

        def __repr__(self):
            return "(v={}, path={})".format(self.vertex, self.path)

    final_paths = dict()
    heap = [HeapElement(start_v, start_p)]

    while len(heap) > 0:
        top = heapq.heappop(heap)
        if top.vertex in final_paths:
            continue
        final_paths[top.vertex] = top.path

        for to, e in graph_wrapper(top.vertex):
            if to in final_paths:
                if path_lt(sum_pe(top.path, e), final_paths[to]):
                    raise AssertionError('Negative loop encountered')
                continue
            heapq.heappush(heap, HeapElement(to, sum_pe(top.path, e)))

    return final_paths


def dijkstra(graph,
             start_v,
             start_p,
             *,
             path_lt=lambda a, b: a < b,
             **kwargs):
    """
    Dijkstra algo.
    Returns dict V->P or V->(P,backtrace_generator)
    Will raise AssertionError in case if negative loop is encountered.

    :rtype : dict

    Parameters:

    graph: container vertex->(dict|iterable).

    start_v: start vertex

    start_p: start path

    Keyword parameters:

    path_lt: function(a, b) will be used to compare two paths. Default: a < b.

    edge_to: function(edge) returns the destination vertex id and should be provided if graph[v] is not dict
    and graph is not adjacency matrix.

    edge_w: function(edge) will be used to fetch weight from edge in case if sum_pe is not explicitly
    provided. By default return edge.

    sum_pe: function(path, edge) will be used to update path adding edge at the end. Default: path + edge.
    """

    for k in kwargs.keys():
        if k not in {'sum_pe', 'edge_w', 'edge_to'}:
            raise TypeError("dijkstra got an unexpected keyword argument '{}'".format(k))

    sum_pe = kwargs.get('sum_pe', None)
    edge_w = kwargs.get('edge_w', None)
    edge_to = kwargs.get('edge_to', None)

    if edge_w is None:
        edge_w = lambda w: w
    if sum_pe is None:
        sum_pe = lambda p, e: p + edge_w(e)

    def dict_wrapper(adj):
        yield from adj.items()

    def edge_to_wrapper(adj):
        for e in adj:
            yield edge_to(e), e

    def adj_matrix_line_wrapper(adj):
        for i, e in enumerate(adj):
            if e is not None:
                yield i, e

    adj_list_wrapper = None
    if isinstance(graph[start_v], dict):
        adj_list_wrapper = dict_wrapper
    elif edge_to is not None:
        adj_list_wrapper = edge_to_wrapper
    else:
        adj_list_wrapper = adj_matrix_line_wrapper

    def graph_wrapper(v):
        yield from adj_list_wrapper(graph[v])

    return _dijkstra_internal(graph_wrapper, start_v, start_p, path_lt, sum_pe)
