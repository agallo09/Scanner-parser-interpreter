#include "Scanner.h"
#include "Token.h"
#include "Parser.h"
#include "DatalogProgram.h"
#include "Interpreter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

int main(int argc, char* argv[]) {
  Relation studentRelation("students", Scheme({"ID", "Name", "Major"}));

    vector<vector<string>> studentValues = {
        {"'42'", "'Ann'", "'CS'"},
        {"'64'", "'Ned'", "'EE'"}
    };

    for (const auto& values : studentValues) {
        studentRelation.addTuple(Tuple(values));
    }

    // Join with itself 
    studentRelation.join(studentRelation);

    // courseRelation
    Relation courseRelation("courses", Scheme({"ID", "Course"}));

    vector<vector<string>> courseValues = {
        {"'42'", "'CS 100'"},
        {"'32'", "'CS 232'"}
    };

    for (const auto& values : courseValues) {
        courseRelation.addTuple(Tuple(values));
    }

    // Join boths
    cerr << "now both joined"<<endl;
    studentRelation.join(courseRelation);



    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <inputfile>" << endl;
        return 1;
    }

    ifstream inputFile(argv[1]);
    if (!inputFile) {
        cerr << "Could not open file: " << argv[1] << endl;
        return 1;
    }

    stringstream buffer;
    buffer << inputFile.rdbuf();
    string inputText = buffer.str();

    Scanner scanner(inputText);
    vector<Token> tokens;

    while (true) {
        Token token = scanner.scanToken();
        tokens.push_back(token);
        if (token.getType() == END) break;
    }

    Parser parser(tokens);

    try {
        DatalogProgram datalogProgram = parser.datalogProgram();
        Interpreter interpreter(datalogProgram);
        interpreter.evaluate();  
    } catch (const Token& errorToken) {
        cout << "Failure!" << endl;
        cout << "  " << errorToken.toString() << endl;
        return 1;
    }

    return 0;
}