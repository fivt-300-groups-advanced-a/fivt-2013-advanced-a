#include "graph.h"
#include <algorithm>
#include <cassert>

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


std::unique_ptr<IntIterator> List<std::vector<int> >::begin() {
    return std::unique_ptr<IntIterator>(new Iterator(v.begin(), v.end()));
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
    v.erase(std::find(v.begin(), v.end(), x));
}

List<std::vector<int> >::List(int n) {
}

List<std::vector<int> >::List() {
}

List<std::vector<bool> >::Iterator::Iterator(std::vector<bool>::iterator it, 
                                                std::vector<bool>::iterator last)
                                                    : it(it), last(last) {
    while (this->it != this->last && !*it) {
        ++this->it;
    }
}

void List<std::vector<bool> >::Iterator::operator ++ () {
    it++;
    while (it != last && !*it) {
       it++;
    }
}

std::unique_ptr<IntIterator> List<std::vector<bool> >::begin() {
    return std::unique_ptr<IntIterator>(new Iterator(v.begin(), v.end()));
}

int List<std::vector<bool> >::Iterator::operator * () {
    return *it;
}

bool List<std::vector<bool> >::Iterator::end() {
    return it == last;
}

void List<std::vector<bool> >::add(int x) {
    v[x] = true;
}

void List<std::vector<bool> >::remove(int x) {
    v[x] = false;   
}

bool List<std::vector<bool> >::isConnected(int x) {
   return v[x];
}

List<std::vector<bool> >::List(int n): v(n) {
}

GraphWithRevEdges::GraphWithRevEdges(std::unique_ptr<ListFactory> ptr, int n): Graph(std::move(ptr), n) {
}

void GraphWithRevEdges::addEdge(int u, int v) {
    Graph::addEdge(u, v);
    Graph::addEdge(v, u);
}

void GraphWithRevEdges::delEdge(int u, int v) {
    Graph::delEdge(u, v);
    Graph::delEdge(v, u);
}

FunctionalList::FunctionalList(std::function<bool (int)> pred, int n) : pred(pred), n(n) {
}

void FunctionalList::add(int x) {
    assert(false);   
}

void FunctionalList::remove(int) {
    assert(false);
}

bool FunctionalList::isConnected(int x) {
    return pred(x);
}

FunctionalList::Iterator::Iterator(std::function<bool (int)> pred, 
                                        int it, int last)
                                            : pred(pred), it(it), last(last) {
    while (this->it != this->last && !pred(it)) {
        ++this->it;
    }
}

std::unique_ptr<IntIterator> FunctionalList::begin() {
    return std::unique_ptr<IntIterator>(new Iterator(pred, 0, n));
}

void FunctionalList::Iterator::operator ++ () {
    it++;
    while (it != last && !this->pred(it)) {
        it++;
    }
}

int FunctionalList::Iterator::operator * () {
    return it;
}

bool FunctionalList::Iterator::end() {
    return it == last;
}

FunctionalFactory::FunctionalFactory(std::function<bool (int, int)> pred): pred(pred) {
}

ListOfIncidents * FunctionalFactory::get(int x, int n) {
    return new FunctionalList([=](int _x) {return pred(x, _x);}, n);
}
