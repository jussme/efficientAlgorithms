#include "TabuSearch.h"
#include <climits>
#include <random>
#include <algorithm>
#include <chrono>

#include <time.h>

#include <iostream>

using std::vector;

void TabuSearch::Printer::printSync(std::string input){
    std::lock(this->inserting, this->inserting2);
    std::cout << input;
    this->inserting.unlock();
    this->inserting2.unlock();
}

TabuSearch::Timer::Timer(int s){
    this->period = s;
    this->startEpoch = std::time(nullptr);
}

inline bool TabuSearch::Timer::elapsed() {
    if(std::time(nullptr) - this->startEpoch > this->period){
        return true;
    }
    return false;
}

inline void swap_(int g, int h, vector<int>& path){
    int buff = path[g];
    path[g] = path[h];
    path[h] = buff;
}

inline int getSwappedCost(int** adjMatrix, int g, int h, int n, vector<int>& path, int pathCost){
    pathCost -= adjMatrix[ path[g-1] ][ path[g] ];
    pathCost += adjMatrix[ path[g-1] ][ path[h] ];

    pathCost -= adjMatrix[ path[h] ][ path[h+1] ];
    pathCost += adjMatrix[ path[g] ][ path[h+1] ];
    //czy zamieniane miasta sasiaduja?
    if(g + 1 != h){//nie sasiaduja
        pathCost -= adjMatrix[ path[g] ][ path[g+1] ];
        pathCost += adjMatrix[ path[h] ][ path[g+1] ];

        pathCost -= adjMatrix[ path[h-1] ][ path[h] ];
        pathCost += adjMatrix[ path[h-1] ][ path[g] ];
    } else {//sasiaduja
        pathCost -= adjMatrix[ path[g] ][ path[h] ];
        pathCost += adjMatrix[ path[h] ][ path[g] ];
    }
    return pathCost;
}

vector<int> TabuSearch::genPathBFS(int** adjMatrix, int n, int start){
    vector<int> path = vector<int>();
    path.reserve(n + 1);
    vector<bool> visited = vector<bool>();
    visited.resize(n, false);
    int currCity = start;
    visited[currCity] = true;
    path.push_back(currCity);
    int closestCurrentCityIndex = 0;
    int closestCurrentCityDist = INT_MAX;

    for(int pathLength = 1; pathLength < n; ++pathLength){
        closestCurrentCityDist = INT_MAX;
        for(int dest = 0; dest < n; ++dest){
            if(adjMatrix[currCity][dest] < closestCurrentCityDist && visited[dest] == false){
                closestCurrentCityDist = adjMatrix[currCity][dest];
                closestCurrentCityIndex = dest;
            }
        }
        currCity = closestCurrentCityIndex;
        visited[currCity] = true;
        path.push_back(currCity);
    }
    path.push_back(start);
    return path;
}

vector<int> TabuSearch::genPathRand(int n){
    vector<int> path;
    path.reserve(n + 1);
    for(int it = 0; it < n; ++it) {
        path.push_back(it);
    }

    std::mt19937 generator(std::chrono::high_resolution_clock::now().time_since_epoch().count());

    std::shuffle(path.begin(), path.end(), generator);
    path.push_back(path[0]);

    return path;
}

vector<int> TabuSearch::findShortestHCycle(int** adjMatrix, int n, int memoryLength,
int itWithNoImprovLimit, int lightestHCycle, int threadN, int start)
{

    TabuSearch::Timer timer(60);
    std::string firstImprov = "g";
    Printer printer;

    //ile jeszcze iteracji, kombinacja operatorow jest tabu?
    vector<vector<int>> tabuOperands = vector<vector<int>>();
    tabuOperands.resize(n, vector<int>());
    for(int it = 0; it < n; ++it){
        tabuOperands[it].resize(n, 0);
    }
    vector<int> bestCurrPath;
    int bestCurrCost = INT_MAX;
    vector<int> currPath = genPathBFS(adjMatrix, n, start);
    int currCost = getPathCost(adjMatrix, currPath);
    int buffCost;

    int bestFoundCost;
    int bestFoundG;
    int bestFoundH;

    bool foundAnyNeighbour;
    int itWithNoImprov = 0;
    int iteration = 0;

    while(!timer.elapsed()) {
        //dywersyfikacja
        if(itWithNoImprov >= itWithNoImprovLimit){
            itWithNoImprov = 0;
            currPath = genPathRand(n);
            currCost = getPathCost(adjMatrix, currPath);
        }

        //szukanie najlepszego sasiada
        bestFoundCost = INT_MAX;
        foundAnyNeighbour = false;
        for(int g = 1; g < n - 1; ++g){
            for(int h = g + 1; h < n; ++h){
                if(tabuOperands[g][h] == 0
                   && (buffCost = getSwappedCost(adjMatrix, g, h, n, currPath, currCost)) < bestFoundCost){
                    bestFoundCost = buffCost;
                    bestFoundG = g;
                    bestFoundH = h;
                    foundAnyNeighbour = true;
                }
            }
        }

        if(foundAnyNeighbour){
            swap_(bestFoundG, bestFoundH, currPath);
            currCost = bestFoundCost;
            tabuOperands[bestFoundG][bestFoundH] = memoryLength;
            //tabuOperands[h][g] = memoryLength;//niepotrzebne
        } else {//wszystkie przejscia sa tabu
            for(int it = 0; it < n; ++it){std::cout << "aspiracja" << std::endl;
                for(int itt = 0; itt < n; ++itt){
                    if(tabuOperands[it][itt] > 0){
                        --tabuOperands[it][itt];
                    }
                }
            }
        }

        //czy znaleziony najlepszy lokalny sasiad jest lepszy od najlepszego znalezionego rozwiazania
        if(currCost < bestCurrCost) {
            bestCurrPath = currPath;
            bestCurrCost = currCost;
            float prd = 100 * (bestCurrCost - lightestHCycle)/(float)lightestHCycle;
            std::string feed = std::to_string(iteration) +  "@" + std::to_string(threadN) + "\t\t" +
             std::to_string(bestCurrCost) + "\tPRD = " + std::to_string(prd) + "%\n";
            printer.printSync(feed);
            if(firstImprov.at(0) == 'g') {
                firstImprov = feed;
            }
        } else {
            ++itWithNoImprov;
        }

        //zapominanie
        for(int it = 0; it < n; ++it){
            for(int itt = 0; itt < n; ++itt){
                if(tabuOperands[it][itt] > 0){
                    --tabuOperands[it][itt];
                }
            }
        }
        ++iteration;
    }
    printer.printSync(firstImprov);
    return bestCurrPath;
}


int TabuSearch::getPathCost(int** adjMatrix, vector<int>& path){
    int cost = 0;
    int prevCity = path[0];
    for(int it = 1; it < path.size(); ++it){
        cost += adjMatrix[prevCity][path[it]];
        prevCity = path[it];
    }
    return cost;
}
