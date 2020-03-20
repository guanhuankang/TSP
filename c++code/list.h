#ifndef LIST
#define LIST

#include <algorithm>
#include <vector>
#include <map>
#include <iostream>

#include "Random.h"

using std::vector;
using std::cout;
using std::cin;
using std::min;
using std::max;
using std::map;

template<class T>
class List{
public:
    vector<T> *list;//base 1
    int length;// default 0

    List();
    List(const List<T> &A);
    List(int len);
    ~List();
    void operator = ( const List<T> &A );

    void sequeue();
    void shuffle();
    void print() const;
    void set(int i, T a);

    List<T> dot (int k) const;
    List<T> cat (const List<T> &A) const;
    void append (const T a);
    void swap(int i,int j);

    List<T> operator * (int k) const;
    T operator [] (int k) const;//read only
    List<T> operator - (const List<T> &A) const;
    List<T> operator + (const List<T> &A) const;
    List<T> operator / (const List<T> &A) const;
    bool operator == (const List<T> &A);
};

template<class T>
List<T>::List(){
    this->length = 0;
    this->list = nullptr;
}

template<class T>
List<T>::List(int len){
    this->length = len;
    this->list = new vector<T>(len+1);
}

template<class T>
List<T>::List(const List<T> &A){
    this->length = 0;
    this->list = nullptr;
    *this = A;
}

template<class T>
List<T>::~List(){
    delete this->list;
}

template<class T>
void List<T>::operator = (const List<T> &A){
    if(&A==this)return;
    if(this->list)delete this->list;
    
    this->length = A.length;
    this->list = new vector<T>(0);
    this->list->clear();
    for(int i=0;i<A.list->size();i++){
        this->list->push_back( (*(A.list))[i] );
    }
}

template<class T>
void List<T>::sequeue(){
    for(int i=1;i<=this->length;i++){
        (*(this->list))[i] = i;
    }
}

template<class T>
void List<T>::shuffle(){
    Random random;
    for(int i=1;i<=this->length;i++){
        int j = random.randint(i,this->length);
        std::swap( (*(this->list))[i], (*(this->list))[j] );
    }
}

template<class T>
void List<T>::set(int i, T a){
    (*(this->list))[i] = a;
}

template<class T>
void List<T>::print() const{
    for(int i=1;i<=this->length;i++){
        cout<< (*this)[i] <<" \n"[i==this->length];
    }
}

template<class T>
List<T> List<T>::dot (int k) const{
    k = max(min(k, this->length), 0);
    List<T> ret(k);
    for(int i=1;i<=k;i++){
        (*(ret.list))[i] = (*this)[i];
    }
    return ret;
}

template<class T>
List<T> List<T>::cat (const List<T> &A) const{
    List<T> ret(0);
    for(int i=1;i<=this->length;i++){
        ret.list->push_back( (*this)[i] );
        ret.length++;
    }
    for(int i=1;i<=A.length;i++){
        ret.length++;
        ret.list->push_back( A[i] );
    }
    return ret;
}

template<class T>
void List<T>::append (const T a){
    this->length++;
    this->list->push_back( a );
}


template<class T>
void List<T>::swap (int i,int j){
    std::swap( (*(this->list))[i], (*(this->list))[j] );
}


template<class T>
List<T> List<T>::operator * (int k) const{
    Random random;
    k = max(min(k, this->length), 0);
    int s = random.randint(0, this->length - k);
    List<T> ret(k);
    for(int i=1;i<=k;i++){
        (*(ret.list))[i] = (*this)[i+s];
    }
    return ret;
}

template<class T>
T List<T>::operator [] (int k) const{
    return (*(this->list))[k];
}

template<class T>
List<T> List<T>::operator - (const List<T> &A) const{
    int k = 0;

    map<T,bool> e;
    e.clear();
    for(int i=1;i<=A.length;i++){
        e[A[i]] = true;
    }
    
    vector<T> vec;
    vec.clear();
    for(int i=1;i<=this->length;i++){
        if(!e.count( (*this)[i] )){
            vec.push_back( (*this)[i] );
        }
    }

    List<T> ret(vec.size());
    for(int i=1;i<=vec.size();i++){
        (*(ret.list))[i] = vec[i-1];
    }

    return ret;
}


template<class T>
List<T> List<T>::operator + (const List<T> &A) const{
    return ((*this)-A).cat(A);
}

template<class T>
List<T> List<T>::operator / (const List<T> &A) const{
    List<T> AB = (*this) - A;
    List<T> BA = A - (*this);
    return ((*this)-AB).cat(AB).cat(BA);
}

template<class T>
bool List<T>::operator == (const List<T> &A){
    if(A.length!=this->length)return false;
    for(int i=1;i<=A.length;i++){
        if(A[i] != (*this)[i] )return false;
    }
    return true;
}

#endif