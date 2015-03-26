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
    vector<string> lines;
    cout << "Arithmetic test start" << endl;
    while (getline(cin, input)) {
        vector<string> lines;
        lines.push_back(input);
        Lexer* lexer = new Lexer();
        vector<Node> tokens;
        tokens = lexer->work(input);
        bool flag = false;
        int num = calculateArithmeticExpression(tokens, flag);
        cout << (flag?"Divided by 0":convertIntegerToString(num)) << endl;
        delete lexer;
    }
    cout << "Arithmetic test end" << endl;
    return 0;
}