#include <vector>
#include "Token.h"
using namespace std;
#include <string>
#include<sstream>
#include <iostream>
#include <fstream>

class Parser {
 private:
  vector<Token> tokens;
 public:
  Parser(const vector<Token>& tokens) : tokens(tokens) { }

TokenType tokenType() const {
    return tokens.at(0).getType();
  }
  void advanceToken() {
    tokens.erase(tokens.begin());
  }
  void throwError() {
    cout << "error" << endl;
  }
  void match(TokenType t) {
    cout << "match: " << t << endl;

    if (tokens.empty()) {
        cout << "Syntax Error: unexpected end of input." << endl;
        throwError();
        return;
    }

    if (tokens.front().getType() == t) {
        advanceToken();
    } else {
        
        throwError();
    }
    };

    //idList grammar rule
     void idList() {
    if (tokenType() == COMMA) {
      match(COMMA);
      match(ID);
      idList();
    } else {
      // lambda
    }
  }
  //schemes grammar rule
  void scheme() {
    // add code for parsing a 'scheme'
    if (tokenType() == ID) {
        match(ID);
        match(LEFT_PAREN);
        match(ID);
        idList();
        match(RIGHT_PAREN);
    }
  }
};
