#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <stack>
#include "Parser.h"
#include "Database.h"

using namespace std;

string Parser::terminalSymbol[] = {
    "CREATE",
    "INSERT",
    "TABLE",
    "DELETE",
    "SELECT",
    "ID",
    "INT",
    "PRIMARY",
    "KEY",
    "ASSIGN",
    "DEFAULT",
    "NUM",
    "EPSILON",
    "INTO",
    "VALUES",
    "WHERE",
    "AND",
    "NOTEQUAL",
    "EQUAL",
    "LT",
    "GT",
    "LTE",
    "GTE",
    "FROM",
    "LP",
    "RP",
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "OR",
    "NOT",
    "SEM",
    "COMMA",
};

string Parser::nonTerminalSymbol[][20] = {
    {"ssql_stmt", "create_stmt", "|", "insert_stmt", "|", "delete_stmt", "|", "query_stmt"},
    {"create_stmt", "CREATE", "TABLE", "ID", "LP", "decl_list", "RP", "SEM"},
    {"insert_stmt", "INSERT", "INTO", "ID", "LP", "column_list", "RP", "VALUES", "LP", "value_list", "RP", "SEM"},
    {"delete_stmt", "DELETE", "FROM", "ID", "where_clause", "SEM"},
    {"query_stmt", "SELECT", "select_list", "FROM", "ID", "where_clause", "SEM"},
    {"decl_list", "decl", "decl_list_extra"},
    {"decl_list_extra", "COMMA", "decl", "decl_list_extra", "|", "EPSILON"},
    {"decl", "ID", "INT", "default_spec", "|", "PRIMARY", "KEY", "LP", "column_list", "RP"},
    {"default_spec", "DEFAULT", "ASSIGN", "simple_expr", "|", "EPSILON"},
    {"column_list", "ID", "column_list_extra"},
    {"column_list_extra", "COMMA", "ID", "column_list_extra", "|", "EPSILON"},
    {"value_list", "simple_expr", "value_list_extra"},
    {"value_list_extra", "COMMA", "simple_expr", "value_list_extra", "|", "EPSILON"},
    {"where_clause", "WHERE", "disjunct", "|", "EPSILON"},
    // {"conjunct_list", "boolean", "conjunct_list_extra"},
    // {"conjunct_list_extra", "AND", "boolean", "conjunct_list_extra", "|", "EPSILON"},
    {"boolean", "LP", "disjunct", "RP", "|", "NOT", "boolean", "|", "comp"},
    // {"operand", "NUM", "|", "ID"},
    {"rop", "NOTEQUAL", "|", "EQUAL", "|", "GT", "|", "LT", "|", "GTE", "|", "LTE"},
    {"select_list", "column_list", "|", "MUL"},
    {"simple_expr", "simple_term", "simple_expr_extra"},
    {"simple_expr_extra", "ADD", "simple_term", "simple_expr_extra", "|", "SUB", "simple_term",
        "simple_expr_extra", "|", "EPSILON"},
    {"simple_term", "simple_unary", "simple_term_extra"},
    {"simple_term_extra", "MUL", "simple_unary", "simple_term_extra", "|", "DIV", "simple_unary",
        "simple_term_extra", "|", "EPSILON"},
    {"simple_unary", "LP", "simple_expr", "RP", "|", "SUB", "simple_unary", "|", "ADD",
        "simple_unary", "|", "NUM"},
    {"disjunct", "conjunct", "disjunct_extra"},
    {"disjunct_extra", "OR", "conjunct", "disjunct_extra", "|", "EPSILON"},
    {"conjunct", "boolean", "conjunct_extra"},
    {"conjunct_extra", "AND", "boolean", "conjunct_extra", "|", "EPSILON"},
    {"comp", "expr", "rop", "expr"},
    {"expr", "term", "expr_extra"},
    {"expr_extra", "ADD", "term", "expr_extra", "|", "SUB", "term", "expr_extra", "|", "EPSILON"},
    {"term", "unary", "term_extra"},
    {"term_extra", "MUL", "unary", "term_extra", "|", "DIV", "unary", "term_extra", "|", "EPSILON"},
    {"unary", "SUB", "unary", "|", "ADD", "unary", "|", "ID", "|", "NUM"},
};

Parser::Parser() {
    initialize();
}

void Parser::initializeNonTerminalSymbol() {
    for (int i = 0; i < 32; i++) {
        int index = 0;
        string symbol = nonTerminalSymbol[i][0];
        vector<string> temp;
        grammar[symbol].isTerminalSymbol = false;
        while (nonTerminalSymbol[i][++index] != "") {
            if (nonTerminalSymbol[i][index] != "|") {
                grammar[nonTerminalSymbol[i][index]].isStartSymbol = false;
                temp.push_back(nonTerminalSymbol[i][index]);
            } else {
                grammar[symbol].expression.push_back(temp);
                temp.clear();
            }
        }
        if (!temp.empty()) {
            grammar[symbol].expression.push_back(temp);
        }
        symbols.push_back(symbol);
    }
}

