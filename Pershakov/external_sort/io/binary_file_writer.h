#ifndef BINARY_FILE_WRITER

#define BINARY_FILE_WRITER

#include <fstream>
#include <string>

template <class T> class BinaryFileWriter {
    public:
        ~BinaryFileWriter(){
            close();
        }

        explicit BinaryFileWriter(std::ofstream &new_stream){
            setStream(new_stream);
        }

        explicit BinaryFileWriter(const std::string &filename){
            setStream(filename);
        }

        explicit BinaryFileWriter(char *filename){
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
                    std::ofstream::binary | std::ofstream::out);
        }

        void setStream(char *filename){
            out = new std::ofstream(filename, 
                    std::ofstream::binary | std::ofstream::out);
        }

        bool operator () (const T &next){
            if (!out)
                return false;
            out->write((char*)&next, sizeof(T));
            return true;
        }

    private:
        std::ofstream *out;
};

#endif
