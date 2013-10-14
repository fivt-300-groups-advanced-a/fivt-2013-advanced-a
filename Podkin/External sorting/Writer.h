#ifndef WRITER_H
#define WRITER_H

#include <fstream>
#include <string>

template<typename Type> class Writer{
    public:
        ~Writer(){
            Close();
        }
        explicit Writer(const std::string &FileName){
            Stream = NULL;
            SetStream(FileName);
        }
        explicit Writer(const char *FileName){
            Stream = NULL;
            SetStream(FileName);
        }
        void SetStream(const std::string &FileName){
            Close();
            Stream = new std::ofstream(FileName.c_str());
        }
        void SetStream(const char *FileName){
            Close();
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
