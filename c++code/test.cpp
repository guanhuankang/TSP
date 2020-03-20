#include <iostream>
#include <vector>
using namespace std;

class Node{
public:
    int a;
    char* s;
    vector< vector<int> > vec;
    Node(){
        a = 99;
        s = new char(10);
        vec.clear();
        for(int i=0;i<100;i++){
            vec.push_back( vector<int>(100) );
        }
    }
    ~Node(){
        delete s;
    }
};

int main(){
    int T = 100;
    while(T--){
        Node u;
        cout<<u.vec[1][0]<<endl;
    }
    return 0;
}