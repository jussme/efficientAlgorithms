#ifndef GENETIC_H_INCLUDED
#define GENETIC_H_INCLUDED

#include <vector>
#include <map>
#include <mutex>
#include <random>
#include <utility>
#include <string>

class Genetic{
private:
    class Timer{
    private:
        unsigned int period;
        std::time_t startEpoch;
    public:
        Timer(int ms);
        bool elapsed();
    };


    int populationSize;
    float crossoverRate;
    float mutationRate;
    std::vector<std::pair<std::vector<short>, double>> population;

    std::mt19937 generator;

    std::string firstImprov;
    int lightestHCycle;
    double highestFitness;
    std::vector<short> bestCurrPath;

    int iteration;

    void initPopulation(short** adjMatrix, int populationSize, int n);
    std::vector<std::vector<short>> selection();
    std::pair<std::vector<short>, std::vector<short>> pmx(std::vector<short> &p1, std::vector<short> &p2);
    void inversion(std::vector<short> &path);
    static int indexOf(std::vector<short> &vec, short val);
    double reportNewFitness(std::vector<short> path, double fitness, int pathCost);

public:
    Genetic(int populationSize, float crossoverRate, float mutationRate);
    static std::vector<short> genPathBFS(short** adjMatrix, short n, short start);
    std::vector<short> genPathRand(short n);
    static int getPathCost(short** adjMatrix, std::vector<short>& path);
    static double getFitness(short** adjMatrix, std::vector<short>& path);
    std::vector<short> findShortestHCycle(short** adjMatrix, short n, int lightestHCycle);
};

#endif // GENETIC_H_INCLUDED
