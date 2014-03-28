#ifndef LIST_OF_INCIDENTS_H

#define LIST_OF_INCIDENTS_H

#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>

class Iterator {
    public:
        virtual void next() = 0;

        virtual int getVertex() const = 0;

        virtual ~Iterator() { }
};

class ListOfIncidents {
    public:
        virtual bool isConnected(int v) const = 0;

        virtual std::unique_ptr<Iterator> getIterator() const = 0;

        virtual ~ListOfIncidents() { }
};

class ByConnectionIterator : public Iterator {
    public:
        ByConnectionIterator(const ListOfIncidents *_list, int cur, int last) {
            list = _list;
            cur_vertex = cur;
            last_vertex = last;
            if (!list->isConnected(cur_vertex))
                next();
        }

        ~ByConnectionIterator() { }

        void next() override {
            if (cur_vertex == last_vertex)
                cur_vertex = -1;
            if (cur_vertex == -1)
                return;
            cur_vertex++;
            while (cur_vertex < last_vertex && !list->isConnected(cur_vertex)) {
                cur_vertex++;
            }
            if (!list->isConnected(cur_vertex))
                cur_vertex = -1;
        }

        int getVertex() const override {
            return cur_vertex;
        }

    private:
        const ListOfIncidents *list;
        int cur_vertex;
        int last_vertex;
};

template<class T> class StdIterator : public Iterator {
    public:
        StdIterator(const T _cur, const T _last) {
            cur = _cur;
            last = _last;
        }

        ~StdIterator() { }

        void next() override {
            if (cur != last)
                cur++;
        }

        int getVertex() const override {
            if (cur == last)
                return -1;
            else
                return *cur;
        }

    private:
        T cur, last;
};

class FunctionalListOfIncidents : public ListOfIncidents {
    public:
        explicit FunctionalListOfIncidents(int _to) {
            to = _to;
        }

        ~FunctionalListOfIncidents() { }

        bool isConnected(int v) const override {
            return v == to;
        }

        std::unique_ptr<Iterator> getIterator() const override {
            return std::move(
                    std::unique_ptr<Iterator>(
                        new ByConnectionIterator(this, to, to)));
        }

    private:
        int to;
};

class KHeapListOfIncidents : public ListOfIncidents {
    public:
        KHeapListOfIncidents(int _first_son, int _last_son) {
            first_son = _first_son;
            last_son = _last_son;
        }

        ~KHeapListOfIncidents() { }

        bool isConnected(int v) const override {
            return first_son <= v && v <= last_son;
        }

        std::unique_ptr<Iterator> getIterator() const override {
            return std::move(
                    std::unique_ptr<Iterator>(
                        new ByConnectionIterator(this, first_son, last_son)));
        }

    private:
        int first_son, last_son;
};

class MatrixOfIncidents : public ListOfIncidents {
    public:
        explicit MatrixOfIncidents(const std::vector<bool> &_is_edge) {
            is_edge = _is_edge;
        }

        ~MatrixOfIncidents() { }

        bool isConnected(int v) const override {
            return is_edge[v];
        }

        std::unique_ptr<Iterator> getIterator() const override {
            return std::move(
                    std::unique_ptr<Iterator>(
                        new ByConnectionIterator(this, 0, is_edge.size() - 1)));
        }

    private:
        std::vector<bool> is_edge;
};

class IncidentVertexes : public ListOfIncidents {
    public:
        IncidentVertexes(const std::vector<int> &_to) {
            to = _to;
            bool sorted = true;
            for (int i = 0; i < (int)to.size() - 1; i++)
                if (to[i] > to[i + 1]) {
                    sorted = false;
                    break;
                }
            if (!sorted)
                std::sort(to.begin(), to.end());
        }

        ~IncidentVertexes() { }

        bool isConnected(int v) const override {
            auto it = std::lower_bound(to.begin(), to.end(), v);
            if (it == to.end() || *it != v)
                return false;
            else
                return true;
        }

        std::unique_ptr<Iterator> getIterator() const override {
            typedef std::vector<int>::const_iterator iter;
            return std::move(
                    std::unique_ptr<Iterator>(
                        new StdIterator<iter>(to.begin(), to.end())));
        }

    private:
        std::vector<int> to;
};

#endif
