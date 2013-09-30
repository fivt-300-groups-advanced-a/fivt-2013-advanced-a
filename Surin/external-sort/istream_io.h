#ifndef ISTREAM_IO
#define ISTREAM_IO
#include "abstract_io.h"
#include <memory.h>
template<class T, class IS>
class ISReader: public Reader<T> {
private:
    void read(T & x) {
        _is >> x;
    }
    IS & _is;
public:
    ISReader(IS & is): _is(is) {}
    virtual Reader<T> & operator >>(T & x) {
        read(x);
        return *this;
    }
    virtual bool eos() {
        return _is.eof();
    }
};

template<class T, class OS>
class ISWriter: public Writer<T> {
private:
    OS & _os;
public:
    ISWriter(OS & os): _os(os) {}
    virtual Writer<T> & operator <<(const T & x) {
        _os << x;
        return *this;
    }
};


#endif
