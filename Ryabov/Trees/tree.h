#ifndef MY_TREES
#define MY_TREES
#ifdef TREE_DEBUG
#include <iostream>
#endif

#include <vector>



template <typename Metainf, typename Result, typename Modify, typename Create_data>
class tree
{
private:
    std::vector<Metainf> items;
    int size;

    void build(int v, int l, int r)
    {
        if (l + 1 == r)
            items[v] = Metainf(r - l);
        else
        {
            int m = (l + r) / 2;
            build(v * 2 + 1, l, m);
            build(v * 2 + 2, m, r);
            items[v] = Metainf(r - l);
            items[v].update(&items[v * 2 + 1], &items[v * 2 + 2]);
        }
    }

    void build(int v, int l, int r, std::vector<Create_data> &dt)
    {
        if (l + 1 == r)
            items[v] = Metainf(r - l, dt[l]);
        else
        {
            int m = (l + r) / 2;
            build(v * 2 + 1, l, m, dt);
            build(v * 2 + 2, m, r, dt);
            items[v] = Metainf(r - l);
            items[v].update(&items[v * 2 + 1], &items[v * 2 + 2]);
        }
    }

    int get_len(int r)
    {
        int v = 0;
        int l = 0;
        while (l + 1 < r)
        {
            l = (l + r) / 2;
            v = v * 2 + 2;
        }
        return v + 1;
    }

    void modify(int v, int l, int r, int lq, int rq, Modify mod)
    {
        if (rq <= l || r <= lq)
            return;
        if (lq <= l && r <= rq)
            items[v].modify(mod);
        else
        {
            items[v].push(&items[v * 2 + 1], &items[v * 2 + 2]);
            int m = (l + r) / 2;
            modify(v * 2 + 1, l, m, lq, rq, mod);
            modify(v * 2 + 2, m, r, lq, rq, mod);
            items[v].update(&items[v * 2 + 1], &items[v * 2 + 2]);
        }
    }

    Result get(int v, int l, int r, int lq, int rq)
    {
        if (rq <= l || r <= lq)
            return Result();
        if (lq <= l && r <= rq)
        {
            return items[v].get();
        }

        else
        {
            items[v].push(&items[v * 2 + 1], &items[v * 2 + 2]);
            int m = (l + r) / 2;
            if (lq < m && rq > m)
                return items[v].merge_res(get(v * 2 + 1, l, m, lq, rq), get(v * 2 + 2, m, r, lq, rq));
            if (rq <= m)
                return get(v * 2 + 1, l, m, lq, rq);
            if (lq >= m)
                return get(v * 2 + 2, m, r, lq, rq);
        }
        return Result();
    }

public:
    tree() : size(0)
    {
    }

    #ifdef TREE_DEBUG
    void print()
    {
        int k = 1;
        int cnt = 0;
        for (int i = 0; i < items.size(); i++)
        {
            cnt ++;
            std::cout << "[" << i << "]= " << (items[i].b ? "1" : "0") << " " << items[i].res << "  ";
            if (cnt == k)
            {
                std::cout << "\n";
                cnt = 0;
                k *= 2;
            }
        }
        std::cout << "\n";
    }
    #endif

    tree(int size) : size(size)
    {
        items.resize(get_len(size));
        build(0, 0, size);
    }

    tree(int size, std::vector<Create_data> vec) : size(size)
    {
        items.resize(get_len(size));
        build(0, 0, size, vec);
    }

    void modify(int lq, int rq, Modify mod)
    {
        modify(0, 0, size, lq, rq, mod);
    }

    Result get(int lq, int rq)
    {
        return get(0, 0, size, lq, rq);
    }

};

#endif // MY_TREES
