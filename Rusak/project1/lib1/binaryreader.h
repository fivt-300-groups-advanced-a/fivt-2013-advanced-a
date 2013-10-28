template<typename T> class BinaryReader : public GeneralReader<T> 
{
  public:
    explicit BinaryReader() {}

    explicit BinaryReader(std::istream &stream) : GeneralReader<T>(stream) {}

    explicit BinaryReader(const char * filename) : GeneralReader<T>(filename) {}

    explicit BinaryReader(std::string &filename) : GeneralReader<T>(filename) {}

 
    bool operator() (T & element) 
    {
      return (this->is->read((char*) &element, sizeof(element)));
    }
};
