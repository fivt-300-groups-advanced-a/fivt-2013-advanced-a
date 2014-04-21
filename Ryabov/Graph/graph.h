#ifndef MGRAPH
#define MGRAPH

#include <vector>
#include <memory>
#include <stdlib.h>



class Incidents
{
public:
    class Iterator
    {
    public:
        virtual const int& operator*() const = 0;
        virtual Iterator& operator++ () = 0;
        virtual ~Iteartor()
        {}
    };

    virtual Iterator* begin() = 0;
    virtual bool isConnected(int) const = 0;
    //virtual void addEdge(size_t) = 0;
    //virtual void removeEdge(size_t) = 0;
    //virtual void removeEdge(Iterator *it) = 0;
    virtual ~Incidents()
    {}
};

class Factory
{
public:
    virtual ListOfIncidents* getList() = 0;
    virtual ~Factory()
    {}
};
class Graph
{
public:
    
    Graph(Factory* factory) : factory(factory)
    {
        
    }

    Graph(std::unique_ptr<Factory> factory, size_t n) : factory(factory)
    {
        vertexes.assign(n, NULL);
        for (size_t i = 0; i < n; ++i)
            vertexes[i] = factory->getList();
    }
    Graph()
    { }
private:
    Factory *factory;
    std::vector<std::unique_ptr< ListOfIncidents > > vertexes;
};



#endif
