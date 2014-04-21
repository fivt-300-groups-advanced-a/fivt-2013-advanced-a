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

class ListOfIncedents {
    public:
       virtual std::unique_ptr<IntIterator> begin() = 0;
       virtual bool isConnected(int x) = 0;
       virtual ~ListOfIncedents() {}
};

class Graph {
    private:
        std::vector<std::unique_ptr<ListOfIncedents> > lists;
    public:
        virtual int size() const {
            return lists.size();
        }
        explicit Graph(std::vector<std::unique_ptr<ListOfIncedents> > s) {
            lists = std::move(s);
        }
        virtual ListOfIncedents * getIncedents(int i) const {
            return lists[i].get();
        }
        virtual bool isConnected(int x, int y) const {
            return getIncedents(x)->isConnected(y);
        }
        virtual ~Graph() {}
        Graph(Graph && g): lists(std::move(g.lists)) {
        }
};

std::vector<int> getStrongComponents(const Graph &);
Graph getCondensation(const Graph &);
std::vector<std::pair<int, int> > addEdges(const Graph &);
std::vector<std::unique_ptr<ListOfIncedents> > map(int n, std::function<ListOfIncedents*(int)> pred);

#endif
