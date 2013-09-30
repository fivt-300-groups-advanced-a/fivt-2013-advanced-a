#ifndef ABSTRACT_IO_H
#define ABSTRACT_IO_H

template<class T>
class Reader {
public:
    virtual Reader<T> & operator >>(T & x) = 0;
    virtual bool eos() = 0;
};

template<class T>
class Writer {
public:
    virtual Writer<T> & operator <<(const T & x) = 0;
};

#endif
