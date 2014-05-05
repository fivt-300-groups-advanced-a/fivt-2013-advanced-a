#ifndef MALG
#define MALG

#include "graph.h"
#include <vector>
#include <stack>

void TarjanDfs(Graph &g, int v, std::vector<int> &color, int &cur_color, int &timer, std::vector<int> &tin, std::vector<int> &f, std::stack<int> &st)
{
    st.push(v);
    f[v] = tin[v] = ++timer;
    auto it = g[v].begin();
    int u;
    while (-1 != (u = it->get()))
    {
        if (color[u] == -1)
        {
            if (tin[u] == -1)
                TarjanDfs(g, u, color, cur_color, timer, tin, f, st);
            f[v] = std::min(f[v], f[u]);
        }
        it->next();
    }
    if (f[v] == tin[v])
    {
        do
        {
            u = st.top();
            st.pop();
            color[u] = cur_color;
        } while (u != v);
        cur_color++;
    }
}

int getStrongCon(Graph &g, std::vector<int> &ans)
{
    ans.assign(g.size(), -1);
    int cur_color = 0;
    int timer = 0;
    std::stack<int> st;
    std::vector<int> f(g.size(), -1), tin(g.size(), -1);
    for (int i = 0; i < g.size(); ++i)
        if (ans[i] == -1)
            TarjanDfs(g, i, ans, cur_color, timer, tin, f, st);
    return cur_color;
}

int some_dfs(Graph &g, int v, std::vector<char> &used, std::vector<char> &has_output, std::vector<int> &colors)
{
    if (used[v])
        return -1;
    used[v] = 1;
    if (!has_output[colors[v]])
        return v;
    int q = -1;
    auto it = g[v].begin();
    int u;
    while (-1 != (u = it->get()))
    {
        if ((q = some_dfs(g, u, used, has_output, colors) != -1))
                return q;
        it->next();
    }
    return -1;
}


std::vector<std::pair<int, int>> StrongConnectivityAugmentation(Graph &g)
{
    std::vector<std::pair<int, int> > ans;
    std::vector<int> colors;
    int max_color = getStrongCon(g, colors);
    if (max_color <= 1)
        return ans;
    std::vector<char> has_input(max_color, 0), has_output(max_color, 0);
    for (int i = 0; i < g.size(); ++i)
    {
        auto it = g[i].begin();
        while (it->get() != -1)
        {
            int u = it->get();
            if (colors[i] != colors[u])
                has_output[colors[i]] = has_input[colors[u]] = 1;
            it->next();
        }
    }
#ifdef TESTING
    int qres = 0;
    int wres = 0;
    for (int i = 0; i < max_color; ++i)
    {
        if (!has_input[i])
            qres++;
        if (!has_output[i])
            wres++;
    }
#endif
    std::vector<char> used(g.size(), 0);
    int tmp = -1, btmp = -1;
    for (int i = 0; i < g.size(); ++i)
    {
        if (!has_input[colors[i]])
        {
            int q = some_dfs(g, i, used, has_output, colors);
            if (q != -1)
            {
                if (tmp != -1)
                {
                    ans.push_back(std::make_pair(tmp, i));
                    tmp = q;
                }
                else
                {
                    btmp = i;
                    tmp = q;
                }
                has_input[colors[i]] = 1;
                has_output[colors[q]] = 1;
            }
        }
    }
    ans.push_back(std::make_pair(tmp, btmp));
    tmp = btmp = 0;
    while (tmp < g.size() || btmp < g.size())
    {
        while (tmp < g.size() && has_input[colors[tmp]])
            tmp++;
        while (btmp < g.size() && has_output[colors[btmp]])
            btmp++;
        if (tmp < g.size() && btmp < g.size())
        {
            ans.push_back(std::make_pair(btmp, tmp));
            has_input[colors[tmp]] = 1;
            has_output[colors[btmp]] = 1;
            tmp++;
            btmp++;
        }
        else if (tmp < g.size())
        {
            ans.push_back(std::make_pair(tmp, ans.back().second));
            has_input[colors[tmp]] = 1;
            tmp++;
        }
        else if (btmp < g.size())
        {
            ans.push_back(std::make_pair(ans.back().first, btmp));
            has_output[colors[btmp]] = 1;
            btmp++;
        }
    }
#ifdef TESTING
    EXPECT_EQ(std::max(qres, wres), ans.size());
#endif
    return ans;
}

#endif
