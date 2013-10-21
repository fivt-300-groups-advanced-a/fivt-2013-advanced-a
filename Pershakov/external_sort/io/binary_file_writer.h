#ifndef BINARY_FILE_WRITER

#define BINARY_FILE_WRITER

#include <fstream>
#include <string>
#include <memory>

template <class T> class BinaryFileWriter {
    public:
        explicit BinaryFileWriter(){}

        explicit BinaryFileWriter(const std::string &filename){
            setStream(filename);
        }

        explicit BinaryFileWriter(char *filename){
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
                    std::ofstream::binary | std::ofstream::out));
        }

        void setStream(char *filename){
            out.reset(new std::ofstream(filename, 
                    std::ofstream::binary | std::ofstream::out));
        }

        bool operator () (const T &next, const std::string &separator = ""){
            if (!out)
                return false;
            out->write((char*)&next, sizeof(T));
            return true;
        }

    private:
        std::unique_ptr<std::ofstream> out;
};

#endif
