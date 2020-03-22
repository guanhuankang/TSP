#ifndef GASOLVER
#define GASOLVER

#include <algorithm>
#include <cstdio>
#include <ctime>
#include <queue>

#include "../list.h"
#include "../Random.h"
#include "../graph.h"

using namespace std;

class GASolver{
public:
    vector< List<int> > population;//base 0
    List<int> gbest;
    Graph* G;
    double ppossibility, gpossibility;
    int n, generation;

    GASolver(int n, int len, Graph *G, double ppossibility, double gpossibility){
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
    }

    void step(){
        int m = population[0].length;

        //shuffle
        Random random;
        for(int i=0;i<n;i++){
            int j = random.randint(i, n-1);
            swap(population[i], population[j]);
        }
        for(int i=0;i<n;i+=2){
            population.push_back(population[i]);
            population.push_back(population[i+1]);

            //crossover
            int k = random.randint(1, m-1);
            if(random.rand() <= this->gpossibility){
                List<int> A = population[i]*k;
                List<int> B = population[i+1]*k;
                population[i+1] = A.cat( population[i+1] - A );
                population[i] = B.cat( population[i]-B );
            }else{
                List<int> A = population[i].dot(k);
                List<int> B = population[i+1].dot(k);
                population[i] = B.cat(population[i] - B);
                population[i+1] = A.cat(population[i+1] - A);
            }

            //mutation
            for(int j=1;j<=m;j++){
                if(random.rand() <= this->ppossibility)
                    population[i].swap( j, random.randint(1,m) );
                if(random.rand() <= this->ppossibility)
                    population[i+1].swap( j, random.randint(1,m) );
            }
        }
        //update n
        n = population.size();
        
        //choose 2
        double val[n];
        for(int i=0;i<n;i++){
            val[i] = G->cost(population[i]);
            population[i].length = i;
        }
        sort(population.begin(), population.end(), [&](List<int> A, List<int> B){
            // return (G->cost(A)) < (G->cost(B));
            return val[A.length] < val[B.length];
        } );
        for(int i=0;i<n;i++){
            population[i].length = m;
        }

        int nn = n - n/2;
        for(int i=0;i<nn;i++){
            population.pop_back();
        }
        n = population.size();
    }

    List<int> findBest(){
        double cost = G->cost(population[0]);
        List<int> best(0);
        best = population[0];

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
            printf("GA generation %d cost:%lf fps:%lf n:%d left:%lf sec tot: %lf sec\r",\
             generation, gbv, 1.0/t, n, (T-generation)*t, totTime);
            fflush(stdout);
        }
        return vec;
    }
};

#endif