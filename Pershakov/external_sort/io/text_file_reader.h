#ifndef TEXT_FILE_READER

#define TEXT_FILE_READER

#include <string>
#include <fstream>

template <class T> class TextFileReader {
    public:
        ~TextFileReader(){
            close();
        }

        explicit TextFileReader(std::ifstream &new_stream){
            setStream(new_stream);
        }

        explicit TextFileReader(const std::string &filename){
            setStream(filename);
        }

        explicit TextFileReader(char *filename){
            setStream(filename);
        }

        void close(){
            if (in)
                in->close();
            in = NULL;
        }

        void setStream(std::ifstream &new_stream){
            in = &new_stream;
        }

        void setStream(const std::string &filename){
            in = new std::ifstream(filename.c_str(), 
                    std::ifstream::in);
        }

        void setStream(char *filename){
            in = new std::ifstream(filename,
                    std::ifstream::in);
        }

        bool operator () (T &next){
            return *in >> next;
        }

    private:
        std::ifstream *in;
};

#endif
