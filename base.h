#ifndef BASE_STRUCTURE
#define BASE_STRUCTURE

#include <string>
#include <map>
#include <vector>
#include <set>
#include <stack>

using namespace std;

struct Table {
    vector<map<string, int> > rows;
    map<string, bool> primaryKey;
    map<string, int> defaultValues;
    vector<string> fields;
};

struct Symbol {
    bool isTerminalSymbol;
    bool canBeEmpty;
    bool isStartSymbol;
    vector<vector<string> > expression;
    set<string> firstSet, followSet;
    Symbol() {
        canBeEmpty = false;
        isStartSymbol = true;
    };
};

/*
  a node records the name and token of the word,
  and its row and col;
  col is the column of its first character.
*/

struct Node {
    string token, name;
    int row, col;
    Node() {
        token = "";
        name = "";
        row = 0;
        col = 0;
    }
    Node(string _name, string _token) : name(_name), token(_token) {
        row = 0;
        col = 0;
    };
};

int convertStringToInteger(string);
string convertIntegerToString(int);
bool isCompareSymbol(Node);
bool isCalculateSymbol(Node);
bool isBooleanSymbol(Node);
void printTokens(vector<Node>);
void releaseStack(stack<int>&, stack<string>&, bool&);
void releasePositvieOrNegative(stack<int>&, stack<string>&);
int calculateArithmeticExpression(vector<Node>, bool&);
bool calculateCompareExpression(int, int, string);
void releaseBooleanStack(stack<bool>&, stack<string>&);
void getBooleanFromCompare(int&, int&, string&,
    vector<Node>&, stack<bool>&, bool&);
bool calculateBooleanExpression(vector<Node>, map<string, int>, bool&);

#endif