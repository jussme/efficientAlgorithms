#include "FileReader.h"
#include <iostream>

using namespace std;

FileReader::FileReader(string filePath)
{
    fstream* file = new fstream();
    (*file).open(filePath, ios::in);
    if((*file).good())
    {
        this->fileStreamPointer = file;
        this->fileOperational = true;
    }
}

bool FileReader::isOperational()
{
    return this->fileOperational && this->fileStreamPointer != nullptr && (*this->fileStreamPointer).good();
}

string FileReader::readToWhiteSpace()
{
    if(isOperational())
    {
        string strr;
        (*this->fileStreamPointer) >> strr;
        return strr;
    }

    return nullptr;
}

void FileReader::closeFile()
{
    if(this->fileStreamPointer != nullptr && (*this->fileStreamPointer).is_open() )
    {
        (*this->fileStreamPointer).close();
        delete this->fileStreamPointer;
        this->fileOperational = false;
    }
}
