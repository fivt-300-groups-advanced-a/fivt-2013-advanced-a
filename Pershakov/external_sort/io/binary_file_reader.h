#ifndef BINARY_FILE_READER

#define BINARY_FILE_READER

#include <string>
#include <fstream>

template <class T> class BinaryFileReader {
    public:
        explicit BinaryFileReader(std::ifstream &new_stream){
            in = &new_stream;
        }

        explicit BinaryFileReader(std::string filename){
            in = new std::ifstream(filename.c_str(), 
                        std::ifstream::binary | std::ifstream::in); 
        }

        explicit BinaryFileReader(char *filename){
            in = new std::ifstream(filename, 
                        std::ifstream::binary | std::istream::in);
        }

        bool operator () (T &next){
            return in->read((char*)&next, sizeof(T));
        }

    private:
        std::ifstream *in;
};

#endif
