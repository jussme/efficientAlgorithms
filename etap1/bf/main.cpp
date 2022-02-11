#include <iostream>
#include "Input.h"
#include "BruteForce.h"
#include <time.h>
#include <conio.h>

void displayBruteForce(std::string filename) {
    Input input = Input::readInput(filename);
    int start = time(nullptr);
    Path* path = BruteForce::findShortestHCycle(input.adjacencyMatrix, input.nOfCities, 0, input.shortestHCycleLength);
    std::cout << path->toString() << std::endl;
    std::cout << "\ntime: " + std::to_string(time(nullptr) - start) + " [s]";

    getch();
}

int main(int argc, char *argv[]) {
    displayBruteForce(argv[1]);

    return 0;
}
