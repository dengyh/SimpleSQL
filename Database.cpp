#include <iostream>
#include <fstream>
#include <string>
#include "Database.h"

using namespace std;

 Database* Database::getInstance() {
    static Database instance;
    return &instance;
 }

 Database::Database() {
    // readFromFile();
 }

 Database::~Database() {
    // writeToFile();
 }

 void Database::readFromFile() {
    ifstream fin("database.txt");
    string word, tableName;
    while (fin >> word >> tableName) {
        Table table;
        fin >> word;
        fin >> word;
        while (word != "PrimaryKey") {
            table.fields.push_back(word);
            fin >> word;
        }
        fin >> word;
        while (word != "DefaultValue") {
            fin >> table.primaryKey[word];
            fin >> word;
        }
        fin >> word;
        while (word != "Row" && word != "End") {
            fin >> table.defaultValues[word];
            fin >> word;
        }
        while (word != "End") {
            fin >> word;
            map<string, int> row;
            while (word != "Row" && word != "End") {
                fin >> row[word];
                fin >> word;
            }
            table.rows.push_back(row);
        }
        tables[tableName] = table;
    }
    fin.close();
 }

 void Database::writeToFile() {
    ofstream fout("database.txt");
    for (map<string, Table>::iterator it = tables.begin();
        it != tables.end(); it++) {
        fout << "Table " << it->first << endl;
        Table table = it->second;

        fout << "Fields";
        for (int i = 0; i < table.fields.size(); i++) {
            fout << " " << table.fields[i];
        }
        fout << endl;

        fout << "PrimaryKey";
        for (int i = 0; i < table.fields.size(); i++) {
            fout << " " << table.fields[i] << " " << table.primaryKey[table.fields[i]];
        }
        fout << endl;

        fout << "DefaultValue";
        for (int i = 0; i < table.fields.size(); i++) {
            fout << " " << table.fields[i] << " " << table.defaultValues[table.fields[i]];
        }
        fout << endl;

        for (int i = 0; i < table.rows.size(); i++) {
            fout << "Row";
            for (int j = 0; j < table.fields.size(); j++) {
                fout << " " << table.fields[j] << " " << table.rows[i][table.fields[j]];
            }
            fout << endl;
        }
        fout << "End" << endl;
    }
    fout.close();
 }

 vector<int> Database::findVector(string keyString, vector<map<string, int> > rows) {
    vector<int> value;
    for (int i = 0; i < rows.size(); i++) {
        if (rows[i].find(keyString) != rows[i].end())
            value.push_back(rows[i].find(keyString)->second);
    }
    return value;
}

void Database::printTable(vector<string> word, vector<map<string, int> > rows) {
    vector< vector<int> > value;
    for (int i = 0; i < word.size(); i++) {
        cout << "|---------------";
    }
    cout << "|" << endl;

    for (int i = 0; i < word.size(); i++) {
        cout << "|" << '\t' << word[i] << '\t';
        value.push_back(findVector(word[i], rows));
    }
    cout << "|" << endl;
    for (int j = 0; j < rows.size(); j++) {
        for (int i = 0; i < word.size(); i++) {
            cout << "|---------------";
        }
        cout << "|" << endl;
        for (int i = 0; i < word.size(); i++) {
            if (!value[i].empty()) {
              cout << "|" << '\t' << value[i].back() << '\t';
              value[i].pop_back();
            }
        }
        cout << "|" << endl;
    }
    for (int i = 0; i < word.size(); i++) {
        cout << "|---------------";
    }
    cout << "|" << endl;
}

bool Database::create(string tableName, vector<string> fields,
    vector<int> values, vector<string> primaryKeys, string& message) {
    if (tables.find(tableName) != tables.end()) {
        message = "The table has been created before!";
        return false;
    }
    if (fields.size() > 100) {
        message = "The columns of the table is more than 100";
        return false;
    }
    if (primaryKeys.size() > 100) {
        message = "The number of the primary keys is more than 100";
    }
    Table tempTable;
    for (int i = 0; i < fields.size(); i++) {
        for (int j = i + 1; j < fields.size(); j++) {
            if (fields[i] == fields[j]) {
                message = "The table has the same fields called '" +
                    fields[i] + "' when you create it!";
                return false;
            }
        }
        tempTable.primaryKey[fields[i]] = false;
        tempTable.defaultValues[fields[i]] = values[i];
        tempTable.fields.push_back(fields[i]);
    }
    for (int i = 0; i < primaryKeys.size(); i++) {
        for (int j = i + 1; j < primaryKeys.size(); j++) {
            if (primaryKeys[i] == primaryKeys[j]) {
                message = "The primary keys has the same fields called '" + primaryKeys[i] + "'.";
                return false;
            }
        }
        bool primaryFlag = true;
        for (int j = 0; j < fields.size(); j++) {
            if (primaryKeys[i] == fields[j]) {
                primaryFlag = false;
            }
        }
        if (primaryFlag) {
            message = "The primary key called '" + primaryKeys[i] + "' doesn't in the table.";
            return false;
        }
        tempTable.primaryKey[primaryKeys[i]] = true;
    }
    tables[tableName] = tempTable;
    message = "Create table '" + tableName + "' succeed!";
    // writeToFile();
    return true;
}

