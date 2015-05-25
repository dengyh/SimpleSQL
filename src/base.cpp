#include <string>
#include <map>
#include <vector>
#include <set>
#include <stack>
#include <iostream>
#include "base.h"

using namespace std;

int convertStringToInteger(string s) {
    int num = 0;
    if (s[0] == '-') {
        for (int i = 1; i < s.length(); i++) {
            num = num * 10 + (s[i] - '0');
        }
        num = 0 - num;
    } else {
        for (int i = 0; i < s.length(); i++) {
            num = num * 10 + (s[i] - '0');
        }
    }
    return num;
}

string convertIntegerToString(int n) {
    string s;
    bool flag = false;
    if (n == 0) {
        s = "0";
    } else {
        if (n < 0) {
            n = -n;
            flag = true;
        }
        while (n > 0) {
            s = (char)(n % 10 + '0') + s;
            n = n / 10;
        }
    }
    if (flag) {
        s = "-" + s;
    }
    return s;
}

bool isCompareSymbol(Node node) {
    if (node.token == "NOTEQUAL" || node.token == "EQUAL" ||
        node.token == "LT" || node.token == "GT" ||
        node.token == "LTE" || node.token == "GTE") {
        return true;
    }
    return false;
}

bool isCalculateSymbol(Node node) {
    if (node.token == "ADD" || node.token == "SUB" ||
        node.token == "MUL" || node.token == "DIV" ||
        node.token == "NUM") {
        return true;
    }
    return false;
}

bool isBooleanSymbol(Node node) {
    if (node.token == "LP" || node.token == "RP" ||
        node.token == "NOT" || node.token == "OR" ||
        node.token == "AND") {
        return true;
    }
    return false;
}

void printTokens(vector<Node> tokenList) {
    cout << "------------------------------------------" << endl;
    for (int i = 0; i < tokenList.size(); i++) {
        cout << "(" << tokenList[i].name << ", " << tokenList[i].token << ", "
            << "row " << tokenList[i].row << ", "
            << "col " << tokenList[i].col << ")" << endl;
    }
    cout << "----------------------------------------" << endl;
}

void releaseStack(stack<int>& nums, stack<string>& symbols, bool& divideZero) {
    while (!symbols.empty()) {
        if (symbols.top() == "LP") {
            break;
        }
        int temp2 = nums.top();
        nums.pop();
        int temp1 = nums.top();
        nums.pop();
        if (symbols.top() == "ADD") {
            nums.push(temp1 + temp2);
        } else if (symbols.top() == "SUB") {
            nums.push(temp1 - temp2);
        } else if (symbols.top() == "MUL") {
            nums.push(temp1 * temp2);
        } else if (symbols.top() == "DIV") {
            if (temp2 != 0) {
                nums.push(temp1 / temp2);
            } else {
                divideZero = true;
                nums.push(0);
            }
        }
        symbols.pop();
    }
}

void releasePositvieOrNegative(stack<int>& nums, stack<string>& calSymbols) {
    while (!calSymbols.empty() && (calSymbols.top() == "NEG" ||
        calSymbols.top() == "POS")) {
        if (calSymbols.top() == "NEG") {
            int temp = nums.top();
            nums.pop();
            nums.push(-temp);
        }
        calSymbols.pop();
    }
}

int calculateArithmeticExpression(vector<Node> tokens, bool& divideZero) {
    map<string, int> priority;
    stack<int> nums;
    stack<string> calSymbols;
    bool flag = true;
    priority["ADD"] = 1;
    priority["SUB"] = 1;
    priority["MUL"] = 2;
    priority["DIV"] = 2;
    priority["POS"] = 3;
    priority["NEG"] = 3;
    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i].token == "NUM") {
            nums.push(convertStringToInteger(tokens[i].name));
            flag = false;
        } else if (tokens[i].token == "LP") {
            calSymbols.push("LP");
        } else if (tokens[i].token == "RP") {
            releaseStack(nums, calSymbols, divideZero);
            calSymbols.pop();
        } else {
            if (flag) {
                if (tokens[i].token == "ADD") {
                    tokens[i].token = "POS";
                } else if (tokens[i].token == "SUB") {
                    tokens[i].token = "NEG";
                }
            } else {
                if (!calSymbols.empty() && priority[calSymbols.top()] == 3) {
                    releasePositvieOrNegative(nums, calSymbols);
                }
            }
            if (!calSymbols.empty() && priority[calSymbols.top()] >= priority[tokens[i].token] &&
                priority[calSymbols.top()] != 3) {
                releaseStack(nums, calSymbols, divideZero);
            }
            calSymbols.push(tokens[i].token);
            flag = true;
        }
    }
    releasePositvieOrNegative(nums, calSymbols);
    releaseStack(nums, calSymbols, divideZero);
    return nums.top();
}

