#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <stack>

#include "base.h"

using namespace std;

class Parser {
public:
    void printGrammar();
    void printFirstSet();
    void printFollowSet();
    void printPredictSet();
    void work(vector<Node>, vector<string>);
    Parser();

private:
    string static terminalSymbol[];
    string static nonTerminalSymbol[][20];
    map<string, Symbol> grammar;
    map<string, map<string, vector<string> > > predictSet;
    vector<string> symbols;
    void initializeNonTerminalSymbol();
    void initializeTerminalSymbol();
    set<string> dfsFirstSet(string symbol);
    void initializeFirstSet();
    bool checkChange(set<string>& follow, string target);
    bool checkChangeWhenInitializeFollowSet(string symbol);
    void printSet(set<string> tempSet, string symbol);
    void initializeFollowSet();
    void initializePredictSet();
    void initialize();
    void alertError(string, int);
    void grammarError(Node, vector<string>);
    void errorRecovery(stack<string>&, vector<Node>&,
        vector<Node>, int&);
    bool doCreateStatement(vector<Node>, string&);
    bool doDeleteStatement(vector<Node>, string&);
    bool doSelectStatement(vector<Node>, string&);
    bool doInsertStatement(vector<Node>, string&);
};

#endif