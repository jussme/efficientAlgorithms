#ifndef TABUSEARCH_H_INCLUDED
#define TABUSEARCH_H_INCLUDED

#include <vector>
#include <string>
#include <mutex>
#include <queue>

class TabuSearch{
private:
    class Printer{
    private:
        std::mutex inserting;
        std::mutex inserting2;
    public:
        void printSync(std::string input);
    };
    class Timer{
    private:
        unsigned int period;
        std::time_t startEpoch;
    public:
        Timer(int ms);
        bool elapsed();
    };
    static std::vector<int> genPathBFS(int** adjMatrix, int n, int start);
    static std::vector<int> genPathRand(int n);
public:
    static int getPathCost(int** adjMatrix, std::vector<int>& path);
    std::vector<int> static findShortestHCycle(int** adjMatrix, int n, int memoryLength, int itWithNoImprovLimit, int lightestHCycle, int threadN, int start);
};


#endif // TABUSEARCH_H_INCLUDED
