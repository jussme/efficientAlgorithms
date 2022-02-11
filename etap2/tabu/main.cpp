#include <iostream>
#include "TabuSearch.h"
#include "Input.h"
#include <thread>
#include <future>

using namespace std;

int main(int argc, char *argv[])
{
    /*if(argc == 0){
        cout << "pass: <filePath> <threadAmount> <memoryLength> <noImprovItLimit>" << endl;
    }
    Input input = Input::readInput(argv[1]);
    */Input input = Input::readInput("burma14.tsp");
    /*
    //int nOfThreads = argv[2];
    int nOfThreads = 4;
    std::vector<std::future<std::vector<int>>> threads;
    for(int it = 0; it < nOfThreads; ++it){
        threads.push_back(std::async(TabuSearch::findShortestHCycle,
            input.adjacencyMatrix,
            input.nOfCities,
            input.nOfCities,//memory length
            input.nOfCities*input.nOfCities*input.nOfCities,//limit bez poprawy globalnej
            input.shortestHCycleLength,
            it,
            it * (input.nOfCities/nOfThreads)
        ));
    }

    int lowestFoundCost = INT_MAX;
    vector<int> shortestFoundPath;
    vector<int> buff;
    for(int it = 0; it < threads.size(); ++it){
        if(TabuSearch::getPathCost(input.adjacencyMatrix ,buff = threads[it].get()) < lowestFoundCost){
            lowestFoundCost = TabuSearch::getPathCost(input.adjacencyMatrix , buff);
            shortestFoundPath = buff;
        }
    }*/

    vector<int> shortestFoundPath;
    shortestFoundPath = TabuSearch::findShortestHCycle(input.adjacencyMatrix,
        input.nOfCities,
        input.nOfCities,
        input.nOfCities * input.nOfCities * input.nOfCities,
        input.shortestHCycleLength,
        1,
        0
    );

    for(int it = 0; it < shortestFoundPath.size(); ++it){
        cout << shortestFoundPath[it] << " ";
    }
    cout << "\ncost: " << TabuSearch::getPathCost(input.adjacencyMatrix, shortestFoundPath);
    string b;
    cin >> b;
    return 0;
}
