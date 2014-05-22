from dijkstra import Dijkstra

inf = 10**10


class Transport:

    def __init__(self, n):
        """
        :param n: quantity of stations
        """
        self.graph = [[] for i in range(n)]
        self.__calc = None

    def addEdge(self, u, v, tb, te):
        """
        adds edge from u in time tb and arrives v in time te
        """
        self.graph[u] += [((tb, te), v)]

    def calc(self, start, start_time=0):
        def add_edge(time, edge):
            (tin, tout) = edge
            if tin < time:
                return inf
            else:
                return tout
        self.__calc = Dijkstra(self.graph, start=start, path_rec=add_edge, init_id=start_time)

    def getPath(self, v):
        return self.__calc.getShortestPath(v)

    def getTimes(self):
        res = {}
        for (k, v) in self.__calc.dists.items():
            if v != inf:
                res[k] = v
        return res

    def getTime(self, v):
        return self.getTime(v)
