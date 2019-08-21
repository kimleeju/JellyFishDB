#ifndef NODE_H
#define NODE_H

#include<string>
#include<vector>
#include<queue>
#include "SkipList.h"

using namespace std;

struct Node {

    string key;
    string value; 

    int level; 
    long timestamp; 

    vector<struct Node*> next; 

    Node (string k, string v, long t, int l) :
        key(k),
        value(v),
        timestamp(t),  
        level(l), 
        next(level) {}
    ~Node(){}
};

struct VNode {
    string value; 
    string timestamp; 
    struct VNode* next; 
};

#endif
