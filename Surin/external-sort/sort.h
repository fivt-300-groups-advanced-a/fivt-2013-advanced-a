#ifndef SORT_H
#define SORT_H
#include "abstract_io.h"
#include "istream_io.h"
#include "binary_file_io.h"
#include "comparator.h"
#include <algorithm>
#include <vector>
#include <queue>
#include <cstdio>

template<class T>
int readBlock(Reader<T> * ccin, T * & buffer, int MAX_BUF) {
    for (int i = 0; i < (MAX_BUF / sizeof(T)); i++) {
        if (ccin->eos()) return i;
        *ccin >> buffer[i];
    }
}

template<class T>
void writeBlock(Writer<T> * ccout, T * buffer, int sz) {
    for (int i = 0; i < sz; i++) {
        *ccout << buffer[i];
    }
}

template<class T, class cmp=CLess<T> >
void stsort(T * buffer, int n, cmp * cc) {
    std::sort(buffer, buffer + n, *cc);
}

char * tempFileName(int x) {
    char * ss = new char[8];
    sprintf(ss, ".p%.5d", x);
    return ss;
}


template<class T, class cmp=CLess<T> >
void bigSort(Reader<T> * ccin, Writer<T> * ccout, int MAX_BUF, cmp * CC = new cmp(), void qsort(T*, int sz, cmp *)=stsort) {
    bigSort(ccin, ccout, CC, MAX_BUF, qsort);
}

template<class T, class cmp=CLess<T> >
void bigSort(Reader<T> * ccin, Writer<T> * ccout, cmp * CC = new cmp(), int MAX_BUF=30000000, void qsort(T*, int sz, cmp *)=stsort) {
    T * buffer = (T*)malloc(MAX_BUF);
    if (buffer == 0) throw "not enough memory";
    int n = readBlock(ccin, buffer, MAX_BUF);
    if (ccin->eos()) {
        qsort(buffer, n, CC);
        writeBlock(ccout, buffer, n);
        return;
    }
    int cfiles = 0;
    while (!(ccin->eos() && n == 0)) {
        char * fn = tempFileName(cfiles++);
        BFWriter<T> cm(fn);
        qsort(buffer, n, CC);
        writeBlock(&cm, buffer, n);
        n = readBlock(ccin, buffer, MAX_BUF);
    }
    std::vector<BFReader<T> > files;
    for (int i = 0; i < cfiles; i++)
         files.push_back(BFReader<T>(tempFileName(i)));
    //fix types
    typedef std::pair<T, BFReader<T> > TPair;
    std::priority_queue< TPair, 
                         std::vector<TPair>, 
                         CPair<T, BFReader<T>, CInvOp<T, cmp>, CEqual<BFReader<T> > > > 
                               q(CPair<T, BFReader<T>, CInvOp<T, cmp>, CEqual<BFReader<T> > >(*(new CInvOp<T, cmp>(*CC))));
    for (int i = 0; i < files.size(); i++) {
        T * x = (T*)malloc(sizeof(T));
        files[i] >> *x;
        q.push(std::make_pair(*x, files[i]));
        delete x;
    }
    while (!q.empty()) {
        TPair ss = q.top();
        q.pop();
        *ccout << ss.first;
        ss.second >> ss.first;
        if (!ss.second.eos())
            q.push(ss);
    }
}

#endif
