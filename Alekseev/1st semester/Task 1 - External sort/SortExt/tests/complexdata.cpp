#include "complexdata.h"

namespace std
{
std::istream &operator >> (std::istream &in, ComplexData &d)
{
    in >> d.d;
    return getline(getline(in, d.s), d.s);
}

std::ostream &operator << (std::ostream &out, const ComplexData &d)
{
    return out << d.d << std::endl << d.s << std::endl;
}
} //namespace std

bool operator == (const ComplexData &a, const ComplexData &b)
{
    return a.d == b.d && a.s == b.s;
}