void Parser::initializeTerminalSymbol() {
    for (int i = 0; i < 34; i++) {
        grammar[terminalSymbol[i]].isTerminalSymbol = true;
        symbols.push_back(terminalSymbol[i]);
    }
}

set<string> Parser::dfsFirstSet(string symbol) {
    if (!grammar[symbol].firstSet.empty()) {
        return grammar[symbol].firstSet;
    }
    if (grammar[symbol].isTerminalSymbol) {
        grammar[symbol].firstSet.insert(symbol);
        return grammar[symbol].firstSet;
    }
    for (int i = 0; i < grammar[symbol].expression.size(); i++) {
        for (int j = 0; j < grammar[symbol].expression[i].size(); j++) {
            set<string> temp = dfsFirstSet(grammar[symbol].expression[i][j]);
            grammar[symbol].firstSet.insert(temp.begin(), temp.end());
            if (temp.find("EPSILON") == temp.end()) {
                break;
            }
        }
    }
    if (grammar[symbol].firstSet.find("EPSILON") != grammar[symbol].firstSet.end()) {
        grammar[symbol].canBeEmpty = true;
    }
    return grammar[symbol].firstSet;
}

void Parser::initializeFirstSet() {
    for (int i = 0; i < symbols.size(); i++) {
        dfsFirstSet(symbols[i]);
    }
}

bool Parser::checkChange(set<string>& follow, string target) {
    set<string> temp = follow;
    follow.insert(target);
    if (follow != temp) {
        return true;
    }
    return false;
}

bool Parser::checkChangeWhenInitializeFollowSet(string symbol) {
    bool flag = false;
    if (grammar[symbol].isStartSymbol) {
        flag = checkChange(grammar[symbol].followSet, "$");
    }
    for (int i = 0; i < grammar[symbol].expression.size(); i++) {
        for (int j = 0; j < grammar[symbol].expression[i].size() - 1; j++) {
            string nowSymbol = grammar[symbol].expression[i][j];
            string nextSymbol = grammar[symbol].expression[i][j + 1];
            if (!grammar[nowSymbol].isTerminalSymbol) {
                for (set<string>::iterator it = grammar[nextSymbol].firstSet.begin();
                    it != grammar[nextSymbol].firstSet.end(); it++) {
                    if (*it != "EPSILON") {
                        flag = checkChange(grammar[nowSymbol].followSet, *it);
                    }
                }
                if (grammar[nextSymbol].firstSet.find("EPSILON") != grammar[nextSymbol].firstSet.end()) {
                    for (set<string>::iterator it = grammar[symbol].followSet.begin();
                        it != grammar[symbol].followSet.end(); it++) {
                        flag = checkChange(grammar[nowSymbol].followSet, *it);
                    }
                }
            }
        }
        string lastSymbol = grammar[symbol].expression[i].back();
        if (!grammar[lastSymbol].isTerminalSymbol) {
            for (set<string>::iterator it = grammar[symbol].followSet.begin();
                it != grammar[symbol].followSet.end(); it++) {
                flag = checkChange(grammar[lastSymbol].followSet, *it);
            }
        }
    }
    return flag;
}

void Parser::initializeFollowSet() {
    bool changeFlag = true;
    while (changeFlag) {
        changeFlag = false;
        for (int i = 0; i < symbols.size(); i++) {
            if (!grammar[symbols[i]].isTerminalSymbol) {
                if (checkChangeWhenInitializeFollowSet(symbols[i])) {
                    changeFlag = true;
                }
            }
        }
    }
}

void Parser::initializePredictSet() {
    for (int i = 0; i < symbols.size(); i++) {
        string symbol = symbols[i];
        for (int j = 0; j < grammar[symbol].expression.size(); j++) {
            string alpha = grammar[symbol].expression[j].front();
            for (set<string>::iterator it = grammar[alpha].firstSet.begin();
                it != grammar[alpha].firstSet.end(); it++) {
                if (grammar[*it].isTerminalSymbol && *it != "EPSILON") {
                    predictSet[symbol][*it] = grammar[symbol].expression[j];
                }
            }
            if (grammar[alpha].firstSet.find("EPSILON") != grammar[alpha].firstSet.end()) {
                for (set<string>::iterator it = grammar[symbol].followSet.begin();
                    it != grammar[symbol].followSet.end(); it++) {
                    if (grammar[*it].isTerminalSymbol && *it != "EPSILON") {
                        predictSet[symbol][*it] = grammar[symbol].expression[j];
                    }
                }
                if (grammar[symbol].followSet.find("$") != grammar[symbol].followSet.end()) {
                    predictSet[symbol]["$"] = grammar[symbol].expression[j];
                }
            }
        }
    }
}

