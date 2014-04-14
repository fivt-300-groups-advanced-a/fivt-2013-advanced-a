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
       virtual bool isConnected(int x) = 0;
       virtual ~ListOfIncidents() {}
};

class Graph {
    private:
        std::vector<std::unique_ptr<ListOfIncidents> > lists;
    public:
        explicit Graph(std::vector<std::unique_ptr<ListOfIncidents> > s) {
            lists = std::move(s);
        }
        virtual ListOfIncidents * getIncidents(int i) {
            return lists[i].get();
        }
        virtual bool isConnected(int x, int y) {
            return getIncidents(x)->isConnected(y);
        }
        virtual ~Graph() {}
};

#endif
