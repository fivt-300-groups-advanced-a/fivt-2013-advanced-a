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
    }

    explicit GeneralReader(const char * filename) {
      fb = new std::filebuf();
      fb->open (filename, std::ios::in);
      is = new std::istream(fb);
    }

    explicit GeneralReader(std::string &filename) {
      GeneralReader(filename.c_str());
    }

  protected:
    std::filebuf *fb;
    std::istream *is;
};
