#include <iostream>
#include <string>
#include <map>

#include "base.h"
#include "Lexer.h"
#include "Parser.h"
#include "Database.h"

using namespace std;

int main() {
    string input;
    cout << "Grammar test start" << endl;
    while (getline(cin, input)) {
        vector<string> lines;
        lines.push_back(input);
        Lexer* lexer = new Lexer();
        vector<Node> tokens;
        tokens = lexer->work(input);
        lexer->print();
        Parser* parser = new Parser();
        parser->work(tokens, lines);
        delete parser;
        delete lexer;
    }
    cout << "Grammar test end" << endl;
    return 0;
}