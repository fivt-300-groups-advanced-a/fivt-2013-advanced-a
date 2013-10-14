#ifndef COMPLEXDATA_H
#define COMPLEXDATA_H

#include <ios>
#include <string>

struct ComplexData
{
    double d;
    std::string s;
};

static std::istream& operator >> (std::istream &in, ComplexData &d)
{
    return in >> d.d >> d.s;
}

static std::ostream& operator << (std::ostream &out, const ComplexData &d)
{
    return out << d.d << std::endl << d.s << std::endl;
}

static inline bool operator == (const ComplexData &a, const ComplexData &b)
{
    return a.d - b.d < 1e-8 && a.s == b.s;
}

#endif // COMPLEXDATA_H
