#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

template <typename Type> class GeneralWriter
{
  public:
    explicit GeneralWriter() {}

    explicit GeneralWriter(std::ostream &stream) {
      os = &stream;
      is_own_stream = false;
    }

    explicit GeneralWriter(const char * filename) {
      fb = new std::filebuf();
      fb->open (filename, std::ios::out);
      os = new std::ostream(fb);
      is_own_stream = true;
    }

    explicit GeneralWriter(std::string &filename) : GeneralWriter(filename.c_str()) {};

    ~GeneralWriter() {
      close();
    }

    void close() {
      if (is_own_stream) {
        delete os;
        fb->close();
        delete fb;
        is_own_stream = false;
      }
    }


  protected:
    std::filebuf *fb;
    std::ostream *os;
    bool is_own_stream;
};
