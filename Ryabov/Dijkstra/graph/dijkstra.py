from queue import PriorityQueue

class PQelem:
    def __init__(self, vertex, way, cmp):
        self.way = way
        self.vertex = vertex
        self.cmp = cmp
    def __lt__(self, right):
        return self.cmp(self.way, right.way)


def dijkstra(g, start, makelen=lambda waylen, edge, outvertex: waylen + edge[1], getvertex=lambda edge: edge[0], init=0, cmp=lambda way1, way2: way1 < way2):
    res = {start: init}
    used = set()
    pq = PriorityQueue()
    pq.put(PQelem(start, init, cmp))
    while len(res) > len(used):
        vertex = pq.get().vertex
        if vertex in used:
            continue
        used.add(vertex)
        for edge in g[vertex]:
            u = getvertex(edge)
            if u in res:
                if (cmp(makelen(res[vertex], edge, vertex), res[u])):
                    res[u] = makelen(res[vertex], edge, vertex)
                    pq.put(PQelem(u, res[u], cmp))
            else:
                res[u] = makelen(res[vertex], edge, vertex)
                pq.put(PQelem(u, res[u], cmp))
    return res

def getpath(res, vertex):
    ans = []
    while vertex != None:
        ans.append(vertex)
        vertex = res[vertex][1]
    ans.reverse()
    return ans


def patheddijkstra(g, start, makelen=lambda waylen, edge, outvertex: waylen + edge[1], getvertex=lambda edge: edge[0], init=0, cmp=lambda way1, way2: way1 < way2):
    '''
        Interface the same with dijkstra, but it returns dict of vertex -> tuple(waylen, previous vertex)
    '''
    return dijkstra(g, start, lambda way, edge, vertex:(makelen(way[0], edge, vertex), vertex), getvertex=getvertex, init=(init, None), cmp=lambda way1, way2: cmp(way1[0], way2[0]))


def tableproblem(g, start):
    '''
        Graph is dict(vertex -> edge), where edge is tuple(to_vertex, start_time, end_time)
    '''
    return patheddijkstra(g, start, makelen=lambda way,edge,vertex: 10 ** 10 if way > edge[1] else edge[2], init=0)
