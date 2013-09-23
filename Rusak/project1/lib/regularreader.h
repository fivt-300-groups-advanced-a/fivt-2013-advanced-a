template<typename Type> class RegularReader : public GeneralReader<Type> 
{
  public:
    explicit RegularReader(std::istream &stream = std::cin) {
      this->is = &stream; 
    }
    explicit RegularReader(const char * filename) : GeneralReader<Type>(filename) {}

    explicit RegularReader(std::string &filename) : GeneralReader<Type>(filename) {}

    void operator() (Type & element)
    {
      (*this->is) >> element;
    }
};
