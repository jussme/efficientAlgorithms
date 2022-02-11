#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED
#include <string>

class Input {
public:
    std::string filename;
    int nOfCities;
    int shortestHCycleLength;
    int** adjacencyMatrix;

    Input(std::string filename,
        int nOfCities,
        int shortestHCycleLength,
        int** adjacencyMatrix);
    ~Input();

    static Input readInput(std::string filepath);

    std::string toString();
};

#endif // INPUT_H_INCLUDED