bool Database::insert(string tableName, vector<string> fields,
    vector<int> values, string& message) {
    if (tables.find(tableName) == tables.end()) {
        message = "The table called '" + tableName + "' has not been created yet!";
        return false;
    }
    if (fields.size() != values.size()) {
        message = "The size of fields and the size of value don't match.";
        return false;
    }
    Table& table = tables[tableName];
    map<string, int> tempRow = table.defaultValues;
    for (int i = 0; i < fields.size(); i++) {
        for (int j = i + 1; j < fields.size(); j++) {
            if (fields[i] == fields[j]) {
                message = "The table has the same fields called '" + fields[i] + "' when you create it!";
                return false;
            }
        }
        if (table.defaultValues.find(fields[i]) == table.defaultValues.end()) {
            message = "The field called '" + fields[i] + "' doesn't in this table!";
            return false;
        }
        tempRow[fields[i]] = values[i];
    }
    for (int i = 0; i < table.fields.size(); i++) {
        if (table.primaryKey[table.fields[i]]) {
            for (int j = 0; j < table.rows.size(); j++) {
                if (table.rows[j][table.fields[i]] == tempRow[table.fields[i]]) {
                    message = "The primary key '" + table.fields[i] + "' has the same value '" +
                        convertIntegerToString(tempRow[table.fields[i]]) + "' after you insert!";
                    return false;
                }
            }
        }
    }
    table.rows.push_back(tempRow);
    message = "insert value to table '" + tableName + "' succeed!";
    // writeToFile();
    return true;
}

bool Database::query(string tableName, vector<string> fields,
    vector<Node> conditions, string& message) {
    if (tables.find(tableName) == tables.end()) {
        message = "The table called '" + tableName + "' has not been created yet!";
        return false;
    }
    Table table = tables[tableName];
    vector<map<string, int> > querySet;
    for (int i = 0; i < conditions.size(); i++) {
        if (conditions[i].token == "ID") {
            if (table.defaultValues.find(conditions[i].name) == table.defaultValues.end()) {
                message = "The field in conditions called '" + conditions[i].name + "' doesn't in this table!";
                return false;
            }
        }
    }
    for (int i = 0; i < fields.size(); i++) {
        if (fields[i] != "*") {
            if (table.defaultValues.find(fields[i]) == table.defaultValues.end()) {
                message = "The field selected called '" + fields[i] + "' doesn't in this table!";
                return false;
            }
        }
    }
    for (int i = 0; i < table.rows.size(); i++) {
        bool divideZero = false;
        if (calculateBooleanExpression(conditions, table.rows[i], divideZero)) {
            querySet.push_back(table.rows[i]);
        }
        if (divideZero) {
            message = "Number divided by 0 in the expressions";
            return false;
        }
    }
    if (fields.front() == "*") {
        printTable(table.fields, querySet);
    } else {
        printTable(fields, querySet);
    }
    message = "Query OK, " + convertIntegerToString(querySet.size()) + " rows affected."; 
    return true;
}

bool Database::deleteData(string tableName,
    vector<Node> conditions, string& message) {
    if (tables.find(tableName) == tables.end()) {
        message = "The table called '" + tableName + "' has not been created yet!";
        return false;
    }
    Table& table = tables[tableName];
    for (int i = 0; i < conditions.size(); i++) {
        if (conditions[i].token == "ID") {
            if (table.defaultValues.find(conditions[i].name) == table.defaultValues.end()) {
                message = "The field in conditions called " + conditions[i].name + " doesn't in this table!";
                return false;
            }
        }
    }
    int total = 0;
    for (vector<map<string, int> >::iterator it = table.rows.begin(); 
        it != table.rows.end();) {
        bool divideZero = false;
        if (calculateBooleanExpression(conditions, *it, divideZero)) {
            if (divideZero) {
                message = "Number divided by 0 in the expressions";
                return false;
            }
            table.rows.erase(it);
            total++;
        } else {
            it++;
        }
        if (divideZero) {
            message = "Number divided by 0 in the expressions";
            return false;
        }
    }
    message = "Delete OK, " + convertIntegerToString(total) + " rows affected."; 
    // writeToFile();
    return true;
}
