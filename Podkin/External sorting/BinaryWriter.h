#ifndef BINARYWRITER_H
#define BINARYWRITER_H

#include <fstream>
#include <string>

template<typename Type> class BinaryWriter{
    public:
        ~BinaryWriter(){
            Close();
        }
        explicit BinaryWriter(const std::string &FileName){
            Stream = NULL;
            SetStream(FileName);
        }
        explicit BinaryWriter(const char *FileName){
            Stream = NULL;
            SetStream(FileName);
        }
        void SetStream(const std::string &FileName){
            Close();
            Stream = new std::ofstream(FileName.c_str(), std::ofstream::binary | std::ofstream::out);
        }
        void SetStream(const char *FileName){
            Close();
            Stream = new std::ofstream(FileName, std::ofstream::binary | std::ofstream::out);
        }
        bool operator() (Type &NextElement){
            return Ready() && 
                (Stream->write((char*) &NextElement, sizeof(Type)));
        }
        bool Ready(){
            return Stream && *Stream;
        }
        void Close(){
            if (Stream)
                Stream->close();
            Stream = NULL;
        }
    private:
        std::ofstream *Stream;
};

#endif
