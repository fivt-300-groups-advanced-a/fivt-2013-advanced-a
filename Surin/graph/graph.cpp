#include "graph.h"

void Graph::addEdge(int u, int v) {
    getIncidents(u)->add(v);
}

ListOfIncidents * Graph::getIncidents(int x) {
    return lists[x].get();
}
void Graph::delEdge(int u, int v) {
    getIncidents(u)->remove(v);
}

Graph::Graph(std::unique_ptr<ListFactory> ls, int n) {
    for (int i = 0; i < n; i++) {
        lists.push_back(std::unique_ptr<ListOfIncidents>(ls->get(i, n)));
    }
}

Cond::Cond(std::unique_ptr<ListFactory> par1, std::unique_ptr<ListFactory> par2, std::function<bool (int)> par3) {
        orig = std::move(par1);
        nnew = std::move(par2);
        pred = par3;
}

ListOfIncidents * Cond::get(int x, int n) {
    if (!pred(x)) return orig->get(x, n);
    else return nnew->get(x, n);
}

bool ListOfIncidents::isConnected(int x) {
    for (std::unique_ptr<IntIterator> it = std::move(begin());
                !it->end();
                    ++*it) {
        if ((**it) == x) return true;
        
    }
    return false;      
}


List<std::vector<int> >::Iterator::Iterator(std::vector<int>::iterator it,
                        std::vector<int>::iterator last) : it(it), last(last) {
}

void List<std::vector<int> >::Iterator::operator ++ () {
    it++;    
}

int List<std::vector<int> >::Iterator::operator * () {
    return *it;
}

bool List<std::vector<int> >::Iterator::end() {
    return it == last;
}

void List<std::vector<int> >::add(int x) {
    v.push_back(x);
}

void List<std::vector<int> >::remove(int x) {
    v.push_back(x);
}

List<std::vector<int> >::List(int n) : v(n) {
}
