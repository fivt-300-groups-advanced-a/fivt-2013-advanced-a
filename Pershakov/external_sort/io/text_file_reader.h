#ifndef TEXT_FILE_READER

#define TEXT_FILE_READER

#include <string>
#include <fstream>

template <class T> class TextFileReader {
    public:
        explicit TextFileReader(std::ifstream &new_stream){
            in = &new_stream;
        }

        explicit TextFileReader(std::string filename){
            in = new std::ifstream(filename.c_str(), std::ifstream::in); 
        }

        explicit TextFileReader(char *filename){
            in = new std::ifstream(filename, std::ifstream::in);
        }

        bool operator () (T &next){
            return *in >> next;
        }
    private:
        std::ifstream *in;
};

#endif
