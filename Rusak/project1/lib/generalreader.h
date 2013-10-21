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
      fb = new std::filebuf();
      fb->open (filename, std::ios::in);
      is = new std::istream(fb);
      is_own_stream = true;
    }

    explicit GeneralReader(std::string &filename) : GeneralReader(filename.c_str()) {};

    ~GeneralReader() {
      if (is_own_stream) {
        delete is;
        delete fb;
      }
    }

    bool eof() {
      return (is->eof());
    }

  protected:
    std::filebuf *fb;
    std::istream *is;
    bool is_own_stream;
};
