#ifndef RANDOMSOLVER
#define RANDOMSOLVER

#include <algorithm>
#include <iostream>
#include <ctime>

#include "../list.h"
#include "../Random.h"
#include "../graph.h"

using namespace std;

class RandomSolver{
public:
    vector< List<int> > population;//base 0
    List<int> gbest;
    Graph* G;
    int n, generation;

    RandomSolver(int n, int len, Graph *G){
        population.clear();
        this->n = n;
        this->G = G;

        for(int i=1;i<=n;i++){
            population.push_back( List<int>(len) );
        }
        for(int i=0;i<n;i++){
            population[i].sequeue();
            population[i].shuffle();
        }
    }

    void step(){
        for(int i=0;i<n;i++){
            population[i].shuffle();
        }
    }

    List<int> findBest(){
        double cost = G->cost(population[0]);
        List<int> best = population[0];

        for(int i=1;i<n;i++){
            double tmp = G->cost(population[i]);
            if(tmp<cost){
                cost = tmp;
                best = population[i];
            }
        }
        return best;
    }

    vector<double> run(int T = 500){
        generation = 1;
        vector<double> vec;
        vec.clear();

        gbest = findBest();
        double gbv = G->cost(gbest);
        vec.push_back(gbv);

        double totTime = 0.0;
        while(generation<T){
            clock_t start = clock();

            step();
            generation++;

            clock_t end = clock();
            double t = (double)(end - start)/(double)CLOCKS_PER_SEC;
            totTime += t;
            double tmp = G->cost(findBest());
            if(tmp < gbv){
                gbv = tmp;
                gbest = findBest();
            }
            vec.push_back(gbv);
            printf("Random generation %d cost:%lf fps:%lf n:%d left:%lf sec tot:%lf sec\r",\
             generation, gbv, 1.0/t, n, (T-generation)*t, totTime);
            fflush(stdout);
        }
        return vec;
    }
};

#endif