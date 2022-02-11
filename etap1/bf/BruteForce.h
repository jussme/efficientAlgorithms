#ifndef BRUTEFORCE_H_INCLUDED
#define BRUTEFORCE_H_INCLUDED
#include <string>
#include <vector>
class Path
{
public:
    //~Path();
    Path(const Path &path);
    //Path();
    Path(int maxLength);

    int currLength = 0;
    std::vector<int> nodes;
    std::vector<int> weights;
    int weight = 0;

    void addNode(int node, int weight);
    void addNode(int node, int** adjMatrix);
    void removeTopNode();

    std::string toString();
};

class BruteForce
{
private:
    static void findShortestHCycleP(int** adjMatrix, Path* currPath, bool traversedMatrix[], Path** currentLightestPath);
public:
    static Path* findShortestHCycle(int** adjMatrix, int nOfNodes, int startNode, int lightestHCycle);
};
#endif // BRUTEFORCE_H_INCLUDED
