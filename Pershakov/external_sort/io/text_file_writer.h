#ifndef TEXT_FILE_WRITER

#define TEXT_FILE_WRITER

#include <fstream>
#include <string>

template <class T> class TextFileWriter {
    public:
        ~TextFileWriter(){
            close();
        }

        explicit TextFileWriter(std::ofstream &new_stream){
            setStream(new_stream);
        }

        explicit TextFileWriter(const std::string &filename){
            setStream(filename);
        }

        explicit TextFileWriter(char *filename){
            setStream(filename);
        }

        void close(){
            if (out)
                out->close();
            out = NULL;
        }

        void setStream(std::ofstream &new_stream){
            out = &new_stream;
        }

        void setStream(const std::string &filename){
            out = new std::ofstream(filename.c_str(), 
                    std::ofstream::out);
        }

        void setStream(char *filename){
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
