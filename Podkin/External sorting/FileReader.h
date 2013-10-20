#ifndef READER_H
#define READER_H

#include <fstream>
#include <string>
#include <iostream>

template<typename Type> class FileReader{
    public:
        ~FileReader(){
            Close();
        }
        void Close(){
            if (Stream){
                (*Stream).close();
                delete Stream;
                Stream = NULL;
            }
        }
        explicit FileReader(){
            Stream = NULL;
        }
        explicit FileReader(const std::string &FileName){
            Stream = NULL;
            SetStream(FileName);
        }
        void SetStream(const std::string &FileName){
            Close();
            Stream = new std::ifstream(FileName.c_str());
        }
        explicit FileReader(const char *FileName){
            Stream = NULL;
            SetStream(FileName);
        }
        void SetStream(const char *FileName){
            Close();
            Stream = new std::ifstream(FileName);
        }
        bool operator() (Type &NextElement){
            return Next() && (*Stream >> NextElement);
        }
        bool Next() const{
            return Stream && *Stream;
        }
    private:
        std::ifstream *Stream;
};

#endif
