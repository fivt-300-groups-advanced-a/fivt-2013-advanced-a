template<typename T> class RegularReader : public GeneralReader<T> 
{
  public:
    explicit RegularReader() {}

    explicit RegularReader(std::istream &stream = std::cin) {
      this->is = &stream; 
    }
    explicit RegularReader(const char * filename) : GeneralReader<T>(filename) {}

    explicit RegularReader(std::string &filename) : GeneralReader<T>(filename) {}

    bool operator() (T & element)
    {
      return ( (*this->is) >> element );
    }
};
