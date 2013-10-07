#ifndef WRITER_H
#define WRITER_H

#include <fstream>
#include <string>

template<typename Type> class Writer{
    public:
        ~Writer(){
            Close();
        }
        explicit Writer(std::ofstream &YourStream){
            SetStream(YourStream);
        }
        explicit Writer(const std::string &FileName){
            SetStream(FileName);
        }
        explicit Writer(const char *FileName){
            SetStream(FileName);
        }
        void SetStream(std::ofstream &YourStream){
            Stream = &YourStream;
        }
        void SetStream(const std::string &FileName){
            Stream = new std::ofstream(FileName.c_str());
        }
        void SetStream(const char *FileName){
            Stream = new std::ofstream(FileName);
        }
        bool operator() (Type &NextElement,const std::string &YourPrefix = "", const std::string &YourSuffix = " "){
            Suffix = YourSuffix;
            Prefix = YourPrefix;
            return Ready() && (*Stream << Prefix << NextElement << Suffix);
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
        std::string Prefix, Suffix;
};

#endif
