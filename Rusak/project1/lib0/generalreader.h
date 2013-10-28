#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

template <typename Type> class GeneralReader
{
  public:
    explicit GeneralReader() {}

    explicit GeneralReader(std::istream &stream) {
      is = &stream;
      is_own_stream = false;
    }

    explicit GeneralReader(const char * filename) {
      assign(filename);
    }

    explicit GeneralReader(std::string &filename) : GeneralReader(filename.c_str()) {};

    ~GeneralReader() {
      close();
    }

    void assign(const char *filename) {
      fb = new std::filebuf();
      fb->open (filename, std::ios::in);
      is = new std::istream(fb);
      is_own_stream = true;
    }

    void close() {
      if (is_own_stream) {
        delete is;
        fb->close();
        delete fb;
        is_own_stream = false;
      }
    }

  protected:
    std::filebuf *fb;
    std::istream *is;
    bool is_own_stream;
};
