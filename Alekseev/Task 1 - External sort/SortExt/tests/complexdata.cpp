#include "complexdata.h"

namespace std
{
std::istream &operator >> (std::istream &in, ComplexData &d)
{
    return in >> d.d >> d.s;
}

std::ostream &operator << (std::ostream &out, const ComplexData &d)
{
    return out << d.d << std::endl << d.s << std::endl;
}
} //namespace std

bool operator == (const ComplexData &a, const ComplexData &b)
{
    return a.d - b.d < 1e-8 && a.s == b.s;
}
