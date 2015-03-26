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
    cout << "Boolean test start" << endl;
    while (getline(cin, input)) {
        vector<string> lines;
        lines.push_back(input);
        Lexer* lexer = new Lexer();
        vector<Node> tokens;
        tokens = lexer->work(input);
        map<string, int> temp;
        bool divideZero = false, result;
        result = calculateBooleanExpression(tokens, temp, divideZero);
        if (divideZero) {
            cout << "Divided by 0" << endl;
        } else {
            cout << (result?"True":"False") << endl;
        }
        delete lexer;
    }
    cout << "Boolean test end" << endl;
    return 0;
}