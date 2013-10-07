#ifndef BINARYREADER_H
#define BINARYREADER_H

#include <fstream>
#include <string>
#include <iostream>

template<typename Type> class BinaryReader{
    public:
        explicit BinaryReader(std::ifstream &in){
            Stream = &in;
        }
        explicit BinaryReader(const std::string &FileName){
            Stream = new std::ifstream(FileName.c_str(), std::ifstream::binary | std::ifstream::in);
        }
        explicit BinaryReader(const char *FileName){
            Stream = new std::ifstream(FileName, std::ifstream::binary | std::ifstream::in);
        }
        bool operator() (Type &NextElement){
            return Stream->read((char*) &NextElement, sizeof(Type));
        }
    private:
        std::istream *Stream;
};

#endif
