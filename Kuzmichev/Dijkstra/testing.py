from queue import PriorityQueue
import sys, math, random, unittest
from dijkstra import Dijkstra

class TestMy(unittest.TestCase):
    def generate(self, n, m, s, f):
        graph = {}
        for i in range(0, n):
            graph[i] = []
        for it in range(0, m):
            fr = random.randint(0, n - 1)
            to = random.randint(0, n - 1)
            w = random.randint(s, f)
            graph[fr].append((to, w))
        return graph

    def floyd(self, graph, startDist = 0, cmp = lambda x, y : x < y, distEdgeSum = lambda x, y : x + y):
        n = len(graph)
        result=[]
        for i in range(0, n):
            row = [None] * n
            for (a, b) in graph[i]:
                if (row[a] == None):
                    row[a] = b
                else:
                    if (cmp(b, row[a])):
                        row[a] = b
            row[i] = startDist
            result.append(row)
        n = len(graph)
        for i in range(0, n):
            for j in range(0, n):
                for k in range(0, n):
                    if (result[j][i] != None and result[i][k] != None):
                        newD = distEdgeSum(result[j][i], result[i][k])
                        if result[j][k] == None:
                            result[j][k] = newD
                        else:
                            if (cmp(newD, result[j][k])):
                                result[j][k] = newD
        return result

    def funcTest(self, flag, n, m, cmp = lambda x, y : x < y, sum = lambda x, y : x + y, s = 1, f = 10000):
        graph = self.generate(n, m, s, f)
        floydResult = self.floyd(graph, 0, cmp , sum )
        #for i in range(0, n):
         #   print("i = " + str(i))
        #    print(floydResult[i])
        D = Dijkstra(graph, 0, cmp, sum)
        for start in range(0, n):
            if (flag == 0):
                dijkstraResult = D.dijkstraN2(start)
            elif (flag == 1):
                dijkstraResult = D.dijkstraWithHeap(start)
            else:
                dijkstraResult = D.dijkstra(start)
            self.assertEqual(dijkstraResult, floydResult[start])


    def funcPathTest(self, n, m, cmp, sum, itCount, s = 1, f = 10000):
        graph = self.generate(n, m, s, f)
        D = Dijkstra(graph, 0, cmp, sum)
        for iter in range(0, itCount):
            s = random.randint(0, n - 1)
            f = random.randint(0, n - 1)
            dist = D.dijkstra(s)
            path = D.getPath(s, f)
            checkResult = self.checkPath(graph, path, 0, cmp, sum, dist)
            self.assertTrue(checkResult)

    def checkPath(self, graph, list, startDist, cmp, sumFunc, dist):
        sum = startDist
        for i in range(0, len(list) - 1):
            s = list[i]
            f = list[i + 1]
            ch = None
            for (a, b) in  graph[s]:
                if (a == f):
                    if (ch == None or cmp(sumFunc(sum, b), sumFunc(sum, ch))):
                        ch = b
            if (ch == None):
                return False
            sum = sumFunc(sum, ch)
            if (sum != dist[f]):
                return False
        return True

    def test_handy_1(self):
        graph = []
        graph.append([(2, 20), (3, 1)])
        graph.append([(2, 5), (1, 6)])
        graph.append([])
        graph.append([(3, 5), (2, 1)])

        D = Dijkstra(graph, 100)
        res1 = D.dijkstraN2(0)
        #print(res)
        self.assertEqual(len(res1), 4)
        self.assertEqual(res1[0], 100)
        self.assertEqual(res1[1], None)
        self.assertEqual(res1[2], 102)
        self.assertEqual(res1[3], 101)

        path1 = D.getPath(0, 2)
        self.assertEqual(path1, [0, 3, 2])

        res2 = D.dijkstraWithHeap(0)

        self.assertEqual(res1, res2)

        path2 = D.getPath(0, 2)
        self.assertEqual(path1, path2)

        res3 = D.dijkstra(0)
        self.assertEqual(res1, res3)

        path3 = D.getPath(0, 2)
        self.assertEqual(path1, path3)

    def test_handy_2(self):
        graph = []
        graph.append([(2, 20), (3, 1)])
        graph.append([])
        graph.append([(3, 5), (2, 6)])
        graph.append([(1, 5), (2, 1)])

        D = Dijkstra(graph, 100)
        res1 = D.dijkstraN2(2)
       # print(res1)
        self.assertEqual(len(res1), 4)
        self.assertEqual(res1[0], None)
        self.assertEqual(res1[1], 110)
        self.assertEqual(res1[2], 100)
        self.assertEqual(res1[3], 105)

        path1 = D.getPath(1, 3)
        #print(path1)

        self.assertEqual(path1, None)

        path2 = D.getPath(2, 1)
        self.assertEqual(path2, [2, 3, 1])

        res2 = D.dijkstraWithHeap(2)
        self.assertEqual(res1, res2)

        path3 = D.getPath(2, 1)
        self.assertEqual(path2, path3)

        res3 = D.dijkstra(2)
        self.assertEqual(res1, res3)

        path4 = D.getPath(2, 1)
        self.assertEqual(path2, path4)

        path5 = D.getPath(0, 2)
        self.assertEqual(path5, [0, 3, 2])

    def test_default(self):
        for it in range(1, 30):
            self.funcTest(2, 10, 100)



    def test_n2_1(self):
        self.funcTest(0, 100, 2000, lambda x, y : x < y, lambda x, y : x + y)
    def test_n2_2(self):
        self.funcTest(0, 100, 2000, lambda x, y : x < y, lambda x, y : max(x, y))
    def test_heap_1(self):
        self.funcTest(1, 100, 2000, lambda x, y : x < y, lambda x, y : x + y)
    def test_heap_2(self):
        self.funcTest(1, 100, 1000, lambda x, y : x < y, lambda x, y : max(x, y))

    def test_d_1(self):
        self.funcTest(2, 100, 1000, lambda x, y : x < y, lambda x, y : x * y, 1, 2)
    def test_d_2(self):
        self.funcTest(2, 100, 1000, lambda x, y : x < y, lambda x, y : min(x, y))
    def test_d_3(self):
        self.funcTest(2, 200, 2000, lambda x, y : x < y, lambda x, y : min(x, y))

    def test_path_1(self):
        self.funcPathTest(100, 2000, lambda x, y : x < y, lambda x, y : max(x, y), 2)
    def test_path_2(self):
        self.funcPathTest(50, 1000, lambda x, y : x < y, lambda x, y : max(x, y), 10)

unittest.main()
