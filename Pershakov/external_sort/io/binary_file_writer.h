#ifndef BINARY_FILE_WRITER

#define BINARY_FILE_WRITER

#include <fstream>
#include <string>

template <class T> class BinaryFileWriter {
    public:
        ~BinaryFileWriter(){
            close();
        }

        explicit BinaryFileWriter(){
            out = NULL;
        }

        explicit BinaryFileWriter(const std::string &filename){
            out = NULL;
            setStream(filename);
        }

        explicit BinaryFileWriter(char *filename){
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
                    std::ofstream::binary | std::ofstream::out);
        }

        void setStream(char *filename){
            close();
            out = new std::ofstream(filename, 
                    std::ofstream::binary | std::ofstream::out);
        }

        bool operator () (const T &next, const std::string &separator = ""){
            if (!out)
                return false;
            out->write((char*)&next, sizeof(T));
            return true;
        }

    private:
        std::ofstream *out;
};

#endif
