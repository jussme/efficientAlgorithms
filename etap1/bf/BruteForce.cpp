#include "BruteForce.h"
#include <iostream>
#include <climits>

int optimalHCycleWeight;
int nOfNodes;
std::string str = "g";

void BruteForce::findShortestHCycleP(int** adjMatrix, Path* currPath, bool* traversedMatrix, Path** currLightestPath) {
    if(currPath->currLength == nOfNodes) {
        currPath->addNode(currPath->nodes[0], adjMatrix);
        int buffCurrWeight = currPath->weight;
        if(buffCurrWeight < (*currLightestPath)->weight) {
            int prd = 100 * (buffCurrWeight - optimalHCycleWeight)/optimalHCycleWeight;
            if(str.at(0) == 'g'){
                str = std::to_string(buffCurrWeight) + ", PRD = " + std::to_string(prd) + "%\n";
            }
            std::cout << std::to_string(buffCurrWeight) + ", PRD = " + std::to_string(prd) + "%\n";
            delete *currLightestPath;
            *currLightestPath = new Path(*currPath);
        }
        currPath->removeTopNode();
        return;
    }

    int currBranchingNode = currPath->nodes[currPath->currLength - 1];
    for(int node = 0; node < nOfNodes; ++node) {
        if(!traversedMatrix[node] && currBranchingNode != node) {
            currPath->addNode(node, adjMatrix[currBranchingNode][node]);
            traversedMatrix[node] = true;
            findShortestHCycleP(adjMatrix, currPath, traversedMatrix, currLightestPath);
            currPath->removeTopNode();
            traversedMatrix[node] = false;
        }
    }

    return;
}

Path* BruteForce::findShortestHCycle(int** adjMatrix, int nOfNodesAr, int startNode, int lightestHCycle) {
    optimalHCycleWeight = lightestHCycle;
    nOfNodes = nOfNodesAr;
    bool* passedMatrix = new bool[nOfNodes];
    for(int node = 0; node < nOfNodes; ++node) {
        passedMatrix[node] = false;
    }
    passedMatrix[startNode] = true;
    Path currPath(nOfNodes + 1);
    currPath.addNode(startNode, 0);
    Path* currLightestPath = new Path(1);
    currLightestPath->addNode(0, INT_MAX);
    findShortestHCycleP(adjMatrix, &currPath, passedMatrix, &currLightestPath);
    delete[] passedMatrix;
    std::cout << str << std::endl;
    return currLightestPath;
}

inline void Path::addNode(int node, int weight) {
    this->nodes[this->currLength] = node;
    this->weights[this->currLength] = weight;
    this->weight += weight;
    this->currLength += 1;
}

void Path::addNode(int node, int** adjMatrix) {
    this->addNode(node, adjMatrix[this->nodes[this->currLength - 1]][node]);
}

inline void Path::removeTopNode() {
    this->currLength -= 1;
    this->weight -= this->weights[this->currLength];
}

std::string Path::toString() {
    std::string path;
    for(int it = 0; it < this->currLength; ++it) {
        path += "->" + std::to_string(this->nodes[it]);
    }
    /*path += "\n";
    for(int it = 0; it < this->currLength; ++it) {
        path += "->" + std::to_string(this->weights[it]);
    }*/
    return path;
}
/*
Path::Path(const Path &pathO) {
    this->currLength = pathO.currLength;
    this->nodes = std::vector<int>(pathO.nodes); nie kopiuje zawartosci
    this->weights = std::vector<int>(pathO.weights); -||-
    this->weight = pathO.weight;
}
*/
Path::Path(const Path &pathO) {
    this->currLength = pathO.currLength;
    this->nodes.reserve(pathO.nodes.capacity());
    this->weights.reserve(pathO.nodes.capacity());
    for(int it = 0; it < pathO.nodes.capacity(); ++it) {//mozna bo sa takiej samej wielkosci
        this->nodes.push_back(pathO.nodes[it]);
        this->weights.push_back(pathO.weights[it]);
    }
    this->weight = pathO.weight;
}

//unikniecie powiekszania przy push_back; aktualnie, dla [] dalej potrzeba inicjalizacji
Path::Path(int reservedLength) {
    this->nodes.reserve(reservedLength);
    this->weights.reserve(reservedLength);
}
