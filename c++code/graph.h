#ifndef GRAPH
#define GRAPH

#include <fstream>
#include <cstring>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cstring>

#include "list.h"
using namespace std;

class Graph{
public:
    string name;
    double optVal;
    int cityNum; // default 0
    bool existOpt;  // default false

    vector< vector<double> > edges; // base 1,1
    vector< double > x, y; // base 1
    List<int> optSeq;

    Graph(const char* tspfile, const char* tourfile){
        //tsp
        ifstream tsp;
        tsp.open(tspfile);

        string line;
        bool isNum = false;
        cityNum = 0;
        stringstream ss;
        while(getline(tsp, line)){
            if( line.length()>=7 && equ(line, 0, 6, "COMMENT") ){
                name = line;
            }else if( line.length()>=9 && equ(line, 0, 8, "DIMENSION") ){
                string tmp;
                ss.clear();
                ss << line;
                ss >> line >> tmp >> cityNum;
                if(isdigit(tmp[0])){
                    ss.clear();
                    ss << tmp;
                    ss >> cityNum;
                }
                x = vector<double>(cityNum+1);
                y = vector<double>(cityNum+1);
            }else if( line.length()>=3 && equ(line, 0, 2, "EOF") ){
                break;
            }else if( line.length()>=18 && equ(line, 0, 17, "NODE_COORD_SECTION") ){
                isNum = true;
            }else if(isNum){
                int id;
                double _x, _y;
                ss.clear();
                ss << line;
                ss >> id >> _x >> _y;
                x[ id ] = _x;
                y[ id ] = _y;
            }
        }

        edges = vector< vector<double> >( cityNum+1, vector<double>(cityNum+1) );
        for(int i=1;i<=cityNum;i++){
            for(int j=i;j<=cityNum;j++){
                double d = dist(i,j);
                edges[i][j] = d;
                edges[j][i] = d;
            }
        }

        //tour
        ifstream tour;
        tour.open(tourfile);
        int id;
        existOpt = false;
        isNum = false;
        int cnt = 1;
        while(getline(tour, line)){
            if( equ(line, 0, 11, "TOUR_SECTION") ){
                isNum = true;
                existOpt = true;
                optSeq = List<int>(cityNum);
            }else if( equ(line, 0, 2, "EOF") ){
                break;
            }else if( isNum ){
                stringstream ss;
                ss << line;
                ss >> id;
                if(id<0)break;
                optSeq.set(cnt++, id);
            }
        }

        tsp.close();
        tour.close();
    }

    bool equ(string line, int s, int t, const char* T){
        int cnt = 0;
        for(int i=s;i<=t;i++){
            if( i>=line.length() || line[i]!=T[cnt++])return false;
        }
        return true;
    }

    inline double dist(int i,int j){
        return sqrt( (x[i]-x[j])*(x[i]-x[j]) + (y[i]-y[j])*(y[i]-y[j]) );
    }

    double cost(List<int> solution){
        if(solution.length!=this->cityNum){
            return -1;
        }
        double sum = 0;
        for(int i=2;i<=solution.length;i++){
            sum += edges[ solution[i] ][ solution[i-1] ];
        }
        sum += edges[ solution[1] ][ solution[solution.length] ];
        return sum;
    }

    bool isValid(List<int> solution){
        if(solution.length!=this->cityNum)return false;
        bool vis[solution.length+1];
        memset(vis, 0, sizeof(vis));
        for(int i=1;i<=solution.length;i++){
            if(vis[ solution[i] ])return false;
            vis[solution[i]] = true;
        }
        return true;
    }
};

#endif