#include "Genetic.h"
#include <climits>
#include <chrono>
//#include <random>
#include <algorithm>
#include <iostream>
#include <thread>

using namespace std;
//pc 0.6 - 0.75
//pm 0.01 - 1
//Sterna podal dla mutacji wsp 0.01 krzyzowanie wsp 0.8
Genetic::Timer::Timer(int s){
    this->period = s;
    this->startEpoch = std::time(nullptr);
}

inline bool Genetic::Timer::elapsed() {
    if(std::time(nullptr) - this->startEpoch > this->period){
        return true;
    }
    return false;
}

vector<short> Genetic::genPathBFS(short** adjMatrix, short n, short start){
    vector<short> path = vector<short>();
    path.reserve(n);
    vector<bool> visited = vector<bool>();
    visited.resize(n, false);
    int currCity = start;
    visited[currCity] = true;
    path.push_back(currCity);
    short closestCurrentCityIndex = 0;
    short closestCurrentCityDist = SHRT_MAX;

    for(short pathLength = 1; pathLength < n; ++pathLength){
        closestCurrentCityDist = SHRT_MAX;
        for(short dest = 0; dest < n; ++dest){
            if(adjMatrix[currCity][dest] < closestCurrentCityDist && visited[dest] == false){
                closestCurrentCityDist = adjMatrix[currCity][dest];
                closestCurrentCityIndex = dest;
            }
        }
        currCity = closestCurrentCityIndex;
        visited[currCity] = true;
        path.push_back(currCity);
    }
    //path.push_back(start);
    return path;
}

void shiftPathTo(vector<short> &path, int startingVal){
    int startingValIndex = 0;
    int n = path.size();
    for (int it = 0; it < n; ++it){
        if (path[it] == startingVal){
            startingValIndex = it;
            break;
        }
    }
    short currFirstEl = -1;
    while(path[0] != startingVal){
        currFirstEl = path[0];
        for(int it = 0; it < n-1; ++it){
            path[it] = path[it + 1];
        }
        path[n-1] = currFirstEl;
    }
}

vector<short> Genetic::genPathRand(short n){
    vector<short> path;
    //path.reserve(n + 1);
    path.reserve(n);
    for(short it = 0; it < n; ++it) {
        path.push_back(it);
    }

    //std::mt19937 generator(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    //random_device rd;
    //std::mt19937 generator(this->rd());
    std::shuffle(path.begin(), path.end(), this->generator);
    //path.push_back(path[0]);

    return path;
}

Genetic::Genetic(int populationSize, float crossoverRate, float mutationRate){
    this->populationSize = populationSize;
    this->crossoverRate = crossoverRate;
    this->mutationRate = mutationRate;
    random_device rd;
    this->generator = std::mt19937(rd());
    this->firstImprov = "g";
}

void Genetic::inversion(vector<short> &path) {
    uniform_real_distribution<float> urd(1, path.size()/2.0f);//bylo (0,
    float pos1 = urd(this->generator);
    float pos2 = urd(this->generator);

    reverse(path.begin() + round(pos1), path.begin() + round(pos1 + pos2));
    //shiftPathTo(path, 0);//todo potrzebne?
}

void Genetic::swapMutation(vector<short>& path){
    uniform_real_distribution<float> urd(1, path.size());
    int pos1 = urd(this->generator);
    int pos2 = urd(this->generator);

    short buff = path[pos2];
    path[pos2] = path[pos1];
    path[pos1] = buff;
}

int counter = 0;
double Genetic::reportNewFitness(vector<short> path, double fitness, int pathCost){
    if(fitness > this->highestFitness){
        this->highestFitness = fitness;
        this->bestCurrPath = path;
        //raport
        float prd = 100 * (pathCost - this->lightestHCycle)/(float)this->lightestHCycle;
        string feed = to_string(iteration) + ".\t\t" +
            to_string(pathCost) + "\tPRD = " + to_string(prd) + "%\n";
        cout << feed;
        if(this->firstImprov.at(0) == 'g') {
            this->firstImprov = feed;
        }
    }
    //cout <<"fitness = "<<fitness << ", cost = "<< pathCost<< endl;
    return fitness;
}

