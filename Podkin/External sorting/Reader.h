#ifndef READER_H
#define READER_H

#include <fstream>
#include <string>
#include <iostream>

template<typename Type> class Reader{
    public:
        explicit Reader(std::ifstream &Input){
            Stream = &Input;
        }
        explicit Reader(const std::string &FileName){
            Stream = new std::ifstream(FileName.c_str());
        }
        explicit Reader(const char *FileName){
            Stream = new std::ifstream(FileName);
        }
        bool operator() (Type &NextElement){
            return Next() && (*Stream >> NextElement);
        }
        bool Next() const{
            return Stream && *Stream;
        }
    private:
        std::istream *Stream;
};

#endif
