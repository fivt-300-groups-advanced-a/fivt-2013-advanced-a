#ifndef TEXT_FILE_READER

#define TEXT_FILE_READER

#include <string>
#include <fstream>
#include <memory>

template <class T> class TextFileReader {
    public:
        TextFileReader(){}

        explicit TextFileReader(const std::string &filename){
            setStream(filename);
        }

        explicit TextFileReader(char *filename){
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
                    std::ifstream::in));
        }

        void setStream(char *filename){
            in.reset(new std::ifstream(filename,
                    std::ifstream::in));
        }

        bool operator () (T &next){
            return in && (*in >> next);
        }

    private:
        std::unique_ptr<std::ifstream> in;
};

#endif