void Genetic::initPopulation(short** adjMatrix, int populationSize, int n){
    vector<short> generatedPath;
    int pathCost = -1;
    for(int it = 0; it < n && it < populationSize; ++it){//generacja bfs startujac w kazdym z miast w <0 ... n-1>
        generatedPath = genPathBFS(adjMatrix, n, it);
        shiftPathTo(generatedPath, 0);
        pathCost = getPathCost(adjMatrix, generatedPath);
        this->population.push_back(pair<vector<short>, double>(generatedPath, reportNewFitness(generatedPath, 1.0/pathCost, pathCost)));
    }

    while(this->population.size() < populationSize){//generacja losowych, przesuwanych w lewo do startowego miasta 0
        generatedPath = genPathRand(n);
        shiftPathTo(generatedPath, 0);
        pathCost = getPathCost(adjMatrix, generatedPath);
        this->population.push_back(pair<vector<short>, double>(generatedPath, reportNewFitness(generatedPath, 1.0/pathCost, pathCost)));
    }
}

int Genetic::indexOf(vector<short> &vec, short val){
    for(int it = 0; it < vec.size(); ++it){
        if(vec[it] == val){
            return it;
        }
    }
    return -1;
}

bool checkUnique(vector<short> &path){
    for(int it = 0; it < path.size(); ++it){
        for(int itt = 0; itt < path.size(); ++itt){
            if(it != itt && path[itt] == path[it]){
                return false;
            }
        }
    }
    return true;
}

pair<vector<short>, vector<short>> Genetic::pmx(vector<short> &p1, vector<short> &p2){
    uniform_real_distribution<float> urd(0, p1.size()/2.0f);
    //index oznacza przed ktorym indeksem jest granica, +1 aby moc kopiowac caly chromosom
    //ooooooo|index1oooooo|index2oooooo

    int index1 = (int)urd(this->generator);
    int index2 = (urd(this->generator) + index1);

    vector<short> child1;
    child1.resize(p1.size(), -1);
    vector<short> child2;
    child2.resize(p1.size(), -1);

    for(int it = index1; it < index2; ++it){
        child1[it] = p2[it];
        child2[it] = p1[it];
    }

    for(int it = 0; it < child1.size(); ++it){
        if(child1[it] != -1){//obszar mapy
            continue;
        }

        int pickedVal = p1[it];
        int valIndex;
        while( (valIndex = indexOf(child1, pickedVal)) != -1){
            pickedVal = p1[valIndex];
        }
        child1[it] = pickedVal;
    }

    for(int it = 0; it < child2.size(); ++it){
        if(child2[it] != -1){//obszar mapy
            continue;
        }

        int pickedVal = p2[it];
        int valIndex;
        while( (valIndex = indexOf(child2, pickedVal)) != -1){
            pickedVal = p2[valIndex];
        }
        child2[it] = pickedVal;
    }

    return pair<vector<short>, vector<short>>(child1, child2);
}

