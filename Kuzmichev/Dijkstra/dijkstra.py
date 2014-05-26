from queue import PriorityQueue
import sys, math, random, unittest

class PQElem:
    def __init__(self, key, prior, cmp):
        self.key = key
        self.prior = prior
        self.cmp = cmp
    def __lt__(self, an):
        return self.cmp(self.prior, an.prior)

class Dijkstra:
    def __init__(self, graph, startDist = 0,
                 cmp = lambda firstPath, secondPath : firstPath < secondPath, pathEdgeSum = lambda path, edge : path + edge):
        self.graph = graph
        self.startDist = startDist
        self.cmp = cmp
        self.pathEdgeSum = pathEdgeSum
        self.lastQuery = -1

    def dijkstraN2Inner(self, graph, startVertex, startDist, cmp, pathEdgeSum):
        n = len(graph)
        self.parents = [None] * n
        d = [None] * n
        state = [0] * n
        #0 - not visited, 1 - processing, 2 - finished
        d[startVertex] = startDist
        state[startVertex] = 1
        while (True):
            chosen = -1
            for vertex in range(0, n):
                if ((state[vertex] == 1) and (chosen == -1 or d[vertex] < d[chosen])):
                    chosen = vertex

            if (chosen == -1):
                break
            state[chosen] = 2
            for edge in graph[chosen]:
                (vertexNext, weight) = edge
                newDist = pathEdgeSum(d[chosen], weight)
                if (newDist == None):
                    continue
                if (state[vertexNext] == 0 or cmp(newDist, d[vertexNext])):
                    state[vertexNext] = 1
                    d[vertexNext] = newDist
                    self.parents[vertexNext] = chosen
        return d

    def dijkstraN2(self, startVertex):
        lastQuery = startVertex
        return self.dijkstraN2Inner(self.graph, startVertex, self.startDist, self.cmp, self.pathEdgeSum)

    def dijkstraWithHeapInner(self, graph, startVertex, startDist, cmp, pathEdgeSum):
        n = len(graph)
        self.parents = [None] * n
        dist = [None] * n
        state = [0] * n
        #0 - not visited, 1 - processing, 2 - finished
        dist[startVertex] = startDist
        state[startVertex] = 1
        heap = PriorityQueue()
        heap.put(PQElem(startVertex, startDist, cmp))

        while (not heap.empty()):
            chosenElem = heap.get()
            chosen = chosenElem.key
            if (state[chosen] == 2):
                continue
            state[chosen] = 2
            for edge in graph[chosen]:
                (vertexNext, weight) = edge
                newDist = pathEdgeSum(dist[chosen], weight)
                if (newDist == None):
                    continue
                if (state[vertexNext] == 0 or cmp(newDist, dist[vertexNext])):
                    dist[vertexNext] = newDist
                    state[vertexNext] = 1
                    self.parents[vertexNext] = chosen
                    heap.put(PQElem(vertexNext, newDist, cmp))
        return dist

    def dijkstraWithHeap(self, startVertex):
        lastQuery = startVertex
        return self.dijkstraWithHeapInner(self.graph, startVertex, self.startDist, self.cmp, self.pathEdgeSum)

    def dijkstraInner(self, graph, startVertex, startDist, cmp, pathEdgeSum):
        N = len(graph)
        M = 0
        for i in range(0, N):
            M += len(graph[i])
        operationsWithHeap = M * math.log(N)
        operationsWithN2 = N * N
        if (operationsWithHeap < operationsWithN2):
            return self.dijkstraWithHeapInner(graph, startVertex, startDist, cmp, pathEdgeSum)
        else:
            return self.dijkstraN2Inner(graph, startVertex, startDist, cmp, pathEdgeSum)

    def dijkstra(self, startVertex):
        lastQuery = startVertex
        return self.dijkstraInner(self.graph, startVertex, self.startDist, self.cmp , self.pathEdgeSum)

    def getPath(self, startVertex, finishVertex):
        if (self.lastQuery != startVertex):
            self.dijkstra(startVertex)
        if (self.parents[finishVertex] == None):
            if (finishVertex == startVertex):
                return [startVertex]
            else:
                return None
        result = []
        cur = finishVertex
        while(cur != startVertex):
            result.append(cur)
            cur = self.parents[cur]
        result.append(startVertex)
        return result[::-1]

