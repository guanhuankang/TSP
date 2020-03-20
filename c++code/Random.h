#ifndef RANDOM
#define RANDOM

#include <cmath>
#include <random>
#include <ctime>

class Random{
public:
    std::default_random_engine e;

    Random(){
        e.seed(clock());
        rand();
    }

    int randint(int a,int b){
        std::uniform_int_distribution<int> u(a,b);
        return u(e);
    }

    double rand(){
        std::uniform_real_distribution<double> u(0.0, 1.0);
        return u(e);
    }
};

#endif