vector<short> Genetic::findShortestHCycle(short** adjMatrix, short n, int lightestHCycle){
    this->lightestHCycle = lightestHCycle;
    this->iteration = 0;
    initPopulation(adjMatrix, this->populationSize, n);
    Timer timer(60);
    int currPopSize = this->populationSize;

    float multiplier = 0.85f;
    double highestNewPopFitness = 0;
    double highestOldPopFitness = this->highestFitness;

    uniform_real_distribution<float> urdCrossMut(0, 1);
    uniform_real_distribution<float> fromPopPick(0, this->populationSize);
    while(!timer.elapsed()){
        ++this->iteration;

        uniform_real_distribution<float> urdSel(this->highestFitness*0.8f, this->highestFitness);

        vector<short> firstSpecimen;
        int firstSpecimenIndex;
        pair<vector<short>, double> specimen;
        bool firstSpecimenSet = false;

        pair<vector<short>, vector<short>> children;
        int pathCost;
        double pathFitness;

        /*
        int belowThre = 0;
        int aboveThre = 0;
        double avgFitness = 0;
        for(int it = 0; it < this->population.size(); ++it){
            avgFitness += this->population[it].second;
            /*
            if(this->population[it].second < urdSel(this->generator)){
                ++belowThre;
                //cout << this->population[it].second << endl;
            }else{
                ++aboveThre;
                //cout << this->population[it].second << endl;
            }//
        }
        avgFitness /= this->population.size();
        cout << "avgFitness: " << avgFitness << endl;
        cout << "highestOldPopFitness: " << highestOldPopFitness<< endl;
        //cout << "highest: " << this->highestFitness << ", highest*0.5: " << this->highestFitness*0.5f << endl;
       //cout << "above: " << aboveThre<<", below: "<< belowThre << endl;
        string g; cin >> g;*/
        vector<pair<vector<short>, double>> newPopulation;
        newPopulation.reserve(this->population.size());
        int lowestNewPopPathCost = 0;
        while(newPopulation.size() < this->populationSize){
            int pick = fromPopPick(this->generator);
            //cout << pick <<endl;
            specimen = this->population[pick];
            if(specimen.second > urdSel(this->generator)){//zostal wylosowany

                if(this->crossoverRate > urdCrossMut(this->generator)){
                    if(firstSpecimenSet){
                        children = pmx(firstSpecimen, specimen.first);
                //mutacja
                        if (this->mutationRate > urdCrossMut(this->generator)){//mutuje1
                            inversion(children.first);

                        } else {
                            //nie mutuje1
                        }
                        pathCost = getPathCost(adjMatrix, children.first);
                        pathFitness = reportNewFitness(children.first, 1.0/pathCost, pathCost);
                        if (pathFitness > highestNewPopFitness){
                            highestNewPopFitness = pathFitness;
                            lowestNewPopPathCost = pathCost;
                            //cout << "newHigh1: " << highestNewPopFitness << " cuz of pathCost: " << pathCost<<endl;
                        }
                        if(pathFitness >= this->population[firstSpecimenIndex].second * 0.85){//highestOldPopFitness*0.5
                            newPopulation.push_back(pair<vector<short>, double>(children.first, pathFitness));
                        }

                        if (this->mutationRate > urdCrossMut(this->generator)){//mutuje2
                            inversion(children.second);

                        } else {
                            //nie mutuje2
                        }
                        pathCost = getPathCost(adjMatrix, children.second);
                        pathFitness = reportNewFitness(children.second, 1.0/pathCost, pathCost);
                        if (pathFitness > highestNewPopFitness){
                            highestNewPopFitness = pathFitness;
                            lowestNewPopPathCost = pathCost;
                            //cout << "newHigh2: " << highestNewPopFitness << " cuz of pathCost: " << pathCost<<endl;
                        }
                        if(pathFitness >= this->population[pick].second * 0.85){//
                            newPopulation.push_back(pair<vector<short>, double>(children.second, pathFitness));
                        }
                        firstSpecimenSet = false;
                    }else{
                        firstSpecimen = specimen.first;
                        firstSpecimenIndex = pick;
                        firstSpecimenSet = true;
                    }
                }else{//nie rozmnaza sie
                    newPopulation.push_back(specimen);
                }
            }
        }
        this->population = newPopulation;
        cout << "newHigh: " << highestNewPopFitness << " cuz of pathCost: " << lowestNewPopPathCost<<endl;
        highestOldPopFitness = highestNewPopFitness;
        highestNewPopFitness = 0;
    }
    cout << firstImprov << endl;
    return this->bestCurrPath;
}

double Genetic::getFitness(short** adjMatrix, vector<short>& path){
    return 1.0/getPathCost(adjMatrix, path);
}

int Genetic::getPathCost(short** adjMatrix, vector<short>& path){
    int cost = 0;
    for(short it = 1; it < path.size(); ++it){
        cost += adjMatrix[path[it-1]][path[it]];
    }
    cost += adjMatrix[path[path.size()-1]][path[0]];//od ostatniego do pierwszego
    //return 1.0f/cost;
    return  cost;
}
