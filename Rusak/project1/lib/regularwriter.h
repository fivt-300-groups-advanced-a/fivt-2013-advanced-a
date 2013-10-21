template<typename T> class RegularWriter : public GeneralWriter<T> 
{
  public:
    explicit RegularWriter(std::istream &stream) : GeneralWriter<T>(stream) {}

    explicit RegularWriter(const char * filename) : GeneralWriter<T>(filename) {}

    explicit RegularWriter(std::string &filename) : GeneralWriter<T>(filename) {}

 
    bool operator() (const T & element) 
    {
      (*this->os) << element;
//      return ( (*this->os) << element << "\n" );
    }
};
