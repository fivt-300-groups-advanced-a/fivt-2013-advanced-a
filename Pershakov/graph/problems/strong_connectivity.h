#ifndef STRONG_CONNECTIVITY_H

#define STRONG_CONNECTIVITY_H

#include <vector>
#include <memory>
#include <stack>
#include <cassert>

#include "../graph.h"
#include "../factory.h"

struct Edge {
    int from, to;

    Edge() {
        from = to = 0;
    }

    Edge(int _from, int _to) : from(_from), to(_to) { }
};

class StrongConnectivity {
    public:
        std::vector<int> getStronglyConnectedComponents(const Graph &gr) {
            int sz = gr.getSize();
            initTarjan(sz);

            for (int i = 0; i < sz; i++)
                if (used[i] == false) {
                    assert(st.size() == 0);
                    dfsTarjan(gr, i);
                }

            return ans;
        }

        Graph buildCondensation(const Graph &gr, const std::vector<int> &comp) {
            int sz = gr.getSize();
            int cnt_comp = 0;
            for (int i = 0; i < sz; i++)
                cnt_comp = std::max(cnt_comp, comp[i] + 1);
            std::vector<std::vector<int> > components(cnt_comp);
            for (int i = 0; i < sz; i++)
                components[comp[i]].push_back(i);

            std::vector<std::unique_ptr<ListOfIncidents> > cond(cnt_comp);
            for (int i = 0; i < cnt_comp; i++) {
                LIFactory fact(cnt_comp);
                for (int j = 0; j < (int)components[i].size(); j++) {
                    const ListOfIncidents& li = gr.getIncidents(components[i][j]);
                    std::unique_ptr<Iterator> it = std::move(li.getIterator());
                    for (; it->getVertex() != -1; it->next()) {
                        int to = it->getVertex();
                        if (comp[to] != i)
                            fact.addEdge(comp[to]);
                    }
                }
                fact.unique();
                cond[i] = std::move(fact.build());
            }
            
            Graph ans(cond);
            return std::move(ans);
        }

        std::vector<Edge> addEdges(const Graph &gr) {
            std::vector<int> comp = getStronglyConnectedComponents(gr);
            Graph cond = std::move(buildCondensation(gr, comp));
            int cnt_comp = cond.getSize();
            std::vector<int> deleg(cnt_comp);
            for (int i = 0; i < gr.getSize(); i++)
                deleg[comp[i]] = i;

            std::vector<Edge> res;
            if (cond.getSize() == 1)
                return res;
            initAddEdges(gr, cond);
            
            int prev_sink = -1;
            for (int i = 0; i < (int)sources.size(); i++) {
                int cur_sink = dfs(cond, sources[i]);
                if (cur_sink != -1) {
                    used_sink[rev_sinks[cur_sink]] = true;
                    used_source[i] = true;
                    if (prev_sink != -1)
                        res.push_back(Edge(deleg[prev_sink], deleg[sources[i]]));
                    prev_sink = cur_sink;
                }
            }
            res.push_back(Edge(deleg[rev_sinks[prev_sink]], deleg[sources[0]]));

            buildBad(bad_sinks, used_sink);
            buildBad(bad_sources, used_source);
            for (int i = 0; i < (int)bad_sinks.size(); i++)
                bad_sinks[i] = sinks[bad_sinks[i]];
            for (int i = 0; i < (int)bad_sources.size(); i++)
                bad_sources[i] = sources[bad_sources[i]];
            for (int i = 0; 
                 i < (int)std::min(bad_sources.size(), bad_sinks.size()); i++) 
                 res.push_back(Edge(deleg[bad_sinks[i]], deleg[bad_sources[i]])); 
            for (int i = std::min(bad_sources.size(), bad_sinks.size()); 
                    i < (int)bad_sources.size(); i++) 
                res.push_back(Edge(deleg[sinks[0]], deleg[bad_sources[i]]));
            for (int i = std::min(bad_sources.size(), bad_sinks.size()); 
                    i < (int)bad_sinks.size(); i++) 
                res.push_back(Edge(deleg[bad_sinks[i]], deleg[sources[0]]));

            assert(res.size() == std::max(sources.size(), sinks.size()));
            return res;
        }

