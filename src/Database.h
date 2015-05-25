#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "base.h"

using namespace std;

class Database {
public:
    static Database* getInstance();
    bool create(string, vector<string>, vector<int>,
        vector<string>, string&);
    bool insert(string, vector<string>, vector<int>, string&);
    bool deleteData(string, vector<Node>, string&);
    bool query(string, vector<string>, vector<Node>, string&);
private:
    Database();
    ~Database();
    void readFromFile();
    void writeToFile();
    vector<int> findVector(string, vector<map<string, int> >);
    void printTable(vector<string>, vector<map<string, int> >);
    map<string, Table> tables;
};

#endif