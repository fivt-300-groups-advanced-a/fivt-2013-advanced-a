#ifndef GRAPH_H
#define GRAPH_H
#include <memory>
#include <vector>

class IntIterator {
public:
    virtual void operator ++ () = 0;
    virtual int operator * () = 0;   
    virtual bool end() = 0;
    virtual ~IntIterator() {}
};

class ListOfIncidents {
    public:
       virtual std::unique_ptr<IntIterator> begin() = 0;
       virtual void add(int) = 0;
       virtual void remove(int) = 0;
       virtual bool isConnected(int x);
       virtual ~ListOfIncidents() {}
};

class ListFactory {
    public:
        virtual ListOfIncidents* get(int, int) = 0;
        virtual ~ListFactory(){}
};

class AGraph {
public: 
    virtual bool isConnected(int, int);
    virtual void addEdge(int, int) = 0;
    virtual void delEdge(int, int) = 0;
    virtual ListOfIncidents * getIncidents(int) = 0;
    virtual ~AGraph(){}
};

class FGraph : public AGraph {
    std::function<bool(int, int)> pred;
    int n;
public:
    FGraph(std::function<bool(int, int)>, int);
    virtual void addEdge(int, int);
    virtual void delEdge(int, int);
    virtual ListOfIncidents * getIncidents(int);
};

class Graph : public AGraph{
    private:
        std::vector<std::unique_ptr<ListOfIncidents> > lists;
    public:
        Graph(std::unique_ptr<ListFactory>, int);
        virtual void addEdge(int, int);
        virtual void delEdge(int, int);
        virtual ListOfIncidents * getIncidents(int);
};


class GraphWithRevEdges : public Graph{
    public:
        GraphWithRevEdges(std::unique_ptr<ListFactory> ptr, int n);
        virtual void addEdge(int, int);
        virtual void delEdge(int, int);
};

template<class T>
class Const: public ListFactory {
private:
public:
    virtual ListOfIncidents * get(int x, int n) override {
        return new T(n);
    }
};

class Cond: public ListFactory {
private:
    std::unique_ptr<ListFactory> orig;
    std::unique_ptr<ListFactory> nnew;
    std::function<bool (int)> pred;
public:
    Cond(std::unique_ptr<ListFactory>, std::unique_ptr<ListFactory>, std::function<bool (int)>);
    virtual ListOfIncidents * get(int, int);
};

class FunctionalFactory: public ListFactory {
private:
    std::function<bool (int, int)> pred;
    int n;
public:
    FunctionalFactory(std::function<bool (int, int)>);
    virtual ListOfIncidents * get(int, int);
};

class FunctionalList : public ListOfIncidents {
    private:
        std::function<bool (int)> pred;
        int n;
    public:
        class Iterator : public IntIterator {
            private:
                int it, last;
                std::function<bool (int)> pred;
            public:
                virtual void operator ++ ();
                virtual int operator * ();
                virtual bool end();
                Iterator(std::function<bool (int)>, int, int);
        };
        FunctionalList(std::function<bool (int)> pred, int n); 
        virtual std::unique_ptr<IntIterator> begin();
        virtual void add(int);
        virtual void remove(int);
        virtual bool isConnected(int x);
        virtual ~FunctionalList() {}
};

template<class T>
class List : public ListOfIncidents{
};

template<>
class List<std::vector<int> >: public ListOfIncidents {
private:
    std::vector<int> v;
public:
    class Iterator: public IntIterator{
    private:
        friend class List<std::vector<int> >;
        std::vector<int>::iterator it, last;
        Iterator(std::vector<int>::iterator it,
                    std::vector<int>::iterator last); 
    public:
        virtual void operator ++ ();
        virtual int operator * ();
        virtual bool end();
    };
    virtual void add(int) ;
    virtual void remove(int);
    virtual std::unique_ptr<IntIterator> begin();
    List(int);
    List();
};


template<>
class List<std::vector<bool> > : public ListOfIncidents {
private:
    std::vector<bool> v;
    int n;
public:
    class Iterator: public IntIterator{
    private:
        friend class List<std::vector<bool> >;
        std::vector<bool>::iterator it, last;
        Iterator(std::vector<bool>::iterator it, std::vector<bool>::iterator last); 
    public:
        virtual void operator ++ ();
        virtual int operator * ();
        virtual bool end();
    };
    virtual void add(int) ;
    virtual void remove(int);
    virtual bool isConnected(int);
    virtual std::unique_ptr<IntIterator> begin();
    List(int);
};

class OneList : public ListOfIncidents{
private:
    int val;
public:
    class Iterator: IntIterator {
    private:
        bool endv;
        int val;
        friend class OneList;
        Iterator(bool, int);
    public:
        virtual void operator ++ ();
        virtual int operator * ();
        virtual bool end();
    };
    virtual void add(int) ;
    virtual void remove(int);
    virtual bool isConnected(int);
    virtual std::unique_ptr<IntIterator> begin();
    OneList(int);
    OneList();
};

#endif
