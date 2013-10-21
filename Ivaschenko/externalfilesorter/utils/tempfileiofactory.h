#ifndef FILEIOFACTORY_H
#define FILEIOFACTORY_H

#include <memory>

#include <cstdio>

/**
 * Factory class that controlls creating Writers to write data on the disk and Readers to read it again
 * All readers and writers will be closed properly (destructors will be called)
 */
template< typename Reader, typename Writer> class TempFileIOFactory
{
	public:
		TempFileIOFactory(): readersCnt(0) {}

		/*
		 * Deleting all temporary files
		 */
		~TempFileIOFactory()
		{
			for (auto file : fileNames)
				unlink(file.c_str());
		}

		/*
		 * Open reader corresponding to the next writer
		 */
		std::unique_ptr<Reader> openReader()
		{
			assert(readersCnt < fileNames.size());
			return std::unique_ptr<Reader>(new Reader(fileNames[readersCnt++]));
		}

		/*
		 * Create new writer. Temporary file will be created by std::tmpnam function
		 */
		std::unique_ptr<Writer> openWriter()
		{
			fileNames.push_back(std::string(std::tmpnam(nullptr)));
			return std::unique_ptr<Writer>(new Writer(fileNames.back()));
		}

	private:
		std::vector<std::string> fileNames;
		std::size_t readersCnt;
};

#endif // FILEIOFACTORY_H
