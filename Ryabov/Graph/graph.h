#ifndef MGRAPH
#define MGRAPH
#include <bitset>
#include <vector>
#include <memory>
#include <stdlib.h>
#include <algorithm>


class Incidents
{
public:
    class Iterator
    {
    public:
        virtual int get() = 0;
        virtual void next() = 0;
        virtual ~Iterator()
        {}
    };

    virtual std::unique_ptr<Iterator> begin() = 0;
    virtual bool isConnected(int) = 0;
    virtual ~Incidents()
    {}
};

class Graph
{
public:

    int size()
    {
        return vertexes.size();
    }

    Graph(std::vector<std::unique_ptr<Incidents> > &v)
    {
        for (int i = 0; i < v.size(); ++i)
            vertexes.push_back(std::move(v[i]));
    }

    Incidents& operator[](int i)
    {
        return *vertexes[i];
    }

private:

    std::vector<std::unique_ptr< Incidents > > vertexes;
};

//------------------------------------------
//----------IMPLEMENTIONS-------------------
//------------------------------------------



class BitsetIncidents : public Incidents
{
public:
    class BitsetIterator : public Iterator
    {
        public:
            int get()
            {
                while (it < end && !(*it))
                    it++;
                if (it == end)
                    return -1;
                return it - begin;
            }

            void next()
            {
                while (it < end && !(*it))
                    it++;
                if (it < end)
                    it++;
            }
            BitsetIterator(std::vector<bool>::iterator begin, std::vector<bool>::iterator end) : begin(begin), end(end), it(begin)
            {
            }
        private:
            std::vector<bool>::iterator it;
            std::vector<bool>::iterator begin;
            std::vector<bool>::iterator end;
    };


    std::unique_ptr<Iterator> begin()
    {
        return std::move(std::unique_ptr<Iterator>(new BitsetIterator(mlist.begin(), mlist.end())));
    }

    bool isConnected(int v)
    {
        return mlist[v];
    }

    BitsetIncidents(std::vector<bool> &v) : mlist(v)
    {
    }

private:
    std::vector<bool> mlist;
};


class BitsetFactory
{
private:
    std::vector<bool> v;

public:

    BitsetFactory(int n) : v(n, false)
    {
    }

    void addEdge(int q)
    {
        v[q] = true;
    }

    std::unique_ptr<Incidents> get()
    {
        return std::move(std::unique_ptr<Incidents>(new BitsetIncidents(v)));
    }
};

class OneIncidents : public Incidents
{
public:
    class OneIterator : public Iterator
    {
    public:
        int get()
        {
            return val;
        }
        void next()
        {
            val = -1;
        }
        OneIterator(int val) : val(val)
        {
        }
    private:
        int val;
    };
    std::unique_ptr<Iterator> begin()
    {
        return std::move(std::unique_ptr<Iterator>(new OneIterator(val)));
    }

    bool isConnected(int v)
    {
        if (v == val && val != -1)
            return true;
        return false;
    }

    OneIncidents(int val) : val(val)
    {
    }

private:
    int val;
};

class OneFactory
{
private:
    int val = -1;
public:
    void addEdge(int v)
    {
        val = v;
    }
    std::unique_ptr<Incidents> get()
    {
        return std::move(std::unique_ptr<Incidents>(new OneIncidents(val)));
    }
};

class ListIncidents : public Incidents
{
public:
    class ListIterator : public Iterator
    {
    public:
        int get()
        {
            if (it >= end)
                return -1;
            return *it;
        }

        void next()
        {
            if (it < end)
                it++;
        }

        ListIterator(std::vector<int>::iterator begin, std::vector<int>::iterator end) : end(end), it(begin)
        {
        }
    private:
        std::vector<int>::iterator end, it;
    };
    std::unique_ptr<Iterator> begin()
    {
        return std::move(std::unique_ptr<Iterator>(new ListIterator(mlist.begin(), mlist.end())));
    }
    bool isConnected(int v)
    {
        auto it = std::lower_bound(mlist.begin(), mlist.end(), v);
        return (it < mlist.end() && *it == v);
    }

    ListIncidents(std::vector<int> &v): mlist(v)
    {
        sort(mlist.begin(), mlist.end());
    }
private:
    std::vector<int> mlist;
};

class ListFactory
{
private:
    std::vector<int> v;
public:
    void addEdge(int q)
    {
        v.push_back(q);
    }
    std::unique_ptr<Incidents> get()
    {
        return std::move(std::unique_ptr<Incidents>(new ListIncidents(v)));
    }
};

class CleverFactory
{
public:
    CleverFactory(int n) : n(n), q(false)
    {
    }
    
    void addEdge(int v)
    {
        if (q)
            vbool[v] = true;
        else
        {
            vint.push_back(v);
            if (vint.size() * 32 > n)
            {
                q = true;
                vbool.assign(n, false);
                for (auto i = vint.begin(); i != vint.end(); ++i)
                    vbool[*i] = true;
                vint.clear();
            }
        }
    }

    std::unique_ptr<Incidents> get()
    {
        if (q)
            return std::move(std::unique_ptr<Incidents>(new BitsetIncidents(vbool)));
        return std::move(std::unique_ptr<Incidents>(new ListIncidents(vint)));
    }
private:
    bool q;
    int n;
    std::vector<int> vint;
    std::vector<bool> vbool;
};
#endif
