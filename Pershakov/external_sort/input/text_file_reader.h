#ifndef TEXT_FILE_READER

#define TEXT_FILE_READER

#include <string>
#include <fstream>

template <class T> class TextFileReader {
    public:
        explicit TextFileReader(std::ifstream &new_stream){
            in = &new_stream;
        }

        explicit TextFileReader(std::string file_name){
            in = new std::ifstream(file_name.c_str(), std::ifstream::in); 
        }

        explicit TextFileReader(char *file_name){
            in = new std::ifstream(file_name, std::ifstream::in);
        }

        bool operator () (T &next){
            return *in >> next;
        }
    private:
        std::ifstream *in;
};

#endif
