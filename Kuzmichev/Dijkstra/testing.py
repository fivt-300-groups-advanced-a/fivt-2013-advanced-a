from queue import PriorityQueue
import sys, math, random, unittest
from dijkstra import Dijkstra
from standard_functions import cmpTrans
from standard_functions import pathEdgeSumTrans

class TestMy(unittest.TestCase):
    def generate(self, n, m, rangeStart, rangeFinish):
        graph = {}
        for i in range(0, n):
            graph[i] = []
        for it in range(0, m):
            fr = random.randint(0, n - 1)
            to = random.randint(0, n - 1)
            weight = random.randint(rangeStart, rangeFinish)
            graph[fr].append((to, weight))
        return graph

    def generateTrans(self, n, m, rangeStart, rangeFinish):
        graph = {}
        for i in range(0, n):
            graph[i] = []
        for it in range(0, m):
            fr = random.randint(0, n - 1)
            to = random.randint(0, n - 1)
            w1 = random.randint(rangeStart, rangeFinish)
            w2 = random.randint(w1, rangeFinish)
            weight = (w1, w2)
            graph[fr].append((to, weight))
        return graph

    def floyd(self, graph, startDist = 0,
              cmp = lambda firstPath, secondPath : firstPath < secondPath, pathEdgeSum = lambda path, edge : path + edge):
        n = len(graph)
        result=[]
        for i in range(0, n):
            row = [None] * n
            for (vertexNext, weight) in graph[i]:
                if (row[vertexNext] == None):
                    row[vertexNext] = weight
                else:
                    if (cmp(weight, row[vertexNext])):
                        row[vertexNext] = weight
            row[i] = startDist
            result.append(row)
        n = len(graph)
        for i in range(0, n):
            for j in range(0, n):
                for k in range(0, n):
                    if (result[j][i] != None and result[i][k] != None):
                        newD = pathEdgeSum(result[j][i], result[i][k])
                        if result[j][k] == None:
                            result[j][k] = newD
                        else:
                            if (cmp(newD, result[j][k])):
                                result[j][k] = newD
        return result

    def fordBellman(self, graph, startVertex, startDist = 0,
                    cmp = lambda firstPath, secondPath : firstPath < secondPath, pathEdgeSum = lambda path, edge : path + edge):
        n = len(graph)
        result = [None] * n
        result[startVertex] = startDist
        while(True):
            changeFlag = False
            for vertex in range(n):
                if (result[vertex] == None):
                    continue
                for edge in graph[vertex]:
                    (nextVertex, weight) = edge
                    newD = pathEdgeSum(result[vertex], weight)
                    if (newD == None):
                        continue
                    if (cmp(newD, result[nextVertex])):
                        changeFlag = True
                        result[nextVertex] = newD
            if (not changeFlag):
                break
        return result

    def funcTest(self, flag, n, m, cmp = lambda firstPath, secondPath : firstPath < secondPath,
                 sum = lambda path, edge : path + edge, rangeStart = 1, rangeFinish = 10000):
        graph = self.generate(n, m, rangeStart, rangeFinish)
        floydResult = self.floyd(graph, 0, cmp , sum )
        D = Dijkstra(graph, 0, cmp, sum)
        for start in range(0, n):
            if (flag == 0):
                dijkstraResult = D.dijkstraN2(start)
            elif (flag == 1):
                dijkstraResult = D.dijkstraWithHeap(start)
            else:
                dijkstraResult = D.dijkstra(start)
            self.assertEqual(dijkstraResult, floydResult[start])

    def funcTestTrans(self, flag, n, m, rangeStart = 1, rangeFinish = 10000):
        graph = self.generateTrans(n, m, rangeStart, rangeFinish)
        D = Dijkstra(graph, 0, cmpTrans, pathEdgeSumTrans)
        for start in range(0, n):
            fordBellmanResult = self.fordBellman(graph, start, 0, cmpTrans, pathEdgeSumTrans)
            if (flag == 0):
                dijkstraResult = D.dijkstraN2(start)
            elif (flag == 1):
                dijkstraResult = D.dijkstraWithHeap(start)
            else:
                dijkstraResult = D.dijkstra(start)
            self.assertEqual(dijkstraResult, fordBellmanResult)


    def funcPathTest(self, n, m, cmp, sum, itCount, rangeStart = 1, rangeFinish = 10000, flagTrans = False):
        graph = (self.generate(n, m, rangeStart, rangeFinish) if not flagTrans else self.generateTrans(n, m, rangeStart, rangeFinish))
        D = Dijkstra(graph, 0, cmp, sum)
        for iter in range(0, itCount):
            s = random.randint(0, n - 1)
            f = random.randint(0, n - 1)
            dist = D.dijkstra(s)
            if (dist[f] != None):
                path = D.getPath(s, f)
                checkResult = self.checkPath(graph, path, 0, cmp, sum, dist)
                if (not checkResult):
                    print("FAIL " + str(s) + " " + str(f))
                self.assertTrue(checkResult)

    def checkPath(self, graph, list, startDist, cmp, sumFunc, dist):
        sum = startDist
        if (list == None or list == []):
           return False
        for i in range(0, len(list) - 1):
            start = list[i]
            finish = list[i + 1]
            bestEdge = None
            for (nextVertex, weight) in  graph[start]:
                if (nextVertex == finish):
                    if (bestEdge == None or cmp(sumFunc(sum, weight), sumFunc(sum, bestEdge))):
                        bestEdge = weight
            if (bestEdge == None):
                return False
            sum = sumFunc(sum, bestEdge)
            if (sum != dist[finish]):
                return False
        return True

    def test_handy_1(self):
        print("test_handy_1")
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
        print("test_handy_2")
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

    def test_1_handy_3(self):
        print("test_handy_3")
        graph = []
        graph.append([(5, 100)])
        graph.append([(0, 2), (2, 3)])
        graph.append([(4, 5), (3, 4)])
        graph.append([(4, 6)])
        graph.append([(5, 2)])
        graph.append([])

        D = Dijkstra(graph, 1, lambda firstPath, secondPath : firstPath > secondPath, lambda path, edge : path * edge)
        res1 = D.dijkstraN2(1)
        #print(res1)
        self.assertEqual(len(res1), 6)
        self.assertEqual(res1[0], 2)
        self.assertEqual(res1[1], 1)
        self.assertEqual(res1[2], 3)
        self.assertEqual(res1[3], 12)
        self.assertEqual(res1[4], 72)
        self.assertEqual(res1[5], 200)

        path1 = D.getPath(1, 5)

        self.assertEqual(path1, [1, 0, 5])

        path2 = D.getPath(1, 4)
        #print(path1)

        self.assertEqual(path2, [1, 2, 3, 4])


    def test_1_handy_trans_1(self):
        print("test_handy_trans_1")
        graph = []
        graph.append([])
        graph.append([(2, (10, 11)), (2, (15, 25))])
        graph.append([(0, (25, 35))])
        graph.append([(1, (0, 11)), (0, (0, 100))])


        D = Dijkstra(graph, 0, cmpTrans, pathEdgeSumTrans)
        res1 = D.dijkstraN2(3)

        #rightD = self.fordBellman(graph, 3, 0, cmpTrans, pathEdgeSumTrans)
        #print(rightD)
        #print(res1)
        self.assertEqual(len(res1), 4)
        self.assertEqual(res1[0], 35)
        self.assertEqual(res1[1], 11)
        self.assertEqual(res1[2], 25)
        self.assertEqual(res1[3], 0)

        path1 = D.getPath(3, 0)
        self.assertEqual(path1, [3, 1, 2, 0])
        path2 = D.getPath(0, 3)
        self.assertEqual(path2, None)
        path3 = D.getPath(1, 0)
        self.assertEqual(path3, [1, 2, 0])
        path4 = D.getPath(0, 2)
        self.assertEqual(path4, None)

        res2 = D.dijkstraWithHeap(3)
        self.assertEqual(res1, res2)

        res3 = D.dijkstraN2(3)
        self.assertEqual(res1, res3)

    def test_2_handy_trans_2(self):
        print("test_handy_trans_2")
        graph = []
        graph.append([(4, (6, 15))])
        graph.append([(5, (0, 0)), (4, (1, 1))])
        graph.append([(0, (0, 10)), (3, (0, 5)), (5, (0, 50))])
        graph.append([(0, (5, 7))])
        graph.append([(2, (100, 100))])
        graph.append([(4, (50, 100))])

        D = Dijkstra(graph, 0, cmpTrans, pathEdgeSumTrans)
        res1 = D.dijkstraN2(2)
        #print(res1)
        self.assertEqual(len(res1), 6)
        self.assertEqual(res1[0], 7)
        self.assertEqual(res1[1], None)
        self.assertEqual(res1[2], 0)
        self.assertEqual(res1[3], 5)
        self.assertEqual(res1[4], 100)
        self.assertEqual(res1[5], 50)

        path1 = D.getPath(2, 4)
        self.assertEqual(path1, [2, 5, 4])


    def test_default_1(self):
        print("test_default_functions_1")
        for it in range(1, 30):
            self.funcTest(2, 10, 100)

    def test_default_2(self):
        print("test_default_functions_2")
        for it in range(1, 30):
            self.funcTest(2, 50, 300)

    def test_1_trans_big(self):
        print("test_trans_big")
        self.funcTestTrans(2, 200, 2000)

    def test_1_trans_big_path_1(self):
        print("test_trans_big_path_1")
        self.funcPathTest(30, 100, cmpTrans, pathEdgeSumTrans, 10, 1, 10000, True)
    def test_1_trans_big_path_2(self):
        print("test_trans_big_path_2")
        self.funcPathTest(50, 200, cmpTrans, pathEdgeSumTrans, 10, 1, 10000, True)

    def test_n2_1(self):
        print("test_O(N^2)_1")
        self.funcTest(0, 100, 2000)
        #self.funcTest(0, 100, 2000, lambda firstPath, secondPath : firstPath < secondPath, lambda path, edge : path + edge)
    def test_n2_2(self):
        print("test_O(N^2)_2")
        self.funcTest(0, 100, 2000, lambda firstPath, secondPath : firstPath < secondPath, lambda x, y : max(x, y))

    def test_heap_1(self):
        print("test_dijkstra_with_heap_1")
        self.funcTest(1, 100, 2000)
        #self.funcTest(1, 100, 2000, lambda firstPath, secondPath : firstPath < secondPath, lambda path, edge : path + edge)
    def test_heap_2(self):
        print("test_dijkstra_with_heap_2")
        self.funcTest(1, 100, 1000, lambda firstPath, secondPath : firstPath < secondPath, lambda x, y : max(x, y))

    def test_dijkstra_big_random_1(self):
        print("test_dijkstra_big_random_1")
        self.funcTest(2, 100, 1000, lambda firstPath, secondPath : firstPath < secondPath, lambda x, y : x * y, 1, 2)
    def test_dijkstra_big_random_2(self):
        print("test_dijkstra_big_random_2")
        self.funcTest(2, 100, 1000, lambda firstPath, secondPath : firstPath < secondPath, lambda x, y : min(x, y))
    def test_dijkstra_big_random_3(self):
        print("test_dijkstra_big_random_3")
        self.funcTest(2, 200, 2000, lambda firstPath, secondPath : firstPath < secondPath, lambda x, y : min(x, y))

    def test_path_big_random_1(self):
        print("test_path_1_big_random")
        self.funcPathTest(70, 1000, lambda firstPath, secondPath : firstPath < secondPath, lambda x, y : x + y, 50)
    def test_path_big_random_2(self):
        print("test_path_2_big_random")
        self.funcPathTest(30, 500, lambda firstPath, secondPath : firstPath < secondPath, lambda x, y : max(x, y), 100)

unittest.main()