void Parser::printGrammar() {
    cout << "***********************************************************" << endl;
    cout << "***                       Grammar                       ***" << endl;
    cout << "***********************************************************" << endl;
    for (int i = 0; i < symbols.size(); i++) {
        if (!grammar[symbols[i]].isTerminalSymbol) {
            cout << symbols[i] << " -> ";
            for (int j = 0; j < grammar[symbols[i]].expression.size(); j++) {
                if (j != 0) {
                    cout << " | ";
                }
                for (int k = 0; k < grammar[symbols[i]].expression[j].size(); k++) {
                    if (k != 0) {
                        cout << " ";
                    }
                    cout << grammar[symbols[i]].expression[j][k];
                }
            }
            cout << endl;
        }
    }
    cout << "***********************************************************" << endl;
    cout << endl;
}

void Parser::printSet(set<string> tempSet, string symbol) {
    cout << symbol << " {";
    for (set<string>::iterator it = tempSet.begin();
        it != tempSet.end(); it++) {
        if (it != tempSet.begin()) {
            cout << ", ";
        }
        cout << *it;
    }
    cout << "}" << endl;
}

void Parser::printFirstSet() {
    cout << "***********************************************************" << endl;
    cout << "***                     First Table                     ***" << endl;
    cout << "***********************************************************" << endl;
    for (int i = 0; i < symbols.size(); i++) {
        printSet(grammar[symbols[i]].firstSet, symbols[i]);
    }
    cout << "***********************************************************" << endl;
    cout << endl;
}

void Parser::printFollowSet() {
    cout << "***********************************************************" << endl;
    cout << "***                     Follow Table                    ***" << endl;
    cout << "***********************************************************" << endl;
    for (int i = 0; i < symbols.size(); i++) {
        if (!grammar[symbols[i]].isTerminalSymbol) {
            printSet(grammar[symbols[i]].followSet, symbols[i]);
        }
    }
    cout << "***********************************************************" << endl;
    cout << endl;
}

void Parser::printPredictSet() {
    cout << "***********************************************************" << endl;
    cout << "***                    Predict Table                    ***" << endl;
    cout << "***********************************************************" << endl;
    for (int i = 0; i < symbols.size(); i++) {
        if (!grammar[symbols[i]].isTerminalSymbol) {
            for (map<string, vector<string> >::iterator itt = predictSet[symbols[i]].begin();
                itt != predictSet[symbols[i]].end(); itt++) {
                cout << symbols[i] << ", " << itt->first << " -------->";
                for (int j = 0; j < itt->second.size(); j++) {
                    cout << " " << itt->second[j];
                }
                cout << endl;
            }
        }
    }
    cout << "***********************************************************" << endl;
    cout << endl;
}

void Parser::initialize() {
    initializeNonTerminalSymbol();
    initializeTerminalSymbol();
    initializeFirstSet();
    initializeFollowSet();
    initializePredictSet();
}

bool Parser::doCreateStatement(vector<Node> statement, string& message) {
    string tableName = statement[2].name;
    vector<string> primaryKeys;
    vector<string> fields;
    vector<int> values;
    bool flag = false;
    bool divideZero = false;
    for (int i = 4; i < statement.size() - 2; i++) {
        if (statement[i].token == "ID") {
            fields.push_back(statement[i].name);
            i += 2;
            if (statement[i].token == "DEFAULT") {
                i += 2;
                vector<Node> expression;
                while (statement[i].token != "COMMA" && i < statement.size() - 2) {
                    expression.push_back(statement[i]);
                    i++;
                }
                values.push_back(calculateArithmeticExpression(expression, divideZero));
                if (divideZero) {
                    message = "Number divided by 0 in the expressions";
                    return false;
                }
            } else {
                values.push_back(0);
            }
        } else {
            if (flag) {
                message = "The statment has two primary key declarations";
                return false;
            }
            i += 3;
            while (statement[i].token != "RP" && i < statement.size() - 2) {
                if (statement[i].token == "ID") {
                    primaryKeys.push_back(statement[i].name);
                }
                i++;
            }
            flag = true;
            i++;
        }
    }
    return Database::getInstance()->create(tableName, fields, values, primaryKeys, message);
}

bool Parser::doInsertStatement(vector<Node> statement, string& message) {
    string tableName = statement[2].name;
    vector<string> fields;
    vector<int> values;
    int index = 4;
    bool divideZero = false;
    while (statement[index].token != "RP") {
        if (statement[index].token == "ID") {
            fields.push_back(statement[index].name);
        }
        index++;
    }
    index += 3;
    while (index < statement.size() - 2) {
        vector<Node> expression;
        while (index < statement.size() - 2 && statement[index].token != "COMMA") {
            expression.push_back(statement[index]);
            index++;
        }
        index++;
        values.push_back(calculateArithmeticExpression(expression, divideZero));
        if (divideZero) {
            message = "Number divided by 0 in the expressions";
            return false;
        }
    }
    return Database::getInstance()->insert(tableName, fields, values, message);
}

