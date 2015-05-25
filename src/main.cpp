#include <fstream>
#include <string>

#include "base.h"
#include "Lexer.h"
#include "Parser.h"

int main(int argc, char **argv) {
    if (argc > 1) {
        string input;
        vector<string> lines;
        ifstream fin(argv[1]);
        string s;
        while (getline(fin, s)) {
            input += s + '\n';
            lines.push_back(s);
        }
        cout << input << endl;
        fin.close();
        Lexer* lexer = new Lexer();
        Parser* parser = new Parser();
        vector<Node> tokens = lexer->work(input);
        parser->work(tokens, lines);
        delete lexer;
        delete parser;
    } else {
        string input;
        cout << ">> ";
        while (getline(cin, input)) {
            vector<string> lines;
            lines.push_back(input);
            Lexer* lexer = new Lexer();
            vector<Node> tokens = lexer->work(input);
            Parser* parser = new Parser();
            parser->work(tokens, lines);
            delete parser;
            delete lexer;
            cout << ">> ";
        }
    }
    return 0;
}