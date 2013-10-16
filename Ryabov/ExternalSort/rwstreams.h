#ifndef RWSTREAMS_H
#define RWSTREAMS_H
#include <fstream>
#include <string>
#include <memory>

template <class T>
class Reader
{
private:
    std :: unique_ptr<std :: ifstream> ifsp;
    //std :: ifstream* ifs;
public:
    Reader(std :: unique_ptr<std :: ifstream> ifs) : ifsp(std::move(ifs))
    {
    }
    bool read(T &value) const
    {
        return ((*ifsp) >> value);
    }
    void close()
    {
        ifsp->close();
    }
};

template <class T>
class Writer
{
private:
    std :: unique_ptr<std :: ofstream> ofsp;
    std :: string separator;
public:
    Writer(std :: unique_ptr<std :: ofstream> ofs, std :: string separator = " ") : ofsp(std::move(ofs)), separator(separator)
    {
    }
    void write(T value) const
    {
        (*ofsp) << value << separator;
    }
    void close()
    {
        ofsp->close();
        //delete ofs;
    }
};

template <class T>
class BinaryReader
{
private:
    std :: unique_ptr<std :: ifstream> ifsp;
public:
    BinaryReader()
    {
    }
    BinaryReader(std :: unique_ptr<std :: ifstream> ifs) : ifsp(std::move(ifs))
    {
    }
    bool read(T &value)
    {
        return (ifsp->read(reinterpret_cast<char*>(&value), sizeof (T)));
    }
    void close()
    {
        ifsp->close();
        //delete ifs;
    }
};

template <class T>
class BinaryWriter
{
private:
    std :: unique_ptr<std :: ofstream> ofsp;
public:
    BinaryWriter(std :: unique_ptr<std :: ofstream> ofs) : ofsp(std :: move(ofs))
    {
    }
    void write(T &value)
    {
        ofsp->write(reinterpret_cast<char*>(&value), sizeof value);
    }
    void close()
    {
        ofsp->close();
    }
};
#endif
