#ifndef ISTREAM_IO
#define ISTREAM_IO
#include "abstract_io.h"
#include <memory.h>
#include <string>
#include <iostream>

template<class T, class IS=std::istream>
class ISReader: public Reader<T> {
private:
    void read(T & x) {
        _is >> x;
    }
    IS & _is;
public:
    ISReader(IS * is): _is(*is) {}
    virtual Reader<T> & operator >>(T & x) {
        read(x);
        return *this;
    }
    virtual bool eos() {
        return _is.eof();
    }
};

template<class T, class OS=std::ostream>
class OSWriter: public Writer<T> {
private:
    OS * _os;
    std::string dl;
public:
    OSWriter(OS * os, const std::string & del): _os(os), dl(del) {}
    virtual Writer<T> & operator <<(const T & x) {
        (*_os) << x << dl;
        return *this;
    }
};


#endif
