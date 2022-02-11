#include <iostream>
#include "Input.h"
#include <string>
#include <time.h>
#include <conio.h>
#include "BranchNBound.h"

void displayBB(std::string filename){
    Input input = Input::readInput(filename);
    std::vector<std::vector<int>> newMatrix;
    newMatrix.reserve(input.nOfCities);
    for(int it = 0; it < input.nOfCities; ++it){
        newMatrix.push_back(*new std::vector<int>);
        newMatrix[it].reserve(input.nOfCities);
        for(int itt = 0; itt < input.nOfCities; ++itt){
            newMatrix[it].push_back(input.adjacencyMatrix[it][itt]);
        }
    }
    int start = time(nullptr);
    std::vector<int> path = BranchNBound::findShortestHCycle(newMatrix, input.nOfCities, 0, input.shortestHCycleLength);
    int prev = 0;
    for(int it = 0; it <= input.nOfCities; ++it) {
        std::cout << " -";
        //std::cout << ((input.adjacencyMatrix[prev][path[it]] == INT_MAX)? 0 : input.adjacencyMatrix[prev][path[it]]);
        std::cout << ">" + std::to_string(path[it]);
        prev = path[it];
    }
    std::cout << "\ntime: " + std::to_string(time(nullptr) - start) + " [s]";

    getch();
}

int main(int argc, char *argv[])
{
    displayBB(argv[1]);
    return 0;
}
