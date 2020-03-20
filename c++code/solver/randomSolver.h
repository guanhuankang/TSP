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

    void run(int T = 500){
        generation = 1;

        double totTime = 0.0;
        while(generation<T){
            clock_t start = clock();

            step();
            generation++;

            clock_t end = clock();
            double t = (double)(end - start)/(double)CLOCKS_PER_SEC;
            totTime += t;
            printf("Random generation %d cost:%lf fps:%lf n:%d left:%lf sec tot:%lf sec\r",\
             generation, G->cost(findBest()), 1.0/t, n, (T-generation)*t, totTime);
            fflush(stdout);
        }
    }
};

#endif