#ifndef BINARY_FILE_READER

#define BINARY_FILE_READER

#include <string>
#include <fstream>
#include <memory>

template <class T> class BinaryFileReader {
    public:
        explicit BinaryFileReader(){}

        explicit BinaryFileReader(std::ifstream &new_stream){
            setStream(new_stream);
        }

        explicit BinaryFileReader(const std::string &filename){
            setStream(filename);
        }

        explicit BinaryFileReader(char *filename){
            setStream(filename);
        }

        void close(){
            if (in){
                in->close();
            }
            in = NULL;
        }

        void setStream(const std::string &filename){
            in.reset(new std::ifstream(filename.c_str(), 
                    std::ifstream::in | std::ifstream::binary));
        }

        void setStream(char *filename){
            in.reset(new std::ifstream(filename, 
                    std::ifstream::in | std::ifstream::binary));
        }

        bool operator () (T &next){
            return in && in->read((char*)&next, sizeof(T));
        }

    private:
        std::unique_ptr<std::ifstream> in;
};

#endif