    private:

        std::vector<int> up;
        std::vector<int> tin;
        std::vector<int> ans;
        std::vector<bool> in_stack;
        std::vector<bool> used;
        int cnt_comp;
        int tm;
        std::stack<int> st;
        std::vector<int> sinks;
        std::vector<int> sources;
        std::vector<int> rev_sinks;
        std::vector<bool> used_source;
        std::vector<bool> used_sink;
        std::vector<int> bad_sources;
        std::vector<int> bad_sinks;

        void buildBad(std::vector<int> &to, const std::vector<bool> &used) {
            for (int i = 0; i < (int)used.size(); i++)
                if (!used[i])
                    to.push_back(i);
        }

        void initAddEdges(const Graph &gr, const Graph &cond) {
            int sz = gr.getSize();
            used = std::vector<bool>(sz, false);
            sources = getSources(cond);
            sinks = getSinks(cond);
            rev_sinks = std::vector<int>(sz, 0);
            for (int i = 0; i < (int)sinks.size(); i++)
                rev_sinks[sinks[i]] = i;
            used_source = std::vector<bool>(sources.size(), false);
            used_sink = std::vector<bool>(sinks.size(), false);
        }

        int dfs(const Graph &gr, int v) {
            used[v] = true;
            const ListOfIncidents& li = gr.getIncidents(v);
            auto it = std::move(li.getIterator());
            if (it->getVertex() == -1)
                return v;
            for (; it->getVertex() != -1; it->next()) {
                int to = it->getVertex();
                if (used[to])
                    continue;
                int res = dfs(gr, to);
                if (res != -1)
                    return res;
            }
            return -1;
        }

        void initTarjan(int sz) {
            up = std::vector<int>(sz);
            ans = std::vector<int>(sz);
            used = std::vector<bool>(sz, false);
            tin = std::vector<int>(sz);
            tm = 0;
            in_stack = std::vector<bool>(sz, false);
            cnt_comp = 0;
        }

        void addComponent(int root) {
            while (true) {
                int v = st.top();
                st.pop();
                ans[v] = cnt_comp;
                in_stack[v] = false;
                if (v == root)
                    break;
            }
            cnt_comp++;
        }

        void dfsTarjan(const Graph &gr, int v) {
            const ListOfIncidents& li = gr.getIncidents(v);
            tin[v] = tm;
            up[v] = tm;
            tm++;
            used[v] = true;
            st.push(v);
            in_stack[v] = true;

            auto it = std::move(li.getIterator());
            for (; it->getVertex() != -1; it->next()) {
                int to = it->getVertex();
                if (!used[to]) {
                    dfsTarjan(gr, to);
                    up[v] = std::min(up[v], up[to]);
                } else if (in_stack[to])
                    up[v] = std::min(up[v], tin[to]); 
            }

            if (tin[v] == up[v]) {
                addComponent(v);
            }
        }

        std::vector<int> getSources(const Graph &gr) {
            std::vector<int> ans;
            std::vector<bool> used(gr.getSize());
            for (int i = 0; i < gr.getSize(); i++) {
                const ListOfIncidents& li = gr.getIncidents(i);
                std::unique_ptr<Iterator> it = std::move(li.getIterator());
                for (; it->getVertex() != -1; it->next()) {
                    int to = it->getVertex();
                    used[to] = true;
                }
            }
            for (int i = 0; i < gr.getSize(); i++)
                if (!used[i])
                    ans.push_back(i);
            return ans;
        }

        std::vector<int> getSinks(const Graph &gr) {
            std::vector<int> ans;
            for (int i = 0; i < gr.getSize(); i++) {
                const ListOfIncidents& li = gr.getIncidents(i);
                std::unique_ptr<Iterator> it = std::move(li.getIterator());
                if (it->getVertex() == -1)
                    ans.push_back(i);
            }
            return ans;
        }

};

#endif
