#include "graph.h"
#include <algorithm>
#include <cassert>
#include "lists_of_incidents.h"

std::vector<std::unique_ptr<ListOfIncedents> > map(int n, std::function<ListOfIncedents*(int)> pred) {
    std::vector<std::unique_ptr<ListOfIncedents> > vv;
    for (int i = 0; i < n; i++)
        vv.push_back(std::unique_ptr<ListOfIncedents>(pred(i)));
    return vv;
}

namespace {
    std::vector<bool> mark, source, sink;
    int search(int v, const Graph & G) {
        if (mark[v]) return -1;
        mark[v] = true;
        if (sink[v]) return v;
        for (auto i = G.getIncedents(v)->begin(); !i->end(); ++*i) {
            int tmp;
            if ((tmp = search(**i, G)) != -1) {
                return tmp;
            }
        }
        return -1;
    }
    std::vector<int> color, timev;
    std::vector<int> stack;
    int T;
    const int inf = 1e+9;

    int colorComps(int v, const Graph & g, int & cc) {
       if (mark[v]) return timev[v];
       mark[v] = true;
       timev[v] = T++;
       stack.push_back(v);
       int ttime = timev[v];
       for (auto i = g.getIncedents(v)->begin(); !i->end(); ++*i) {
           ttime = std::min(colorComps(**i, g, cc), ttime);
       }
       if (ttime >= timev[v]) {
           while (*(--stack.end()) != v) {
               color[*(--stack.end())] = cc;
               timev[*(--stack.end())] = inf;
               stack.pop_back();
           }
           stack.pop_back();
           color[v] = cc++;
           timev[v] = inf;
           return inf;
       }
       timev[v] = ttime;
       return ttime;
    }
}

std::vector<int> getStrongComponents(const Graph & g) {
    mark.assign(g.size(), false);
    color.assign(g.size(), 0);
    ::timev.assign(g.size(), inf);
    ::T = 0;
    int cc = 1;
    for (int i = 0; i < g.size(); i++) {
        ::colorComps(i, g, cc);
    }
    return ::color;
}

Graph getCondensation(const Graph & g) {
    std::vector<int> cols = getStrongComponents(g);
    int n = 0;
    for (int i = 0; i < cols.size(); i++)
        n = std::max(n, cols[i]);
    std::vector<std::vector<int> > Gtmp(n);
    for (int i = 0; i < g.size(); i++) {
        for (auto j = g.getIncedents(i)->begin(); !j->end(); ++*j) {
            Gtmp[cols[i] - 1].push_back(cols[**j] - 1);
        }
    }
    return Graph(map(n, [&](int i) {
                    return new VectorIntList(std::move(Gtmp[i]));
                }));
}

std::vector<std::pair<int, int> > addEdges(const Graph & g_old) {
   /* std::vector<std::pair<int, int> > x;
    for (int i = 1; i < g_old.size(); ++i) {
        x.emplace_back(0, i);
        x.emplace_back(i, 0);
    }
    return x;*/
    Graph g = getCondensation(g_old);
    sink.assign(g.size(), true);
    source.assign(g.size(), true);
    for (int i = 0; i < g.size(); i++) {
        for (auto j = g.getIncedents(i)->begin(); !j->end(); ++*j) {
            if (i == **j) continue;
            source[**j] = false;
            sink[i] = false;
        }
    }
    mark.assign(g.size(), false);
    std::vector<int> w1, w2;
    int isrc, isnk;
    for (int i = 0; i < g.size(); i++) {
        int tmp;
        if (source[i] && !mark[i] && (tmp = search(i, g)) != -1) {
            isrc = i; isnk = tmp;
            w1.push_back(i);
            source[i] = false;
            w2.push_back(tmp);
            sink[tmp] = false;
        }
    }
    std::vector<int> sources, sinks;
    std::vector<std::pair<int, int> > E;
    for (int i = 0; i < w1.size(); i++) {
        if (w1[i] != w2[(i + 1) % w1.size()]) {
            source[w1[i]] = false;
            sink[w2[(i + 1) % w1.size()]] = false;
            E.push_back(std::make_pair(w2[(i + 1) % w1.size()], w1[i]));
        }
    }
    for (int i = 0; i < g.size(); i++) {
        if (source[i]) sources.push_back(i);
        if (sink[i]) sinks.push_back(i);
    }
    for (int i = 0; i < std::min(sources.size(), sinks.size()); i++) {
        E.push_back(std::make_pair(sources[i], sinks[i]));
    }
    if (sources.size() <= sinks.size()) {
        for (int i = sources.size(); i < sinks.size(); i++) {
            E.push_back(std::make_pair(sinks[i], isrc));
        }
    } else {
        for (int i = sinks.size(); i < sources.size(); i++) {
            E.push_back(std::make_pair(isnk, sources[i]));
        }
    }
    std::vector<int> rev_ind(g.size());
    for (int i = 0; i < color.size(); i++) {
        rev_ind[color[i] - 1] = i;
    }
    for (int i = 0; i < E.size(); i++) {
        E[i] = std::make_pair(rev_ind[E[i].first], rev_ind[E[i].second]);
    }
    return E;
}
