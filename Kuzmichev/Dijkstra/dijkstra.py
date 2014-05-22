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
    def __init__(self, graph, startDist = 0, cmp = lambda x, y : x < y, distEdgeSum = lambda x, y : x + y):
        self.graph = graph
        self.startDist = startDist
        self.cmp = cmp
        self.distEdgeSum = distEdgeSum
        self.lastQuery = -1

    def dijkstraN2Inner(self, graph, startVertex, startDist = 0, cmp = lambda x, y : x < y, distEdgeSum = lambda x, y : x + y):
        n = len(graph)
        self.parents = [None] * n
        d = [None] * n
        state = [0] * n
        #0 - not visited, 1 - processing, 2 - finished
        d[startVertex] = startDist
        state[startVertex] = 1
        while (True):
            chosen = -1
            for i in range(0, n):
                if ((state[i] == 1) and (chosen == -1 or d[i] < d[chosen])):
                    chosen = i
            if (chosen == -1):
                break
            state[chosen] = 2
            for e in range(0, len(graph[chosen])):
                (v, w) = graph[chosen][e]
                newDist = distEdgeSum(d[chosen], w)
                if (state[v] == 0 or cmp(newDist, d[v])):
                    state[v] = 1
                    d[v] = newDist
                    self.parents[v] = chosen
        return d

    def dijkstraN2(self, startVertex):
        lastQuery = startVertex
        return self.dijkstraN2Inner(self.graph, startVertex, self.startDist, self.cmp, self.distEdgeSum)

    def dijkstraWithHeapInner(self, graph, startVertex, startDist = 0, cmp = lambda x, y : x < y, distEdgeSum = lambda x, y : x + y):
        n = len(graph)
        self.parents = [None] * n

        d = [None] * n
        state = [0] * n
        #0 - not visited, 1 - processing, 2 - finished
        d[startVertex] = startDist
        state[startVertex] = 1
        heap = PriorityQueue()
        heap.put(PQElem(startVertex, startDist, cmp))
        #heapq.heappush(my_heap, (startDist, startVertex))
        while (not heap.empty()):
            chosenElem = heap.get()
            chosen = chosenElem.key
            if (state[chosen] == 2):
                continue
            state[chosen] = 2
            for e in range(0, len(graph[chosen])):
                (v, w) = graph[chosen][e]
                newDist = distEdgeSum(d[chosen], w)
                if (state[v] == 0 or cmp(newDist, d[v])):
                    d[v] = newDist
                    state[v] = 1
                    self.parents[v] = chosen
                    heap.put(PQElem(v, newDist, cmp))
        return d
        #for j in range(0, n):
        #    sys.stdout.write((str(d[j]) if state[j] != 0 else "2009000999") + " ")
        #print("")

    def dijkstraWithHeap(self, startVertex):
        lastQuery = startVertex
        return self.dijkstraWithHeapInner(self.graph, startVertex, self.startDist, self.cmp, self.distEdgeSum)

    def dijkstraInner(self, graph, startVertex, startDist = 0, cmp = lambda x, y : x < y, distEdgeSum = lambda x, y : x + y):
        N = len(graph)
        M = 0
        for i in range(0, N):
            M += len(graph[i])
        operationsWithHeap = M * math.log(N)
        operationsWithN2 = N * N
        #print(operationsWithHeap)
        #print(operationsWithN2)
        if (operationsWithHeap < operationsWithN2):
            return self.dijkstraWithHeapInner(graph, startVertex, startDist, cmp, distEdgeSum)
        else:
            return self.dijkstraN2Inner(graph, startVertex, startDist, cmp, distEdgeSum)

    def dijkstra(self, startVertex):
        lastQuery = startVertex
        return self.dijkstraInner(self.graph, startVertex, self.startDist, self.cmp , self.distEdgeSum)

    def getPath(self, startVertex, finishVertex):
        if (self.lastQuery != startVertex):
            self.dijkstra(startVertex)
        if (self.parents[finishVertex] == None):
            return None
        result = []
        cur = finishVertex
        while(cur != startVertex):
            result.append(cur)
            cur = self.parents[cur]
        result.append(startVertex)
        return result[::-1]

