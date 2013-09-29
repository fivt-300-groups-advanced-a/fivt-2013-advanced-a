#ifndef BINARY_FILE_READER

#define BINARY_FILE_READER

#include <string>
#include <fstream>
#include <iostream>

template <class T> class BinaryFileReader {
    public:
        explicit BinaryFileReader(std::ifstream &new_stream){
            in = &new_stream;
        }

        explicit BinaryFileReader(std::string file_name){
            in = new std::ifstream(file_name.c_str(), 
                        std::ifstream::binary | std::ifstream::in); 
        }

        explicit BinaryFileReader(char *file_name){
            in = new std::ifstream(file_name, 
                        std::ifstream::binary | std::istream::in);
        }

        bool operator () (T &next){
            return in->read((char*)&next, sizeof(T));
        }
    private:
        std::ifstream *in;
};

#endif
