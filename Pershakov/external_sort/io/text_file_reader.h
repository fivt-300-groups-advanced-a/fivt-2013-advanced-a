#ifndef TEXT_FILE_READER

#define TEXT_FILE_READER

#include <string>
#include <fstream>

template <class T> class TextFileReader {
    public:
        ~TextFileReader(){
            close();
        }

        explicit TextFileReader(){
            in = NULL;
        }

        explicit TextFileReader(const std::string &filename){
            in = NULL;
            setStream(filename);
        }

        explicit TextFileReader(char *filename){
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
                    std::ifstream::in);
        }

        void setStream(char *filename){
            close();
            in = new std::ifstream(filename,
                    std::ifstream::in);
        }

        bool operator () (T &next){
            return in && (*in >> next);
        }

    private:
        std::ifstream *in;
};

#endif
