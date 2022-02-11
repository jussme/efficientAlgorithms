#ifndef BRANCHNBOUND_H_INCLUDED
#define BRANCHNBOUND_H_INCLUDED

#include <vector>
#include <string>
#include "BranchNBound.h"

class Node{
public:
    Node();
    Node(Node* node, int latestCity, std::vector<std::vector<int>> &adjMatrix);
    Node(std::vector<std::vector<int>> &matrix, int n, int latestCity);
    ~Node();

    bool operator> (const Node node) const{
        //return ((float)this->lowerBound)/this->lengthOfPath > ((float)node.lowerBound)/node.lengthOfPath;
        return this->lowerBound/this->lengthOfPath > node.lowerBound/node.lengthOfPath;
    }

    std::string toString();

    //char
    std::vector<int> path;
    int city;
    int lengthOfPath;
    int costOfPath;
    int n;//rozmiar instancji problemu, ilosc miast; dla destuctor'a
    std::vector<std::vector<int>> reducedMatrix;
    int lowerBound;

};

class BranchNBound{
private:
    static int findLowestValInRow(std::vector<std::vector<int>> &matrix, int row, int nOfColumns);
    static int findLowestValInCol(std::vector<std::vector<int>> &matrix, int col, int nOfRows);
    static void reduceCol(std::vector<std::vector<int>> &matrix, int col, int nOfRows, int lowestVal);
    static void reduceRow(std::vector<std::vector<int>> &matrix, int row, int nOfColumns, int lowestVal);

public:
    static int reduceMatrix(std::vector<std::vector<int>> &matrix, int n);
    //usuwa krawedzie idace do dwoch odwiedzonych miast
    static void markEdge(std::vector<std::vector<int>> &matrix, int n, int city1, int city2);
    static std::vector<int> findShortestHCycle(std::vector<std::vector<int>> &adjMatrix, int nOfNodes, int startNode, int lightestHCycle);
};

#endif // BRANCHNBOUND_H_INCLUDED
