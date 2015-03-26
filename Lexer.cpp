#include "Lexer.h"

string keyword[13] = { "CREATE", "TABLE", "INT", "DEFAULT", "PRIMARY", "KEY", "INSERT",
                       "INTO", "VALUES", "DELETE", "FROM", "WHERE", "SELECT" };

//records col number of each row;
vector<int> colList;

Lexer::Lexer () {
    tokenList.clear();
    totalRow = 1;
    index = 0;
}
Lexer::~Lexer() {
    tokenList.clear();
}

bool isLetter(char a) {
    if ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z'))
        return true;
    return false;
}

bool isNum(char a) {
    if (a >= '0' && a <= '9')
        return true;
    return false;
}

void Lexer::print() {
    for (int i = 0; i < tokenList.size(); i++) {
        cout << "(" << tokenList[i].name << ", " << tokenList[i].token << ", "
            << "row " << tokenList[i].row << ", "
            << "col " << tokenList[i].col << ")" << endl;
    }
}

vector<Node> Lexer::work(string input) {
    int length = input.length();
    colList.push_back(0);

    while (index < length) {
        char ch;
        ch = input[index++];

        //ignore space and tab
        while (ch == ' ' || ch == '\t' || ch == '\r') {
            ch = input[index++];
            if (index == length)
                break;
        }
        if (ch == ' ')
            break;

        //identifier or keyword
        if (isLetter(ch) || ch == '_') {
            Node node;
            node.row = totalRow;
            node.col = index - colList[totalRow-1];
            string word = "";
            while (isLetter(ch) || isNum(ch) || ch == '_') {
                word += ch;
                ch = input[index++];
                if (index == length)
                    break;
            }
            if (isLetter(ch) || isNum(ch) || ch == '_') {
                word += ch;
            }
            else {
                index--;
            }
            if (word.length() > 64) {
              node.name = word;
              node.token = "ERROR";
              tokenList.push_back(node);
            } else {
              bool isKeyword = false;

              node.name = word;
              string upName = "";
              for (int i = 0; i < word.size(); i++) {
                  if (word[i] >= 'a' && word[i] <= 'z')
                      upName += word[i] + 'A' - 'a';
                  else
                      upName += word[i];
              }
              for (int i = 0; i < 13; i++) {
                  if (upName.compare(keyword[i]) == 0) {
                      isKeyword = true;
                      node.token = keyword[i];
                      tokenList.push_back(node);
                      break;
                  }
              }
              if (!isKeyword) {
                  node.token = "ID";
                  tokenList.push_back(node);
              }
            }

        }
        //num
        else if (isNum(ch)) {
            int thisCol = index - colList[totalRow-1];
            string word = "";
            while (isNum(ch)) {
                word += ch;
                ch = input[index++];
                if (index == length)
                    break;
            }
            if (isNum(ch)) {
                word += ch;
            }
            else {
                index--;
            }
            if (word.length() > 10) {
              Node node(word, "ERROR");
              tokenList.push_back(node);
            } else {
              Node node(word, "NUM");
              node.row = totalRow;
              node.col = thisCol;
              tokenList.push_back(node);
            }
        }
        //other character
        else {
            string word = "";
            Node node;
            node.row = totalRow;
            node.col = index - colList[totalRow-1];
            switch (ch) {
            case '=':
                word += ch;
                ch = input[index++];
                if (ch == '=') {
                    word += ch;
                    node.name = word;
                    node.token = "EQUAL";
                }
                else {
                    node.name = word;
                    node.token = "ASSIGN";
                    index--;
                }
                tokenList.push_back(node);
                break;
            case '>':
                word += ch;
                ch = input[index++];
                if (ch == '=') {
                    word += ch;
                    node.name = word;
                    node.token = "GTE";
                }
                else {
                    node.name = word;
                    node.token = "GT";
                    index--;
                }
                tokenList.push_back(node);
                break;
            case '<':
                word += ch;
                ch = input[index++];
                if (ch == '=') {
                    word += ch;
                    node.name = word;
                    node.token = "LTE";
                }
                else if (ch == '>') {
                    word += ch;
                    node.name = word;
                    node.token = "NOTEQUAL";
                }
                else {
                    node.name = word;
                    node.token = "LT";
                    index--;
                }
                tokenList.push_back(node);
                break;
            case '+':
                word += ch;
                node.name = word;
                node.token = "ADD";
                tokenList.push_back(node);
                break;
            case '-':
                word += ch;
                node.name = word;
                node.token = "SUB";
                tokenList.push_back(node);
                break;
            case '*':
                word += ch;
                node.name = word;
                node.token = "MUL";
                tokenList.push_back(node);
                break;
            case '/':
                word += ch;
                node.name = word;
                node.token = "DIV";
                tokenList.push_back(node);
                break;
            case '!':
                word += ch;
                node.name = word;
                node.token = "NOT";
                tokenList.push_back(node);
                break;
            case ',':
                word += ch;
                node.name = word;
                node.token = "COMMA";
                tokenList.push_back(node);
                break;
            case '&':
                word += ch;
                ch = input[index++];
                if (ch == '&') {
                    word += ch;
                    node.name = word;
                    node.token = "AND";
                }
                else {
                    node.name = word;
                    node.token = "ERROR";
                    index--;
                }
                tokenList.push_back(node);
                break;
            case '|':
                word += ch;
                ch = input[index++];
                if (ch == '|') {
                    word += ch;
                    node.name = word;
                    node.token = "OR";
                }
                else {
                    node.name = word;
                    node.token = "ERROR";
                    index--;
                }
                tokenList.push_back(node);
                break;
            case '(':
                word += ch;
                node.name = word;
                node.token = "LP";
                tokenList.push_back(node);
                break;
            case ')':
                word += ch;
                node.name = word;
                node.token = "RP";
                tokenList.push_back(node);
                break;
            case ';':
                word += ch;
                node.name = word;
                node.token = "SEM";
                tokenList.push_back(node);
                break;
            case '\n':
                colList.push_back(index);
                totalRow++;
                break;
            default:
                word += ch;
                node.name = word;
                node.token = "ERROR";
                tokenList.push_back(node);
                break;
            }

            if (index == length)
                break;
        }
    }
    return tokenList;
}
