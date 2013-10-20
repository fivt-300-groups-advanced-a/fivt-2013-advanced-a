#ifndef BINARY_FILE_READER

#define BINARY_FILE_READER

#include <string>
#include <fstream>

template <class T> class BinaryFileReader {
    public:
        ~BinaryFileReader(){
            close();
        }

        explicit BinaryFileReader(){
            in = NULL;
        }

        explicit BinaryFileReader(std::ifstream &new_stream){
            in = NULL;
            setStream(new_stream);
        }

        explicit BinaryFileReader(const std::string &filename){
            in = NULL;
            setStream(filename);
        }

        explicit BinaryFileReader(char *filename){
            in = NULL;
            setStream(filename);
        }

        void close(){
            if (in){
                in->close();
                delete in;
            }
            in = NULL;
        }

        void setStream(const std::string &filename){
            close();
            in = new std::ifstream(filename.c_str(), 
                    std::ifstream::in | std::ifstream::binary);
        }

        void setStream(char *filename){
            close();
            in = new std::ifstream(filename, 
                    std::ifstream::in | std::ifstream::binary);
        }

        bool operator () (T &next){
            return in && in->read((char*)&next, sizeof(T));
        }

    private:
        std::ifstream *in;
};

#endif
