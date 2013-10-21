#ifndef BINARY_FILE_IO
#define BINARY_FILE_IO
#include "abstract_io.h"
#include <cstdio>
#include <memory.h>

template<class T>
class BFReader : public Reader<T> {
private:
    void readbuffer() {
        bl = fread(buffer, sizeof(T), BUF_SIZE/(sizeof(T)), fd);
        bf = 0;
    }
    void read(T & x) {
        if (bl == bf && feof(fd)) {
            return;
        }
        if (bl == bf) readbuffer();
        memcpy(&x, buffer + bf, sizeof(T));
        bf++;
        if (bl == bf) readbuffer();
    }
    bool EOS;
    FILE * fd = NULL;
    int BUF_SIZE;
    T * buffer = NULL;
    int bf, bl;
public:
    static const int DEFAULT_CACHE = 300000000;
    void open(std::string filename) {
        bf = bl = 0;
        close();
        fd = fopen(filename.c_str(), "rb");
    }
    BFReader(std::string filename, int BUF_SIZE=DEFAULT_CACHE) {
        this->BUF_SIZE = BUF_SIZE;
        open(filename);
        if ((buffer = (T*)malloc(BUF_SIZE)) == NULL)
            throw "not enough memory";        
    }
    BFReader(int BUF_SIZE=DEFAULT_CACHE) {
        this->BUF_SIZE = BUF_SIZE;
        if ((buffer = (T*)malloc(BUF_SIZE)) == NULL)
            throw "not enough memory";        
    }
    virtual Reader<T> & operator >>(T & x) {
        read(x);
        return *this;
    }
    ~BFReader() {
        delete buffer;
        close();
    }
    void close() {
        if (fd != NULL) fclose(fd);
        fd = NULL;
    }
    virtual bool eos() {
        return feof(fd) && bf == bl;
    }
};

template<class T>
class BFWriter: public Writer<T> {
private:
    FILE * fd = NULL;
    int BUF_SIZE;
    T * buffer = NULL;
    int bl;
    void flush() {
        fwrite(buffer, sizeof(T), bl, fd);    
        fflush(fd);
        bl = 0;
    }
    void write(const T & x) {
        if (bl == BUF_SIZE/(sizeof(T))) {
            flush();
        } else {
            memcpy(buffer + (bl++), &x, sizeof(T));
        }
    }
public:
    static const int DEFAULT_CACHE = 300000000;
    BFWriter(int BUF_SIZE=DEFAULT_CACHE){
        this->BUF_SIZE = BUF_SIZE;
        if ((buffer = (T*)malloc(BUF_SIZE)) == NULL)
            throw "not enough memory";        
    }
    void open(std::string filename) {
        close();
        bl = 0;
        fd = fopen(filename.c_str(), "wb");
    }
    BFWriter(std::string filename, int BUF_SIZE=DEFAULT_CACHE) {
        this->BUF_SIZE = BUF_SIZE;
        if ((buffer = (T*)malloc(BUF_SIZE)) == NULL)
            throw "not enough memory";        
        open(filename);
    }
    virtual Writer<T> & operator <<(const T & x) {
        write(x);
        return *this;
    }
    ~BFWriter() {
        close();
        delete buffer;
    }
    void close() {
        if (fd != NULL) {
            flush();
            fclose(fd);
        }
        fd = NULL;
    }
};

#endif
