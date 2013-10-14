#ifndef BINARYREADER_H
#define BINARYREADER_H

#include <fstream>
#include <string>
#include <iostream>

template<typename Type> class BinaryReader{
    public:
        ~BinaryReader(){
            Close();
        }
        void Close(){
            if (Stream)
                Stream->close();
            Stream = NULL;
        }
        explicit BinaryReader(const std::string &FileName){
            Stream = NULL;
            SetStream(FileName);
        }
        void SetStream(const std::string &FileName){
            Close();
            Stream = new std::ifstream(FileName.c_str(), std::ifstream::binary | std::ifstream::in);
        }
        explicit BinaryReader(const char *FileName){
            Stream = NULL;
            SetStream(FileName);
        }
        void SetStream(const char *FileName){
            Close();
            Stream = new std::ifstream(FileName, std::ifstream::binary | std::ifstream::in);
        }
        bool operator() (Type &NextElement){
            return Stream->read((char*) &NextElement, sizeof(Type));
        }
    private:
        std::ifstream *Stream;
};

#endif
