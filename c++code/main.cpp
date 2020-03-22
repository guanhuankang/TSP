#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <ctime>
#include <fstream>
#include <sstream>
#include <pthread.h>

#include "graph.h"

#include "solver/randomSolver.h"
#include "solver/GASolver.h"
#include "solver/PSOSolver1.h"
#include "solver/PSOSolver2.h"

using namespace std;

struct pThreadData{
    string datasetID;
    int generations;
    pThreadData(string _did, int _g = 2000){
        datasetID = _did;
        generations = _g;
    }
    pThreadData(){}
};


void write2file(const char* filename, vector<double> &vec, Graph* G, List<int> sol){
    ofstream outfile;
    stringstream ss;
    string name;
    ss << "result/" << time(0) << "_" << filename;
    ss >> name;
    outfile.open(name);
    
    outfile << G->name << "\n";
    for(int i=1;i<=G->cityNum;i++){
        outfile << G->x[i] << " \n"[i==G->cityNum];
    }
    for(int i=1;i<=G->cityNum;i++){
        outfile << G->y[i] << " \n"[i==G->cityNum];
    }
    for(int i=0;i<vec.size();i++){
        outfile << vec[i] << " \n"[i==vec.size()-1];
    }
    for(int i=1;i<=sol.length;i++){
        outfile << sol[i] << " \n"[i==sol.length];
    }
    if(G->existOpt){
        for(int i=1;i<=G->optSeq.length;i++){
            outfile << G->optSeq[i] << " \n"[i==G->optSeq.length];
        }
    }
    outfile.flush();
    outfile.close();
}

// void* runcase(string datasetID, int generations = 2000){
void* runcase(void* d){
    pThreadData* u = (pThreadData*) d;
    string datasetID = u->datasetID;
    int generations = u->generations;

    cout<<"Working on "<< datasetID << " " << generations <<endl;

    stringstream ss;
    ss << "../dataset/" << datasetID << ".tsp";
    ss << " ../dataset/" << datasetID << ".opt.tour";

    string tsp, tour, filename;
    ss >> tsp >> tour;
    // Graph G("../dataset/att48.tsp", "../dataset/att48.opt.tour");
    Graph G(tsp.c_str(), tour.c_str());

    if(G.existOpt){
        cout<< datasetID << "Opt:"<<G.cost(G.optSeq)<<endl;
    }else{
        cout<< datasetID << "No opt found"<<endl;
    }

    RandomSolver randsolver(G.cityNum*2, G.cityNum, &G);
    vector<double> val1 = randsolver.run(generations);
    ss.clear();
    ss << "Random-" << datasetID << "-" << generations << ".txt";
    ss >> filename;
    write2file(filename.c_str(), val1, &G, randsolver.gbest);
    cout<<endl;fflush(stdout);

    PSOSolver1 pso1(G.cityNum*2, G.cityNum, &G, 1.0/G.cityNum, 1.0/G.cityNum);
    vector<double> val2 = pso1.run(generations);
    ss.clear();
    ss << "pso1-" << datasetID << "-" << generations << ".txt";
    ss >> filename;
    write2file(filename.c_str(), val2, &G, pso1.gbest);
    cout<<endl;fflush(stdout);

    PSOSolver2 pso2(G.cityNum*2, G.cityNum, &G, 0.4, 0.6, 0.1);
    vector<double> val3 = pso2.run(generations);
    ss.clear();
    ss << "pso2-" << datasetID << "-" << generations << ".txt";
    ss >> filename;
    write2file(filename.c_str(), val3, &G, pso2.gbest);
    cout<<endl;fflush(stdout);

    GASolver gasolver(G.cityNum*2, G.cityNum, &G, 1.0/G.cityNum, 0.5);
    vector<double> val4 = gasolver.run(generations);
    ss.clear();
    ss << "GA-" << datasetID << "-" << generations << ".txt";
    ss >> filename;
    write2file(filename.c_str(), val4, &G, gasolver.gbest);
    cout<<endl;fflush(stdout);

    ofstream outfile;
    ss.clear();
    ss << "analysis/" << datasetID << ".val";
    ss >> filename;
    outfile.open(filename);
    for(int i=0;i<val1.size();i++){
        outfile << val1[i] << " \n"[i==val1.size()-1];
    }
    for(int i=0;i<val2.size();i++){
        outfile << val2[i] << " \n"[i==val1.size()-1];
    }
    for(int i=0;i<val3.size();i++){
        outfile << val3[i] << " \n"[i==val1.size()-1];
    }
    for(int i=0;i<val4.size();i++){
        outfile << val4[i] << " \n"[i==val1.size()-1];
    }
    outfile.flush();
    outfile.close();
    return 0;
}

// int main(){

//     pThreadData param[9];
//     param[0] = pThreadData("att48");
//     param[1] = pThreadData("gr96");
//     param[2] = pThreadData("eil101");
//     param[3] = pThreadData("ch130");
//     param[4] = pThreadData("gr202");
//     param[5] = pThreadData("a280");

//     param[6] = pThreadData("d657");
//     param[7] = pThreadData("dsj1000");
//     param[8] = pThreadData("fl1400");
    
//     runcase(&(param[1]));

//     return 0;
// }

 
int main()
{
    // 定义线程的 id 变量，多个变量使用数组
    int NUM = 9;
    pthread_t tids[NUM];
    pThreadData param[NUM];
    param[0] = pThreadData("att48");
    param[1] = pThreadData("gr96");
    param[2] = pThreadData("eil101");
    param[3] = pThreadData("ch130");
    param[4] = pThreadData("gr202");
    param[5] = pThreadData("a280");

    param[6] = pThreadData("d657");
    param[7] = pThreadData("dsj1000");
    param[8] = pThreadData("fl1400");

    for(int i = 0; i < NUM; ++i)
    {
        //参数依次是：创建的线程id，线程参数，调用的函数，传入的函数参数
        int ret = pthread_create(&tids[i], NULL, runcase, (void*)(&(param[i])));
        if (ret != 0)
        {
           cout << "pthread_create error: error_code=" << ret << endl;
        }
    }
    cout<<"waiting"<<endl;fflush(stdout);

    //等各个线程退出后，进程才结束，否则进程强制结束了，线程可能还没反应过来；
    pthread_exit(NULL);
    return 0;
}
