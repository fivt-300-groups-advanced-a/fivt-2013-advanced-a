#ifndef TEXT_FILE_READER

#define TEXT_FILE_READER

#include <string>
#include <fstream>

template <class T> class Text_file_reader {
    public:
        explicit Text_file_reader(std::ifstream &new_stream){
            this->in = &new_stream;
        }

        explicit Text_file_reader(std::string file_name){
            this->in = 
                new std::ifstream(file_name.c_str(), std::ifstream::in); 
        }

        explicit Text_file_reader(char *file_name){
            this->in = 
                new std::ifstream(file_name, std::ifstream::in);
        }

        bool operator () (T &next){
            return *this->in >> next;
        }
};

#endif