bool calculateCompareExpression(int num1, int num2, string compare) {
    if (num1 == num2 && compare == "EQUAL") {
        return true;
    }
    if (num1 != num2 && compare == "NOTEQUAL") {
        return true;
    }
    if (num1 < num2 && compare == "LT") {
        return true;
    }
    if (num1 > num2 && compare == "GT") {
        return true;
    }
    if (num1 <= num2 && compare == "LTE") {
        return true;
    }
    if (num1 >= num2 && compare == "GTE") {
        return true;
    }
    return false;
}

void releaseBooleanStack(stack<bool>& boolean, stack<string>& boolSymbols) {
    while (!boolSymbols.empty()) {
        if (boolSymbols.top() == "LP") {
            break;
        }
        if (boolSymbols.top() == "NOT") {
            bool temp = boolean.top();
            boolean.pop();
            boolean.push(!temp);
        } else {
            bool temp1 = boolean.top();
            boolean.pop();
            bool temp2 = boolean.top();
            boolean.pop();
            if (boolSymbols.top() == "AND") {
                boolean.push(temp1 && temp2);
            } else if (boolSymbols.top() == "OR") {
                boolean.push(temp1 || temp2);
            }
        }
        boolSymbols.pop();
    }
}

void getBooleanFromCompare(int& num1, int& num2, string& compare,
    vector<Node>& temp, stack<bool>& boolean, bool& divideZero) {
    num2 = calculateArithmeticExpression(temp, divideZero);
    boolean.push(calculateCompareExpression(num1, num2, compare));
    num1 = 0;
    num2 = 0;
    compare = "";
    temp.clear();
}

bool calculateBooleanExpression(vector<Node> tokens, map<string, int> values, bool& divideZero) {
    if (tokens.empty()) {
        return true;
    }
    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i].token == "ID") {
            tokens[i].token = "NUM";
            tokens[i].name = convertIntegerToString(values[tokens[i].name]);
        }
    }
    map<string, int> priority;
    priority["OR"] = 1;
    priority["AND"] = 2;
    priority["NOT"] = 3;
    stack<bool> boolean;
    stack<string> boolSymbols;
    string compare = "";
    int num1, num2;
    vector<Node> temp;
    for (int i = 0; i < tokens.size(); i++) {
        if (isBooleanSymbol(tokens[i])) {
            if (compare != "") {
                getBooleanFromCompare(num1, num2, compare, temp, boolean, divideZero);
            }
            if (tokens[i].token == "LP") {
                boolSymbols.push("LP");
            } else if (tokens[i].token == "RP") {
                releaseBooleanStack(boolean, boolSymbols);
                boolSymbols.pop();
            } else {
                if (!boolSymbols.empty() &&
                    priority[boolSymbols.top()] > priority[tokens[i].token]) {
                    releaseBooleanStack(boolean, boolSymbols);
                }
                boolSymbols.push(tokens[i].token);
            }
        } else if (isCalculateSymbol(tokens[i])) {
            temp.push_back(tokens[i]);
        } else if (isCompareSymbol(tokens[i])) {
            num1 = calculateArithmeticExpression(temp, divideZero);
            temp.clear();
            compare = tokens[i].token;
        }
    }
    if (compare != "") {
        getBooleanFromCompare(num1, num2, compare, temp, boolean, divideZero);
    }
    releaseBooleanStack(boolean, boolSymbols);
    return boolean.top();
}