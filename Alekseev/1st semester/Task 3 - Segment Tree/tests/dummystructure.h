#ifndef DUMMYSTRUCTURE_H
#define DUMMYSTRUCTURE_H

#include <vector>
#include <algorithm>

template<typename T>
class DummyStructure
{
public:
    DummyStructure() {}

    DummyStructure(std::size_t size, T value = T()):
        v(size, value)
    {}

    template<class FwdIt>
    DummyStructure(FwdIt begin, FwdIt end):
        v(begin, end)
    {}

    template<class Plus = std::plus<T> >
    T sum(std::size_t l, std::size_t r, Plus plus = std::plus<T>())
    {
        return std::accumulate(v.begin()+l, v.begin()+r+1, 0, plus);
    }

    template<class Less = std::less<T> >
    T min(std::size_t l, std::size_t r, Less less = std::less<T>())
    {
        return *std::min_element(v.begin()+l, v.begin()+r+1, less);
    }
    template<class Less = std::less<T> >
    T max(std::size_t l, std::size_t r, Less less = std::less<T>())
    {
        return *std::max_element(v.begin()+l, v.begin()+r+1, less);
    }

    template<class Plus = std::plus<T> >
    void add(std::size_t l, std::size_t r, T value, Plus plus = std::plus<T>())
    {
        for (std::size_t i = l; i <= r; ++i)
            v[i] = plus(v[i], value);
    }

    void assign(std::size_t l, std::size_t r, T value)
    {
        for (std::size_t i = l; i <= r; ++i)
            v[i] = value;
    }

private:
    std::vector<T> v;
};

#endif // DUMMYSTRUCTURE_H
