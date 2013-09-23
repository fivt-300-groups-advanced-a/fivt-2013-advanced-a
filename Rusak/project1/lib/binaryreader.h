template<typename Type> class BinaryReader : public GeneralReader<Type> 
{
  public:
    explicit BinaryReader(std::istream &stream) : GeneralReader<Type>(stream) {}

    explicit BinaryReader(const char * filename) : GeneralReader<Type>(filename) {}

    explicit BinaryReader(std::string &filename) : GeneralReader<Type>(filename) {}

 
    void operator() (Type & element) 
    {
      this->is->read((char*) &element, sizeof(element));
    }
};
