#include <iostream>
#include "Input.h"
#include <vector>
#include "Genetic.h"

using namespace std;

int main(int argc, char *argv[])
{
    Input input = Input::readInput(argv[1]);

    vector<short> shortestFoundPath;
    Genetic genetic(stoi(argv[2]), stof(argv[3]), stof(argv[4]));
    //Genetic genetic(1000, 0.9, 0.01);
    shortestFoundPath = genetic.findShortestHCycle(input.adjacencyMatrix, input.nOfCities,
        input.shortestHCycleLength);
    return 0;
    for(int it = 0; it < shortestFoundPath.size(); ++it){
        cout << shortestFoundPath[it] << " ";
    }
    cout << "\ncost: " << Genetic::getPathCost(input.adjacencyMatrix, shortestFoundPath);
    string b;
    cin >> b;
    return 0;
}
