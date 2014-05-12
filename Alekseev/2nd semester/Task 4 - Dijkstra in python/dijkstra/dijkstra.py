__author__ = 'moskupols'

import heapq


def _gen_heap_element(cmp):
    """
    :param cmp: comparator to use for paths
    :return: HeapElement helper for Dijkstra algo with given cmp.
    """

    class HeapElement:
        def __init__(self, vertex, path):
            self.vertex = vertex
            self.path = path

        def __lt__(self, other):
            return cmp(self.path, other.path)

        def __repr__(self):
            return "(v={}, path={})".format(self.vertex, self.path)

    return HeapElement


def _adj_list_wrapper(adj, edge_to):
    """
    Generates tuples of (to, edge) from an iterable adjacency.
    if adj is dict, behaves like adj.items().
    elif edge_to is None, behaves like enumerate(adjacency).
    else generates (edge_to(e), e) for e in adj

    :rtype : (vertex, edge)
    :param adj: iterable
    :param edge_to:
    """
    if isinstance(adj, dict):
        yield from adj.items()
#        for k, v in adj.items():
#            yield k, v
    elif edge_to is None:
        yield from enumerate(adj)
#        for i, e in enumerate(adj):
#            yield i, e
    else:
        for e in adj:
            yield edge_to(e), e


def dijkstra(adj,
             start_v,
             start_p,
             *,
             edge_to=lambda e: e.to,
             edge_w=lambda e: e.weight,
             cmp_path=lambda a, b: a < b,
             sum_pe=None):
    """

    :param adj: container vertex->(dict|iterable)
    :param start_v:
    :param start_p:
    :param edge_w:
    :param cmp_path:
    :param sum_pe:
    """

    if sum_pe is None:
        sum_pe = lambda p, e: p + edge_w(e)

    final_paths = dict()
    heap_element = _gen_heap_element(cmp_path)
    heap = [heap_element(start_v, start_p)]

    while len(heap) > 0:
        top = heapq.heappop(heap)
        if top.vertex in final_paths:
            continue
        final_paths[top.vertex] = top.path

        for to, e in _adj_list_wrapper(adj[top.vertex], edge_to):
            if to in final_paths:
                continue
            heapq.heappush(heap, heap_element(to, sum_pe(top.path, e)))

    return final_paths
