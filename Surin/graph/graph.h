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

class Graph{
    private:
        std::vector<std::unique_ptr<ListOfIncidents> > lists;
    public:
        Graph(std::unique_ptr<ListFactory>, int);
        void addEdge(int, int);
        void delEdge(int, int);
        ListOfIncidents * getIncidents(int);
};

template<class T>
class Const: ListFactory {
private:
    int n;
public:
    Const(int _n): n(_n){
    }
    virtual ListOfIncidents * get(int x) override {
        return new T(n);
    }
};

class Cond: ListFactory {
private:
    std::unique_ptr<ListFactory> orig;
    std::unique_ptr<ListFactory> nnew;
    std::function<bool (int)> pred;
public:
    Cond(std::unique_ptr<ListFactory>, std::unique_ptr<ListFactory>, std::function<bool (int)>);
    virtual ListOfIncidents * get(int, int);
};


template<class T>
class List : ListOfIncidents{
};

template<>
class List<std::vector<int> > {
private:
    std::vector<int> v;
public:
    class Iterator: IntIterator{
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
    List(int);
};


template<>
class List<std::vector<bool> > {
private:
    std::vector<int> v;
public:
    class Iterator: IntIterator{
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
    List(int);
};

#endif
