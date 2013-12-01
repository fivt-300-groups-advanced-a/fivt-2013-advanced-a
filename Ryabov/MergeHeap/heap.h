#ifndef HEAP
#define HEAP

#include <list>
#include <vector>
#include <functional>
#include <cstdlib>
#include <assert.h>
#include <utility>


template <typename DataType, typename Cmp = std::less<DataType> >
class Heap
{
public:

    class Node
    {
    public:
        std::list<Node*> children;
        Node* parent;
        typename std::list<Node*>::iterator listit;
        bool childHasGone;
        DataType value;

        Node(DataType value, Node* parent) : parent(parent), childHasGone(false), value(value)
        {
        }
    };

    class ConstIterator
    {
    public:
        Heap<DataType, Cmp>::Node* it;
    public:
        ConstIterator(Node* it) : it(it)
        {
        }
        ConstIterator() : it(NULL)
        {
        }
    };

    Heap()
    {
        hmin = NULL;
    }

    ConstIterator push(DataType value)
    {
        Node* t = new Node(value, NULL);
        roots.push_back(t);
        auto p = roots.end();
        p--;
        t->listit = p;
        if (hmin == NULL || Cmp()(value, (*hmin).value))
            hmin = *p;
        return ConstIterator(t);
    }

    DataType getMin() const
    {
        assert(hmin != NULL);
        return (*hmin).value;
    }

    bool Empty() const
    {
        return roots.empty();
    }

    DataType pop()
    {
        DataType val = (*hmin).value;
        Erase(hmin);
        return val;
    }

    void Erase(ConstIterator it)
    {
        Erase(it.it);
    }

    void Merge(Heap<DataType, Cmp>& anotherHeap)
    {
        roots.merge(anotherHeap.roots);
        if (anotherHeap.hmin == NULL)
            return;
        if (hmin == NULL)
        {
            hmin = anotherHeap.hmin;
            return;
        }
        if (Cmp()(anotherHeap.hmin->value, hmin->value))
            hmin = anotherHeap.hmin;
    }

#ifdef HEAP_DEBUG
    bool check()
    {
        if (roots.size() == 0)
            return true;
        DataType dt = (*roots.begin())->value;
        for (auto it = roots.begin(); it != roots.end(); it++)
        {
            if (!check(*it))
            {
                std::cout << "check() another check fail!\n";
                return false;
            }
            if (Cmp()((*it)->value, dt))
                dt = (*it)->value;
        }
        if (dt != hmin->value)
        {
            std::cout << "MIN CHECK FAIL\n";
        }
        return (dt == hmin->value);
    }

    bool check(Node* t)
    {
        for (auto it = t->children.begin(); it != t->children.end(); it++)
        {
            if (it != (*it)->listit)
            {
                std::cout << "listit fail\n";
                return false;
            }

            if (!check(*it))
            {
                std::cout << "check(ad) another check(ad) fail\n";
                return false;
            }
            if (Cmp()((*it)->value, t->value))
            {
                std::cout << "Invarian fail\n";
                return false;
            }
        }
        return true;
    }
#endif

private:

    void Erase(Node *t)
    {
        for (typename std::list<Node*>::iterator it = t->children.begin(); it != t->children.end(); ++it)
        {
            roots.push_back(*it);
            auto p = roots.end();
            p--;
            (**it).listit = p;
            (**it).parent = NULL;

        }
        if (t->parent == NULL)
        {
            roots.erase(t->listit);
        }
        else
        {
            t->parent->children.erase(t->listit);
            ChildGo(t->parent);
        }
        delete t;
        update();
    }

    void ChildGo(Node* t)
    {
        if (t == NULL)
            return;
        if (!t->childHasGone)
            t->childHasGone = true;
        else
        {
            t->childHasGone = false;
            t->parent->children.erase(t->listit);
            roots.push_back(t);
            auto p = roots.end();
            p--;
            t->listit = p;
            ChildGo(t->parent);
            t->parent = NULL;
        }
    }

    std::list<Node*> roots;
    Node* hmin;

    void update()
    {
        int maxn = 0;
        for (auto it = roots.begin(); it != roots.end(); it++)
            if (maxn < (int)((*it)->children.size()))
                maxn = (int)((*it)->children.size());
        std::vector<std::list<Node*> > v(maxn + 1);
        for (typename std::list<Node*>::iterator it = roots.begin(); it != roots.end(); ++it)
            v[(**it).children.size()].push_back(*it);
        roots.clear();
        hmin = NULL;
        for (int i = 0; i < (int)v.size(); ++i)
        {
            if (v[i].size() > 1 && i + 1 == (int)v.size())
                v.push_back(std::list<Node*>());
            while (v[i].size() > 1)
            {
                if (Cmp()(v[i].back()->value, v[i].front()->value))
                    std::swap(v[i].back(), v[i].front());
                v[i].back()->parent = v[i].front();
                v[i].front()->children.push_back(v[i].back());
                auto p = v[i].front()->children.end();
                p--;
                v[i].back()->listit = p;
                v[i].pop_back();
                v[i + 1].push_back(v[i].front());
                v[i].pop_front();
            }
            if (v[i].size() == 1)
            {
                roots.push_back(v[i].front());
                auto p = roots.end();
                p--;
                v[i].front()->listit = p;
                v[i].front()->parent = NULL;
                if (hmin == NULL || Cmp()(v[i].front()->value, (*hmin).value))
                    hmin = *p;
            }
        }
    }

};

#endif // HEAP
