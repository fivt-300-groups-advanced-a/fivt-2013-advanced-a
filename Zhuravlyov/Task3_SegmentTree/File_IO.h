#ifndef FILEIO
#define FILEIO
#include <iostream>
#include <fstream>
#include <string>

class BinaryFileReader{
private:
        std::ifstream *inputStream;
public:
        BinaryFileReader(){
                inputStream=NULL;
        }
        void assign(const char *fileName) {
                close();
                inputStream=new std::ifstream(fileName,std::fstream::binary);
        }
        void assign(const std::string &fileName) {
                close();
                inputStream=new std::ifstream(fileName.c_str(),std::ofstream::binary);
        }
        BinaryFileReader(const char *fileName) {
                inputStream=NULL;
                assign(fileName);
        }
        BinaryFileReader(const std::string &fileName) {
                inputStream=NULL;
                assign(fileName);
        }
        template <class T> void operator()(T &element)
        {
                inputStream->read((char*)&element,sizeof(T));
        }
        void close(){
                if (inputStream) {
                                (*inputStream).close();
                                delete inputStream;
                                inputStream=NULL;
                }
        }
        ~BinaryFileReader(){
                close();
        }
};

class BinaryFileWriter{
private:
        std::ofstream *outputStream;
public:
        BinaryFileWriter(){
                outputStream=NULL;
        }
        void assign(char *fileName) {
                close();
                outputStream=new std::ofstream(fileName,std::ofstream::binary);
        }
        void assign(const std::string &fileName) {
                close();
                outputStream=new std::ofstream(fileName,std::ofstream::binary);
        }
        BinaryFileWriter(char *fileName) {
                outputStream=NULL;
                assign(fileName);
        }
        BinaryFileWriter(const std::string &fileName) {
                outputStream=NULL;
                assign(fileName);
        }
        template <class T> void operator()(const T &element) {
                outputStream->write((char*)&element,sizeof(T));
        }
        void close(){
                if (outputStream) {
                                (*outputStream).close();
                                delete outputStream;
                                outputStream=NULL;
                }
        }
        ~BinaryFileWriter(){
                close();
        }
};

bool FileIsExist(std::string filePath)
{
    bool isExist = false;
    std::ifstream fin(filePath.c_str());
 
    if(fin.is_open())
        isExist = true;
 
    fin.close();
    return isExist;
}

#endif