#ifndef SORT_H
#define SORT_H
#include "abstract_io.h"
#include "comparator.h"
#include "istream_io.h"
#include "binary_file_io.h"
#include <algorithm>
#include <vector>
#include <queue>
#include <cstdio>
#include <sstream>

namespace sort_utils {
    template<class T>
    int readBlock(Reader<T> * ccin, T * buffer, int BUF_SIZE) {
        for (int i = 0; i < (BUF_SIZE / sizeof(T)); i++) {
            if (ccin->eos()) return i;
            (*ccin) >> buffer[i];
        }
        return BUF_SIZE/sizeof(T);
    }

    template<class T>
    void writeBlock(Writer<T> * ccout, T * buffer, int sz) {
        for (int i = 0; i < sz; i++) {
            (*ccout) << buffer[i];
        }
    }

    template<class T, class cmp=CLess<T> >
    void stsort(T * buffer, int n, cmp * cc) {
        std::sort(buffer, buffer + n, *cc);
    }

    std::string tempFileName(int x) {
        std::stringstream ss;
        ss << x;
        //char * ss = new char[8];
        //sprintf(ss, ".p%.5d", x);
        return ss.str();
    }
    const int MAX_BUF =  30000000;
}

template<class T, class cmp=CLess<T> >
void bigSort(Reader<T> * ccin, Writer<T> * ccout, int MAX_BUF, void qsort(T*, int , cmp *)=sort_utils::stsort) {
    bigSort(ccin, ccout, new cmp(), sort_utils::MAX_BUF, qsort);
}

template<class T, class cmp=CLess<T> >
void bigSort(Reader<T> * ccin, Writer<T> * ccout, cmp * CC = new cmp(), int MAX_BUF=sort_utils::MAX_BUF, void qsort(T*, int , cmp *)=sort_utils::stsort) {
    T * buffer = (T*)malloc(MAX_BUF);
    if (buffer == 0) throw "not enough memory";
    int n = sort_utils::readBlock(ccin, buffer, MAX_BUF);
    int cfiles = 0;
    if (ccin->eos()) {
        qsort(buffer, n, CC);
        sort_utils::writeBlock(ccout, buffer, n);
        return;
    }
    do {
        std::string fn = sort_utils::tempFileName(cfiles++);
        BFWriter<T> cm(fn);
        qsort(buffer, n, CC);
        sort_utils::writeBlock(&cm, buffer, n);
        n = sort_utils::readBlock(ccin, buffer, MAX_BUF);
    } while (n != 0);
    std::vector<BFReader<T> * > files;
    for (int i = 0; i < cfiles; i++)
         files.push_back(new BFReader<T>(sort_utils::tempFileName(i)));
    //fix types
    typedef std::pair<T, BFReader<T> * > TPair;
    std::priority_queue< TPair, 
                         std::vector<TPair>, 
                         CPair<T, BFReader<T>*, CInvOp<T, cmp>, CEqual<BFReader<T>* > > > 
                               q(CPair<T, BFReader<T>*, CInvOp<T, cmp>, CEqual<BFReader<T>* > >(*(new CInvOp<T, cmp>(*CC))));
    for (int i = 0; i < files.size(); i++) {
        T * x = (T*)malloc(sizeof(T));
        (*files[i]) >> *x;
        q.push(std::make_pair(*x, files[i]));
        delete x;
    }
    while (!q.empty()) {
        TPair ss = q.top();
        q.pop();
        *ccout << ss.first;
        if (!ss.second->eos()) {
            (*ss.second) >> ss.first;
            q.push(ss);
        }
    }
    for (int i = 0; i < files.size(); i++) {
        remove(sort_utils::tempFileName(i).c_str());
    }
}

#endif
