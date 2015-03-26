#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <cstdio>
#include <vector>
#include <string>

#include "base.h"

using namespace std;

/*
  lexer class
*/
class Lexer {
public:
    Lexer();
    ~Lexer();
    /*
      input is the data as a string
      return a vector conterns Node
    */
    vector<Node> work(string input);
    void print();
private:
    vector<Node> tokenList;
    int totalRow;
    int index;
};

#endif
