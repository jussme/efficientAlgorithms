#include "Input.h"
#include "FileReader.h"
#include <climits>

Input::Input(std::string filename,
        short nOfCities,
        int shortestHCycleLength,
        short** adjacencyMatrix) {
    this->filename = filename;
    this->nOfCities = nOfCities;
    this->shortestHCycleLength = shortestHCycleLength;
    this->adjacencyMatrix = adjacencyMatrix;
    }

Input::~Input() {
    for(short row = 0; row < this->nOfCities; ++row) {
        delete[] this->adjacencyMatrix[row];
    }
    delete[] this->adjacencyMatrix;
}

Input Input::readInput(std::string filePath) {
    FileReader reader(filePath);
    std::string filename;
    short nOfCities, **adjacencyMatrix;
    int shortestHCycleLength;

    filename = reader.readToWhiteSpace();
    nOfCities = stoi(reader.readToWhiteSpace());
    adjacencyMatrix = new short*[nOfCities];
    for(short row = 0; row < nOfCities; ++row) {
        adjacencyMatrix[row] = new short[nOfCities];
    }
    for(short row = 0; row < nOfCities; ++row) {
        for(short col = 0; col < nOfCities; ++col) {//dodane
            adjacencyMatrix[row][col] = stoi(reader.readToWhiteSpace());
            if(row == col) {
                adjacencyMatrix[row][col] = INT_MAX;
            }
        }
    }
    shortestHCycleLength = stoi(reader.readToWhiteSpace());

    return Input(filename,
                    nOfCities,
                    shortestHCycleLength,
                    adjacencyMatrix);
}

std::string Input::toString() {
    std::string matrixStr;
    int dist;
    for(short row = 0; row < this->nOfCities; ++row) {
        matrixStr += "\n\n" + std::to_string(row + 1) + ": ";
        for(short col = 0; col < this->nOfCities; ++col) {
            if((dist = this->adjacencyMatrix[row][col]) != 0) {
                matrixStr += "-" + std::to_string(this->adjacencyMatrix[row][col]) + "->" + std::to_string(col + 1) + ", ";
            }
        }
    }
    return "Filename: " + this->filename + "\nNumber of cities: " +
        std::to_string(this->nOfCities) + "\nShortest H. cycle length: " +
        std::to_string(this->shortestHCycleLength) + matrixStr;
}
