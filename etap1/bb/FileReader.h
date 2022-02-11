#ifndef FILEREADER_INCLUDED
#define FILEREADER_INCLUDED

#include <string>
#include <fstream>

class FileReader {
private:
    std::fstream* fileStreamPointer = nullptr;
    bool fileOperational = false;

public:
    FileReader(std::string filePath);

    bool isOperational();
    std::string readToWhiteSpace();
    void closeFile();
};
#endif
