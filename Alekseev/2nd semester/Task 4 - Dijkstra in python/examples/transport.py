from collections import namedtuple
from dijkstra.dijkstra import Dijkstra

class TransportScheduler:
    """
    >>> ts = TransportScheduler([1, 2, 3])
    >>> ts.add_route(1, 2, 0, 4)
    >>> ts.add_route(1, 2, 1, 3)
    >>> ts.add_route(2, 3, 3, 5)
    >>> ts.add_route(2, 3, 5, 10)
    >>> ts.add_route(2, 3, 2, 3)
    >>> ts.find_way(1, 3)
    [Route(from_v=1, to_v=2, time_from=1, time_to=3), Route(from_v=2, to_v=3, time_from=3, time_to=5)]

    """
    Route = namedtuple('Route', ['from_v', 'to_v', 'time_from', 'time_to'])

    def __init__(self, vertices):
        """
        :param vertices_list - list of vertices of graph
        """
        self.graph = {v: [] for v in vertices}

    def add_route(self, from_v, to_v, time_from, time_to):
        """
        :param from_v starting point of route
        :param to_v ending point of route
        :param time_from start time of route
        :param time_to end time of route
        :rtype None
        :return None
        """
        if from_v not in self.graph:
            raise KeyError("from_v does not exist")
        if to_v not in self.graph:
            raise KeyError("to_v does not exist")
        self.graph[from_v].append(TransportScheduler.Route(from_v, to_v, time_from, time_to))

    def find_way(self, from_vertex, to_vertex, start_time=0):
        """
        Calls Dijkstra with constructed graph
        """
        dij = Dijkstra(self.graph, from_vertex,
                       start_p=start_time,
                       edge_to=lambda e: e.to_v,
                       sum_pev=lambda p, e, _: None if e.time_from < p else e.time_to,
                       backtrace=True).trace_to(to_vertex)
        return [r for v, r, p in dij if r is not None]
