#pragma once
#include <string>
#include<sstream>
#include "Token.h"
using namespace std;

class Scanner {
    private:
        string input;
        int line;
     public:
      Scanner(const string& input) : input(input) { }





    Token scanToken() {
    removeWhiteSpace();
    TokenType type = UNDEFINED;
    string value = "";
    int line = 0;  // You might want to track line properly later

    if (input.empty()) {
        type = END;
        value = "";
    }
    else if (input.at(0) == ',') {
        type = COMMA;
        value = ",";
        input = input.substr(1);
    }
    else if (input.at(0) == '.') {
        type = PERIOD;
        value = ".";
        input = input.substr(1);
    }
    else if (input.at(0) == '?') {
        type = Q_MAR;
        value = "?";
        input = input.substr(1);
    }
    else if (input.at(0) == '(') {
        type = LEFT_PAREM;
        value = "(";
        input = input.substr(1);
    }
    else if (input.at(0) == ')') {
        type = RIGHT_PAREN;
        value = ")";
        input = input.substr(1);
    }
    else if (input.at(0) == ':') {
        if (input.size() > 1 && input.at(1) == '-') {
            type = COLON_DASH;
            value = ":-";
            input = input.substr(2);
        } else {
            type = COLON;
            value = ":";
            input = input.substr(1);
        }
    }
    else if (input.at(0) == '*') {
        type = MULTIPLY;
        value = "*";
        input = input.substr(1);
    }
    else if (input.at(0) == '+') {
        type = ADD;
        value = "+";
        input = input.substr(1);
    }
    else if (/* check if input starts with "Schemes" keyword */) {
        // type = SCHEMES;
        // value = "Schemes";
        // update input accordingly
    }
    else if (/* check if input starts with "Facts" keyword */) {
        // type = FACTS;
        // value = "Facts";
        // update input accordingly
    }
    else if (/* check if input starts with "Rules" keyword */) {
        // type = RULES;
        // value = "Rules";
        // update input accordingly
    }
    else if (/* check if input starts with "Queries" keyword */) {
        // type = QUERIES;
        // value = "Queries";
        // update input accordingly
    }
    else if (/* check if input starts with an ID (letter followed by letters/digits) */) {
        // type = ID;
        // value = extracted ID string;
        // update input accordingly
    }
    else if (/* check if input starts with a STRING token (something like '...' or "...") */) {
        // type = STRING;
        // value = extracted string literal;
        // update input accordingly
    }
    else if (/* check if input starts with a COMMENT token */) {
        // type = COMMENT;
        // value = extracted comment;
        // update input accordingly
    }
    else {
        // Handle any characters that do not match known tokens
        // type = UNDEFINED;
        // value = first char;
        // input = input.substr(1);
    }

    return Token(type, value, line);
  }





  void removeWhiteSpace(){
    while (isspace(input.at(0))){
            input = input.substr(1);
    }
    
  }
};
