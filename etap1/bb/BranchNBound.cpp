#include "BranchNBound.h"
#include <iostream>
#include <climits>
#include <queue>

inline int BranchNBound::findLowestValInRow(std::vector<std::vector<int>> &matrix, int row, int nOfColumns) {
    int lowestVal = INT_MAX;
    for(int col = 0; col < nOfColumns; ++col) {
        if(matrix[row][col] < lowestVal && row != col) {
            lowestVal = matrix[row][col];
            if(lowestVal == 0){
                break;
            }
        }
    }
    return (lowestVal == INT_MAX)? 0 : lowestVal;
}

inline int BranchNBound::findLowestValInCol(std::vector<std::vector<int>> &matrix, int col, int nOfRows) {//TODO
    int lowestVal = INT_MAX;
    for(int row = 0; row < nOfRows; ++row) {
        if(matrix[row][col] < lowestVal && row != col) {
            lowestVal = matrix[row][col];
            if(lowestVal == 0){
                break;
            }
        }
    }
    return (lowestVal == INT_MAX)? 0: lowestVal;
}

inline void BranchNBound::reduceCol(std::vector<std::vector<int>> &matrix, int col, int nOfRows, int lowestVal) {
    for(int it = 0; it < nOfRows; ++it) {
        if(matrix[it][col] != INT_MAX && it != col) {
            matrix[it][col] -= lowestVal;
        }
    }
}

inline void BranchNBound::reduceRow(std::vector<std::vector<int>> &matrix, int row, int nOfColumns, int lowestVal) {//TODO
    for(int it = 0; it < nOfColumns; ++it) {
        if(matrix[row][it] != INT_MAX && it != row) {
            matrix[row][it] -= lowestVal;
        }
    }
}

int BranchNBound::reduceMatrix(std::vector<std::vector<int>> &matrix, int n) {
    int reductionCost = 0;
    //redukcja wierszy
    int lowestVal;
    for(int it = 0; it < n; ++it) {
        lowestVal = findLowestValInRow(matrix, it, n);
        reductionCost += lowestVal;
        if(lowestVal != 0) {
            reduceRow(matrix, it, n, lowestVal);
        }
    }
    //redukcja kolumn
    for(int it = 0; it < n; ++it) {
        lowestVal = findLowestValInCol(matrix, it, n);
        reductionCost += lowestVal;
        if(lowestVal != 0) {
            reduceCol(matrix, it, n, lowestVal);
        }
    }
    return reductionCost;
}

void BranchNBound::markEdge(std::vector<std::vector<int>> &matrix, int n, int cityFrom, int cityTo){
    for(int it = 0; it < n; ++it) {
        matrix[cityFrom][it] = INT_MAX;//wiersz miasta z ktorego wychodzi
        matrix[it][cityTo] = INT_MAX;//kolumna miasta do ktorego wchodzi
    }
    matrix[cityTo][cityFrom] = INT_MAX;//krawedz powrotna
}


std::vector<int> BranchNBound::findShortestHCycle(std::vector<std::vector<int>> &adjMatrix, int nOfNodes, int startNode, int lightestHCycle){
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> nodeQ;
    int UB = INT_MAX;
    std::vector<int> currBestPath;
    std::string firstLeafUB = "g";

    nodeQ.push(Node(adjMatrix, nOfNodes, 0));
    Node buff, branchedNode;
    while(!nodeQ.empty()){
        buff = nodeQ.top();
        nodeQ.pop();
        if(buff.lowerBound < UB){//dla przedawnionych wierzcholkow w kolejce, o LB >= UB
            if(buff.lengthOfPath < nOfNodes){
                for(int col = 0; col < nOfNodes; ++col) {
                    if(buff.reducedMatrix[buff.city][col] != INT_MAX && col != startNode) {//branching
                        branchedNode = Node(&buff, col, adjMatrix);
                        if(branchedNode.lowerBound < UB) {
                            nodeQ.push(branchedNode);
                        }
                    }
                }
            }else {
                branchedNode = Node(&buff, 0, adjMatrix);
                if(branchedNode.costOfPath < UB) {
                    UB = branchedNode.costOfPath;
                    int prd = 100 * (UB - lightestHCycle)/lightestHCycle;
                    std::cout << UB << ", PRD = " << prd << "%\n";
                    if(firstLeafUB.at(0) == 'g') {
                        firstLeafUB = std::to_string(UB) + ", PRD = " + std::to_string(prd) + "%\n";
                    }
                    currBestPath = branchedNode.path;
                }
            }
        }
    }
    std::cout << firstLeafUB << std::endl;
    return currBestPath;
}

Node::Node(Node* node, int latestCity, std::vector<std::vector<int>> &adjMatrix){
    this->city = latestCity;
    this->path = node->path;
    this->path.push_back(latestCity);
    this->n = node->n;
    this->lowerBound = node->reducedMatrix[node->city][latestCity] + node->lowerBound;
    this->reducedMatrix = node->reducedMatrix;
    for(int row = 0; row < n; ++row){
        this->reducedMatrix[row] = node->reducedMatrix[row];
    }
    this->lowerBound += BranchNBound::reduceMatrix(this->reducedMatrix, n);
    BranchNBound::markEdge(this->reducedMatrix, n, node->city, latestCity);
    this->lengthOfPath = node->lengthOfPath + 1;
    this->costOfPath = node->costOfPath + adjMatrix[node->city][latestCity];
}

Node::Node(std::vector<std::vector<int>> &matrix, int n, int latestCity) {
    this->city = latestCity;
    this->n = n;
    this->path.reserve(n);
    this->path.push_back(latestCity);
    this->reducedMatrix = matrix;
    for(int row = 0; row < n; ++row){
        this->reducedMatrix[row] = matrix[row];
    }
    this->lowerBound = BranchNBound::reduceMatrix(this->reducedMatrix, n);
    this->lengthOfPath = 1;
    this->costOfPath = 0;
}

std::string Node::toString(){
    std::string str;
    for(int it = 0; it < this->lengthOfPath; ++it){
        str += std::to_string(this->path[it]);
        str += "->";
    }
    str += " | LB = ";
    str += std::to_string(this->lowerBound);
    str += " | costOfPath = ";
    str += std::to_string(this->costOfPath);
    str += "\n";
    return str;
}

Node::Node() {}

Node::~Node() {
    //std::cout << "deconstructor" << std::endl;
}
