from dijkstra import Dijkstra

inf = 10**10

class Town:

    def __init__(self, n, start):
        self.graph = [[] for i in range(n)]
        self.__calc = None
        self.start = start

    def addEdge(self, u, v, tb, te):
        self.graph[u] += ((tb, te), v)

    def __calc__(self):
        if self.__calc is not None: return
        self.__calc = Dijkstra(self.graph)
        def recalc(a, io):
            (tin, tout) = io
            if tin > a: return inf
            else: return tout
        self.__calc = Dijkstra(self.graph, start=self.start, path_rec=recalc)

    def getPath(self, v):
        self.__calc__()
        return self.__calc.getShortestPath(v)

    def getTimes(self):
        self.__calc__()
        return self.getDistList()

    def getTime(self, v):
        self.__calc__()
        return self.getTime(v)
