#ifndef RWSTREAMS_H
#define RWSTREAMS_H

#include <fstream>
#include <string>
#include <memory>


/**
*This class provides base realisation of reading data from ifstream
* Template is from type of data to read
*/
template <class T>
class Reader
{
private:
    std :: unique_ptr<std :: ifstream> ifsp;

public:

    /**
    * Constructor of class std :: need unique_ptr<std :: ifstream>
    * This class will delete ifstream by itself
    */
    Reader(std :: unique_ptr<std :: ifstream> ifs) : ifsp(std::move(ifs))
    {
    }


    /**
    * This function read data from file in value
    */
    bool read(T &value) const
    {
        return ((*ifsp) >> value);
    }


    /**
    * You may call this function to close stream
    */
    void close()
    {
        ifsp->close();
    }
};

/**
*This class provides base realisation of writing data from ifstream
* Template is from type of data to write
*/
template <class T>
class Writer
{
private:
    std :: unique_ptr<std :: ofstream> ofsp;
    std :: string separator;
public:
    /**
    * Constructor of class std :: need unique_ptr<std :: ofstream>
    * This class will delete ofstream by itself
    * separator - is separator, which be placed between data
    */
    Writer(std :: unique_ptr<std :: ofstream> ofs, std :: string separator = " ") : ofsp(std::move(ofs)), separator(separator)
    {
    }


    /**
    * This function read data from value in file
    */
    void write(T value) const
    {
        (*ofsp) << value << separator;
    }


    /**
    * You may call this function to close stream
    */
    void close()
    {
        ofsp->close();
    }
};


/**
*This class provides base realisation of reading binary data from ifstream
* Template is from type of data to read
*/
template <class T>
class BinaryReader
{
private:
    std :: unique_ptr<std :: ifstream> ifsp;
public:
    /**
    * Constructor of class std :: need unique_ptr<std :: ifstream>
    * This class will delete ifstream by itself
    */
    BinaryReader(std :: unique_ptr<std :: ifstream> ifs) : ifsp(std::move(ifs))
    {
    }
    /**
    * This function read data from file in value
    */
    bool read(T &value)
    {
        return (ifsp->read(reinterpret_cast<char*>(&value), sizeof (T)));
    }

    /**
    * You may call this function to close stream
    */
    void close()
    {
        ifsp->close();
    }
};


/**
*This class provides base realisation of writing binary data from ifstream
* Template is from type of data to write
*/
template <class T>
class BinaryWriter
{
private:
    std :: unique_ptr<std :: ofstream> ofsp;
public:
    /**
    * Constructor of class std :: need unique_ptr<std :: ofstream>
    * This class will delete ofstream by itself
    */
    BinaryWriter(std :: unique_ptr<std :: ofstream> ofs) : ofsp(std :: move(ofs))
    {
    }


    /**
    * This function read data from value in file
    */
    void write(T &value)
    {
        ofsp->write(reinterpret_cast<char*>(&value), sizeof value);
    }

    /**
    * You may call this function to close stream
    */
    void close()
    {
        ofsp->close();
    }
};

#endif
