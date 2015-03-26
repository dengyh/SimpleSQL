all: ssql clear

ssql: Lexer.o Parser.o base.o Database.o
	g++ -o ssql Lexer.o Parser.o  base.o Database.o main.cpp

ari: ariTest clear runTest

ariTest: Lexer.o Parser.o base.o Database.o
	g++ -o test Lexer.o Parser.o base.o Database.o testArithmetic.cpp

bool: boolTest clear runTest

boolTest: Lexer.o Parser.o base.o Database.o
	g++ -o test Lexer.o Parser.o base.o Database.o testBoolean.cpp

grammar: grammarTest clear runTest

grammarTest: Lexer.o Parser.o base.o Database.o
	g++ -o test Lexer.o Parser.o base.o Database.o testGrammar.cpp

output: outputTables clear runTest

outputTables: Lexer.o Parser.o base.o Database.o
	g++ -o test Lexer.o Parser.o base.o Database.o outputGrammar.cpp

base.o: base.h base.cpp
	g++ -c base.h base.cpp

Lexer.o: Lexer.h Lexer.cpp base.h base.cpp
	g++ -c Lexer.h Lexer.cpp base.h base.cpp

Parser.o: Database.h Database.cpp Parser.h Parser.cpp base.h base.cpp
	g++ -c Parser.h Parser.cpp Database.h Database.cpp base.h base.cpp

Database.o: Database.h Database.cpp base.h base.cpp
	g++ -c  Database.h Database.cpp base.h base.cpp

run:
	./ssql

runTest:
	./test

clear:
	rm *.o *.gch