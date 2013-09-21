#ifndef ABSTRACT_READER

#define ABSTRACT_READER

#include <fstream>

template <class T> class Abstract_reader {
    public:
        virtual bool operator () (T &next){
            return 0;
        }

    protected:
        std::ifstream *in;
};

#endif
