template<typename T> class BinaryWriter : public GeneralWriter<T> 
{
  public:
    explicit BinaryWriter(std::istream &stream) : GeneralWriter<T>(stream) {}

    explicit BinaryWriter(const char * filename) : GeneralWriter<T>(filename) {}

    explicit BinaryWriter(std::string &filename) : GeneralWriter<T>(filename) {}

 
    bool operator() (const T & element) 
    {
      return (this->os->write((char*) &element, sizeof(element)));
    }
};
