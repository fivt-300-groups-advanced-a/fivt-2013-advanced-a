#ifndef TEXT_FILE_WRITER

#define TEXT_FILE_WRITER

#include <fstream>
#include <string>
#include <memory>

template <class T> class TextFileWriter {
    public:
        explicit TextFileWriter(){}

        explicit TextFileWriter(const std::string &filename){
            setStream(filename);
        }

        explicit TextFileWriter(char *filename){
            setStream(filename);
        }

        void close(){
            if (out){
                out->close();
            }
            out = NULL;
        }

        void setStream(const std::string &filename){
            out.reset(new std::ofstream(filename.c_str(), 
                    std::ofstream::out));
        }

        void setStream(char *filename){
            out.reset(new std::ofstream(filename, 
                    std::ofstream::out));
        }

        bool operator () (const T &next, 
                const std::string &separator = " "){
            if (!out)
                return false;
            *out << next << separator;
            return true;
        }

    private:
        std::unique_ptr<std::ofstream> out;
};

#endif