bool Parser::doDeleteStatement(vector<Node> statement, string& message) {
    string tableName = statement[2].name;
    vector<Node> conditions;
    int index = 4;
    while (index < statement.size() - 1) {
        conditions.push_back(statement[index++]);
    }
    return Database::getInstance()->deleteData(tableName, conditions, message);
}

bool Parser::doSelectStatement(vector<Node> statement, string& message) {
    vector<string> fields;
    vector<Node> conditions;
    int index = 1;
    while (statement[index].token != "FROM") {
        if (statement[index].token != "COMMA") {
            fields.push_back(statement[index].name);
        }
        index++;
    }
    string tableName = statement[++index].name;
    index += 2;
    while (index < statement.size() - 1) {
        conditions.push_back(statement[index++]);
    }
    return Database::getInstance()->query(tableName, fields, conditions, message);
}

void Parser::alertError(string error, int row) {
    cout << "Error (row: " << row << "):  " << error << endl << endl;
}

void Parser::grammarError(Node token, vector<string> input) {
    cout << "Grammar Error (row: " << token.row << ", column: " << token.col
        << ") : " << endl;
    cout << input[token.row - 1] << endl;
    for (int i = 0; i < token.col - 1; i++) {
        cout << " ";
    }
    for (int i = 0; i < token.name.size(); i++) {
        cout << "^";
    }
    cout << endl << endl;
}

void Parser::errorRecovery(stack<string>& parserStack,
    vector<Node>& statement, vector<Node> tokenList, int& index) {
    while (!parserStack.empty()) {
        parserStack.pop();
    }
    parserStack.push("ssql_stmt");
    statement.clear();
    while (index < tokenList.size()) {
        if (tokenList[index++].token == "SEM") {
            break;
        }
    }
}

void Parser::work(vector<Node> tokenList, vector<string> input) {
    stack<string> parserStack;
    vector<Node> statement;
    parserStack.push("ssql_stmt");
    set<int> debug;
    int index = 0;
    debug.insert(1);
    debug.insert(2);
    while (debug.insert(3), !parserStack.empty() && index < tokenList.size()) {
        string symbol = parserStack.top();
        debug.insert(4);
        if (symbol == tokenList[index].token) {
            debug.insert(5);
            statement.push_back(tokenList[index]);
            parserStack.pop();
            index++;
            if (symbol == "SEM") {
                string message;
                bool succeed = false;
                debug.insert(6);
                if (statement.front().token == "CREATE") {
                    debug.insert(7);
                    succeed = doCreateStatement(statement, message);
                } else if (debug.insert(8), statement.front().token == "INSERT") {
                    debug.insert(9);
                    succeed = doInsertStatement(statement, message);
                } else if (debug.insert(10), statement.front().token == "DELETE") {
                    succeed = doDeleteStatement(statement, message);
                    debug.insert(11);
                } else if (debug.insert(12), statement.front().token == "SELECT") {
                    debug.insert(13);
                    succeed = doSelectStatement(statement, message);
                }
                debug.insert(14);
                if (succeed) {
                    debug.insert(15);
                    cout << message << endl << endl;
                } else {
                    debug.insert(16);
                    alertError(message, statement.front().row);
                }
                debug.inset(17);
                statement.clear();
                parserStack.push("ssql_stmt");
            }
        } else if (debug.insert(18), grammar[symbol].isTerminalSymbol) {
            grammarError(tokenList[index], input);
            debug.insert(19);
            errorRecovery(parserStack, statement, tokenList, index);
        } else if (debug.insert(20), predictSet[symbol][tokenList[index].token].empty()) {
            grammarError(tokenList[index], input);
            debug.insert(21);
            errorRecovery(parserStack, statement, tokenList, index);
        } else {

            debug.insert(22);
            parserStack.pop();
            vector<string> expressions = predictSet[symbol][tokenList[index].token];
            for (int i = expressions.size() - 1;debug.insert(23), i >= 0; i--) {
                debug.insert(24);
                if (expressions[i] != "EPSILON") {
                    debug.insert(25);
                    parserStack.push(expressions[i]);
                }
            }
        }
    }
    debug.insert(26);
    if (!statement.empty()) {
        debug.insert(27);
        alertError("Lack of words at the tail of the statement.", statement.front().row);
    }
    debug.insert(28);
    for (set<int>::iterator it = debug.beign(); it != debug.end(); it++) {
        cout << *it << "," << endl;
    }
}
