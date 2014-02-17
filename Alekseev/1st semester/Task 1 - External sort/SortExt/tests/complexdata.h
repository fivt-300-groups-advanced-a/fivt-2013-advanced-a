#ifndef COMPLEXDATA_H
#define COMPLEXDATA_H

#include <iostream>
#include <string>

struct ComplexData
{
    double d;
    std::string s;
    ComplexData(double d = 0., const std::string &s = ""): d(d), s(s) {}
};

namespace std
{
std::istream &operator >> (std::istream &in, ComplexData &d);
std::ostream &operator << (std::ostream &out, const ComplexData &d);
} // namespace std

struct ComplexDataComparator
{
    bool operator() (const ComplexData &a, const ComplexData &b)
    {
        return make_pair(a.d, a.s) < make_pair(b.d, b.s);
    }
};

bool operator == (const ComplexData &a, const ComplexData &b);

#endif // COMPLEXDATA_H
