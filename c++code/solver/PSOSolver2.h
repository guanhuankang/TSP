#ifndef PSOSOLVER2
#define PSOSOLVER2

#include <algorithm>
#include <iostream>
#include <ctime>

#include "../list.h"
#include "../Random.h"
#include "../graph.h"

using namespace std;

class PSOSolver2{
public:
    vector< List<int> > population;//base 0
    vector< List<int> > pbest; // base 0
    Graph* G;
    int n, generation;
    List<int> gbest;
    double c1, c2, beta;

    PSOSolver2(int n, int len, Graph *G, double c1, double c2, double beta){
        population.clear();
        this->n = n;
        this->G = G;
        this->c1 = c1;
        this->c2 = c2;
        this->beta = beta;

        for(int i=1;i<=n;i++){
            population.push_back( List<int>(len) );
        }

        pbest = vector< List<int> >(n);
        for(int i=0;i<n;i++){
            population[i].sequeue();
            population[i].shuffle();
            pbest[i] = population[i];
        }

        gbest = findBest();
    }

    double step(){
        Random random;
        double gbv = G->cost(gbest);
        
        for(int i=0;i<n;i++){
            double alpha_1 = random.rand();
            double alpha_2 = random.rand();

            int plen = this->c1 * alpha_1 * n;
            int glen = this->c2 * alpha_2 * n;
            List<int> Q = (pbest[i]*plen)/(gbest*glen);
            List<int> inc = Q.dot( int((1.0-this->beta)*Q.length) );
            population[i] = population[i] + inc;

            double tmp = G->cost(population[i]);
            double pbv = G->cost(pbest[i]);
            if( tmp < pbv ){
                pbest[i] = population[i];
                if( tmp < gbv ){
                    gbv = tmp;
                    gbest = population[i];
                }
            }
        }

        return gbv;
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

        while(generation<T){
            clock_t start = clock();

            double gbv = step();
            generation++;

            clock_t end = clock();
            double t = (double)(end - start)/(double)CLOCKS_PER_SEC;
            printf("PSO2 generation %d cost:%lf fps:%lf n:%d left:%lf sec %d\r",\
             generation, gbv, 1.0/t, n, (T-generation)*t, G->isValid(gbest));
            fflush(stdout);
        }
    }
};

#endif