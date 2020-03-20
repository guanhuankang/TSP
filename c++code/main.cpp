#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <ctime>

#include "graph.h"

#include "solver/randomSolver.h"
#include "solver/GASolver.h"
#include "solver/PSOSolver1.h"
#include "solver/PSOSolver2.h"

using namespace std;

int main(){
    // Graph G("../dataset/att48.tsp", "../dataset/att48.opt.tour");
    Graph G("../dataset/a280.tsp", "../dataset/a280.opt.tour");
    // Graph G("../dataset/dsj1000.tsp", "../dataset/dsj1000.opt.tour");

    // PSOSolver2 pso2(G.cityNum*2, G.cityNum, &G, 0.4, 0.6, 0.1);
    // pso2.run(30000);
    // cout<<endl;fflush(stdout);

    // RandomSolver randsolver(G.cityNum*2, G.cityNum, &G);
    // randsolver.run(1000);
    // cout<<endl;fflush(stdout);

    // PSOSolver1 pso1(G.cityNum*2, G.cityNum, &G, 1.0/G.cityNum, 1.0/G.cityNum);
    // pso1.run(1000);
    // cout<<endl;fflush(stdout);

    GASolver gasolver(G.cityNum*2, G.cityNum, &G, 1.0/G.cityNum, 0.5);
    gasolver.run(2000);
    cout<<endl;fflush(stdout);

    /*TODO
        3. A-PSO Algorithm implementation
    */

    return 0;
}

