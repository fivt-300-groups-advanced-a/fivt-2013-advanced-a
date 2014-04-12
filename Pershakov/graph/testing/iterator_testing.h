#ifndef ITERATOR_TESTING_H

#define ITERATOR_TESTING_H

#include <vector>
#include <memory>
#include <set>

#include "../graph.h"
#include "../list_of_incidents.h"

class FakeListOfIncidents : public ListOfIncidents {
    public:
        explicit FakeListOfIncidents(const std::vector<int> &_to) {
            to = _to;
        }

        bool isConnected(int v) const override {
            for (int i = 0; i < (int)to.size(); i++)
                if (to[i] == v)
                    return true;
            return false;
        }

        std::unique_ptr<Iterator> getIterator() const override {
            return std::unique_ptr<Iterator>();
        }

    private:
        std::vector<int> to;
};

TEST(IteratorTesting, StdIteratorTesting) {
    int cnt_tests = 1000;
    for (int i = 0; i < cnt_tests; i++) {
        int sz = rand() % 1000;
        std::vector<int> arr(sz);
        for (int j = 0; j < sz; j++)
            arr[j] = rand();
        std::unique_ptr<Iterator> it = std::move(
                std::unique_ptr<Iterator>(
                    new StdIterator<std::vector<int>::iterator>
                    (arr.begin(), arr.end())));
        
        for (int j = 0; j < sz; j++) {
            ASSERT_EQ(arr[j], it->getVertex());
            it->next();
        }   
        ASSERT_TRUE(it->getVertex() == -1);
    }
}

TEST(IteratorTesting, ByConnectionIteratorTesting) {
    int cnt_tests = 200;
    for (int i = 0; i < cnt_tests; i++) {
        int sz = rand() % 200;
        std::vector<int> arr(sz);
        std::set<int> vert;
        for (int j = 0; j < sz; j++) {
            arr[j] = rand() % 500;
            while (vert.find(arr[j]) != vert.end())
                arr[j] = rand() % 500;
            vert.insert(arr[j]);
        }
        ListOfIncidents *li = new FakeListOfIncidents(arr);
        std::unique_ptr<Iterator> it = std::move(
                std::unique_ptr<Iterator>(
                    new ByConnectionIterator
                    (li, 0, 5000)));

        for (int j = 0; j < sz; j++) {
            ASSERT_TRUE(vert.find(it->getVertex()) != vert.end());
            it->next();
        }
        ASSERT_TRUE(it->getVertex() == -1);
        delete li;
    }
}

#endif
