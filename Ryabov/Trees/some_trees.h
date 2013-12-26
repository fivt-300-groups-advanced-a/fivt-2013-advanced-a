#ifndef MY_TREE_TEMPLATES
#define MY_TREE_TEMPLATES
template <typename T, typename Oper>
struct tree_add
{
public:
    T delt;
    T res;
    bool b;

    void modify(T k)
    {
        if (b)
            delt += k;
        else
        {
            b = true;
            delt = k;
        }
        res += k;
    }

    void push(tree_add<T, Oper> *l, tree_add<T, Oper> *r)
    {
        b = false;
        l -> modify(delt);
        r -> modify(delt);
    }
    void update(tree_add<T, Oper> *l, tree_add<T, Oper> *r)
    {
        res = Oper()(l -> res, r -> res);
        if (b)
            res += delt;
    }

    T get()
    {
        return res;
    }

    T merge_res(T a, T b)
    {
        return Oper()(a, b);
    }

    tree_add(int len)
    {
        delt = T();
        res = T();
        b = false;
    }
    tree_add()
    {

    }
};


template <typename T, typename Oper>
struct tree_def
{
public:
    T delt;
    T res;
    bool b;

    void modify(T k)
    {
        b = true;
        delt = k;
        res = k;
    }

    void push(tree_def<T, Oper> *l, tree_def<T, Oper> *r)
    {
        if (b)
        {
            b = false;
            l -> modify(delt);
            r -> modify(delt);
        }
    }

    void update(tree_def<T, Oper> *l, tree_def<T, Oper> *r)
    {
        res = Oper()(l -> res, r -> res);
        if (b)
            res = delt;
    }

    T get()
    {
        return res;
    }

    T merge_res(T a, T b)
    {
        return Oper()(a, b);
    }

    tree_def(int len)
    {
        delt = T();
        res = T();
        b = false;
    }

    tree_def()
    {

    }
};

template <typename T>
struct tree_def_sum
{
public:
    T delt;
    T res;
    int len;
    bool b;

    void modify(T k)
    {
        b = true;
        delt = k;
        res = k * len;
    }

    void push(tree_def_sum<T> *l, tree_def_sum<T> *r)
    {
        if (b)
        {
            b = false;
            l -> modify(delt);
            r -> modify(delt);
        }
    }

    void update(tree_def_sum<T> *l, tree_def_sum<T> *r)
    {
        res = (l -> res + r -> res);
        if (b)
            res = delt * len;
    }

    T get()
    {
        return res;
    }

    T merge_res(T a, T b)
    {
        return (a + b);
    }

    tree_def_sum(int len) : len(len)
    {
        delt = T();
        res = T();
        b = false;
    }

    tree_def_sum(int len, int q) : len(len)
    {
        delt = q;
        res = q;
        b = false;
    }

    tree_def_sum()
    {

    }
};

template <typename T>
struct tree_add_sum
{
public:
    T delt;
    T res;
    int len;
    bool b;

    void modify(T k)
    {
        if (b)
            delt += k;
        else
        {
            b = true;
            delt = k;
        }
        res += k * len;
    }

    void push(tree_add_sum<T> *l, tree_add_sum<T> *r)
    {
        b = false;
        l -> modify(delt);
        r -> modify(delt);
    }
    void update(tree_add_sum<T> *l, tree_add_sum<T> *r)
    {
        res = l -> res + r -> res;
        if (b)
            res += delt * len;
    }

    T get()
    {
        return res;
    }

    T merge_res(T a, T b)
    {
        return (a + b);
    }

    tree_add_sum(int len) : len(len)
    {
        delt = T();
        res = T();
        b = false;
    }
    tree_add_sum()
    {

    }
};

#ifdef TREE_DEBUG

int debug_k = 0;

struct fake_tree
{
public:
    void modify(int k)
    {
        debug_k++;
    }

    void push(fake_tree *l, fake_tree *r)
    {
        debug_k++;
    }
    void update(fake_tree *l, fake_tree *r)
    {
        debug_k++;
    }

    int get()
    {
        return 0;
    }

    int merge_res(int a, int b)
    {
        return (0);
    }

    fake_tree(int len)
    {

    }
    fake_tree()
    {

    }
};


#endif

#endif // MY_TREE_TEMPLATES
