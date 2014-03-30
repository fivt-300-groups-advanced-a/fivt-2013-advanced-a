#ifndef MGRAPH
#define MGRAPH

#include <vector>
#include <memory>
#include <stdlib.h>



class ListOfIncidents
{
public:
    class Iterator
    {
    public:
        virtual const Iterator& operator*() const = 0;
        virtual Iterator& operator++ () = 0;
    };

    virtual Iterator* begin() = 0;
    virtual bool isConnected(size_t) const = 0;
    virtual void addEdge(size_t) = 0;
    virtual void removeEdge(size_t) = 0;
    virtual void removeEdge(Iterator *it) = 0;
};

class Factory
{
public:
    virtual ListOfIncidents* getList() = 0;
};
class Graph
{
public:
    void print();
    std::vector<ListOfIncidents*> vertexes;
    
    Graph(Factory* factory) : factory(factory)
    {
        
    }

    Graph(Factory* factory, size_t n) : factory(factory)
    {
        vertexes.assign(n, NULL);
        for (size_t i = 0; i < n; ++i)
            vertexes[i] = factory->getList();
    }
    Graph()
    { }
private:
    Factory *factory;
};



#endif
