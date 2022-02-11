#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED
#include <string>

class Input {
public:
    std::string filename;
    short nOfCities;
    int shortestHCycleLength;
    short** adjacencyMatrix;

    Input(std::string filename,
        short nOfCities,
        int shortestHCycleLength,
        short** adjacencyMatrix);
    ~Input();

    static Input readInput(std::string filepath);

    std::string toString();
};

#endif // INPUT_H_INCLUDED
