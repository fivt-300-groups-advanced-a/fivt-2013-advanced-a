#ifndef FILECOMMUNICATOR_H
#define FILECOMMUNICATOR_H

#include <cstdio>

class FileCommunicator
{
public:
    explicit FileCommunicator(FILE *file):
        _file(file),
        _ownFile(false)
    {}
    FileCommunicator(const char *fileName, const char *openMode):
        _file(fopen(fileName, openMode)),
        _ownFile(true)
    {}
    ~FileCommunicator()
    {
        clearOwn();
    }

    FILE * file() const { return _file; }
    void setFile(FILE *file)
    {
        clearOwn();
        _file = file;
    }

    void clearOwn()
    {
        if (_ownFile)
        {
            fclose(_file);
            _file = 0;
            _ownFile = false;
        }
    }

protected:
    void setOwnFile(FILE *file)
    {
        setFile(file);
        _ownFile = true;
    }
private:
    FILE *_file;
    bool _ownFile;
};

#endif // FILECOMMUNICATOR_H
