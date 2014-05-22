def dijkstra(g, start, makelen=lambda a, b: a + b[1], getvertex=lambda a: a[0], init=0, key=lambda a: a):
    res = {start: init}
    used = set()
    while len(res) > len(used):
        v = min([(w, q) for q, w in res.items() if q not in used],
                key=lambda a: key(a[0]))[1]
        used.add(v)
        for e in g[v]:
            u = getvertex(e)
            if u in res.keys():
                res[u] = min(res[u], makelen(res[v], e), key=key)
            else:
                res[u] = makelen(res[v], e)
    return res
