#ifndef BINARYWRITER_H
#define BINARYWRITER_H

#include <fstream>
#include <string>

template<typename Type> class BinaryWriter{
    public:
        ~BinaryWriter(){
            Close();
        }
        explicit BinaryWriter(std::ofstream &YourStream){
            SetStream(YourStream);
        }
        explicit BinaryWriter(const std::string &FileName){
            SetStream(FileName);
        }
        explicit BinaryWriter(const char *FileName){
            SetStream(FileName);
        }
        void SetStream(std::ofstream &YourStream){
            Stream = &YourStream;
        }
        void SetStream(const std::string &FileName){
            Stream = new std::ofstream(FileName.c_str(), std::ofstream::binary | std::ofstream::out);
        }
        void SetStream(const char *FileName){
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
