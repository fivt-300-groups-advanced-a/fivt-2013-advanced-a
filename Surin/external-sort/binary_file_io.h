#ifndef BINARY_FILE_IO
#define BINARY_FILE_IO
#include "abstract_io.h"
#include <cstdio>
#include <memory.h>

template<class T>
class BFReader : public Reader<T> {
private:
    void read(T & x) {
        fread((void *)(&x), sizeof(T), 1, fd);
    }
    FILE * fd = NULL;
    T * buffer = NULL;
public:
    void open(char * filename) {
        close();
        fd = fopen(filename, "rb");
    }
    BFReader(char * filename) {
        open(filename);
    }
    virtual Reader<T> & operator >>(T & x) {
        if (buffer != NULL) {
            x = *buffer;
            buffer = NULL;
        } else {
            read(x);
        }
        return *this;
    }
    ~BFReader() {
        close();
    }
    void close() {
        if (fd != NULL) fclose(fd);
        fd = NULL;
    }
    virtual bool eos() {
        if (buffer != NULL) return false;
        buffer = (T*)malloc(sizeof(T));
        read(*buffer);
        return feof(fd);
    }
};
template<class T>
class BFWriter: public Writer<T> {
private:
    FILE * fd = NULL;
public:
    void open(char * filename) {
        close();
        fd = fopen(filename, "wb");
    }
    BFWriter(char * filename) {
        open(filename);
    }
    virtual Writer<T> & operator <<(const T & x) {
        fwrite((void *)(&x), sizeof(T), 1, fd);
        return *this;
    }
    ~BFWriter() {
        close();
    }
    void close() {
        if (fd != NULL) fclose(fd);
        fd = NULL;
    }
};


#endif
