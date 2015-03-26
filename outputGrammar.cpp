#include <iostream>
#include <string>
#include <map>

#include "base.h"
#include "Lexer.h"
#include "Parser.h"
#include "Database.h"

using namespace std;

int main() {
    Parser* parser = new Parser();
    parser->printGrammar();
    parser->printFirstSet();
    parser->printFollowSet();
    parser->printPredictSet();
    return 0;
}