#ifndef BINARYREADER_H
#define BINARYREADER_H

#include <fstream>
#include <string>
#include <iostream>

template<typename Type> class BinaryReader{
    public:
        explicit BinaryReader(std::ifstream &Input){
            Stream = &Input;
        }
        explicit BinaryReader(const std::string &FileName){
            Stream = new std::ifstream(FileName.c_str());
        }
        explicit BinaryReader(const char *FileName){
            Stream = new std::ifstream(FileName);
        }
        bool operator() (Type &NextElement){
            return Next() &&
                (Stream->read((char*) &NextElement, sizeof(Type)));
        }
        bool Next() const{
            return Stream && *Stream;
        }
    private:
        std::istream *Stream;
};

#endif
