#include "Scanner.h"
#include "Token.h"
#include "Parser.h"
#include "DatalogProgram.h"
#include "Interpreter.h"
#include "Node.h"
#include "Graph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

int main(int argc, char* argv[]) {
    //testing code for specific methods

    // predicate names for fake rules
  // first is name for head predicate
  // second is names for body predicates
 pair<string,vector<string>> ruleNames[] = {
    { "A", { "B", "C" } },
    { "B", { "A", "D" } },
    { "B", { "B" } },
    { "E", { "F", "G" } },
    { "E", { "E", "F" } },
  };
  vector<Rule> rules;

  for (auto& rulePair : ruleNames) {
    string headName = rulePair.first;
    Rule rule = Rule(Predicate(headName));
    vector<string> bodyNames = rulePair.second;
    for (auto& bodyName : bodyNames)
      rule.addBodyPredicate(Predicate(bodyName));
    rules.push_back(rule);
  }

  Graph graph = Interpreter::makeGraph(rules);
  cout << graph.toString();

    // run the program with the file as first input
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