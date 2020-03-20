#ifndef PSOSOLVER1
#define PSOSOLVER1

#include <algorithm>
#include <iostream>
#include <ctime>

#include "../list.h"
#include "../Random.h"
#include "../graph.h"

using namespace std;

class PSOSolver1{
public:
    vector< List<int> > population;//base 0
    vector< List<int> > pbestIndexOf;//base 0,1
    vector< List<int> > pbest; // base 0
    Graph* G;
    int n, generation;
    List<int> gbest, gbestIndexOf;
    double ppossibility, gpossibility;

    PSOSolver1(int n, int len, Graph *G, double ppossibility, double gpossibility){
        population.clear();
        this->n = n;
        this->G = G;
        this->ppossibility = ppossibility;
        this->gpossibility = gpossibility;

        for(int i=1;i<=n;i++){
            population.push_back( List<int>(len) );
        }
        for(int i=0;i<n;i++){
            population[i].sequeue();
            population[i].shuffle();
        }

        pbestIndexOf = vector< List<int> >(n);
        pbest = vector< List<int> >(n);
        for(int i=0;i<n;i++){
            pbest[i] = population[i];
            pbestIndexOf[i] = List<int>(len);
            for(int j=1;j<=len;j++){
                pbestIndexOf[i].set( population[i][j], j );
            }
        }

        gbest = findBest();
        gbestIndexOf = List<int>(len);
        for(int j=1;j<=len;j++){
            gbestIndexOf.set( gbest[j], j );
        }
    }

    int indexof(List<int> &A, int val){
        for(int i=1;i<=A.length;i++){
            if(A[i]==val)return i;
        }
        return -1;
    }

    void step(){
        int m = population[0].length;
        Random random;
        double gbc = G->cost(gbest);

        for(int i=0;i<n;i++){
            List<int> p1 = population[i];

            for(int j=1;j<=m;j++){
                if( random.rand() <= ppossibility ){
                    population[i].swap( j, pbestIndexOf[i][ population[i][j] ] );
                    // population[i].swap( j, indexof(pbest[i], population[i][j]) );
                }
            }
            for(int j=1;j<=m;j++){
                if( random.rand() <= gpossibility ){
                    population[i].swap( j, gbestIndexOf[ population[i][j] ] );
                    // population[i].swap( j, indexof(gbest, population[i][j]) );
                }
            }

            double p = G->cost(population[i]);
            double pb = G->cost(pbest[i]);
            if( p < pb ){
                pbest[i] = population[i];
                for(int j=1;j<=m;j++){
                    pbestIndexOf[i].set( population[i][j], j );
                }
                if( p < gbc ){
                    gbc = p;
                    gbest = pbest[i];
                    gbestIndexOf = pbestIndexOf[i];
                }
            }
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

        while(generation<T){
            clock_t start = clock();

            step();
            generation++;

            clock_t end = clock();
            double t = (double)(end - start)/(double)CLOCKS_PER_SEC;
            printf("PSO1 generation %d cost:%lf fps:%lf n:%d left:%lf sec %d\r",\
             generation, G->cost(gbest), 1.0/t, n, (T-generation)*t, G->isValid(gbest) );
            fflush(stdout);
        }
    }
};

#endif