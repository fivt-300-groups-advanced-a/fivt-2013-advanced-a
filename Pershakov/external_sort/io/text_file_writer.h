#ifndef TEXT_FILE_WRITER

#define TEXT_FILE_WRITER

#include <fstream>
#include <string>

template <class T> class TextFileWriter {
    public:
        ~TextFileWriter(){
            close();
        }

        explicit TextFileWriter(){
            out = NULL;
        }

        explicit TextFileWriter(const std::string &filename){
            out = NULL;
            setStream(filename);
        }

        explicit TextFileWriter(char *filename){
            out = NULL;
            setStream(filename);
        }

        void close(){
            if (out){
                out->close();
                delete out;
            }
            out = NULL;
        }

        void setStream(const std::string &filename){
            close();
            out = new std::ofstream(filename.c_str(), 
                    std::ofstream::out);
        }

        void setStream(char *filename){
            close();
            out = new std::ofstream(filename, 
                    std::ofstream::out);
        }

        bool operator () (const T &next, 
                const std::string &separator = " "){
            if (!out)
                return false;
            *out << next << separator;
            return true;
        }

    private:
        std::ofstream *out;
};

#endif
