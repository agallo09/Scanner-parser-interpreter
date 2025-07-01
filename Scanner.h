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
    }else if (isalpha(input.at(0))) { 
        int i = 0;
        while (i < (int)input.size() && (isalpha(input.at(i)) || isdigit(input.at(i)))) {
            i++;
        }
        value = input.substr(0, i);

        if (value == "Schemes") {
            type = SCHEMES;
        } else if (value == "Facts") {
            type = FACTS;
        } else if (value == "Rules") {
            type = RULES;
        } else if (value == "Queries") {
            type = QUERIES;
        } else {
            type = ID;
        }

        input = input.substr(i);
    }else if (input.at(0) == '\'') {
        value += '\'';
        input = input.substr(1);
        bool terminated = false;

        while (!input.empty()) {
            char c = input.at(0);
            value += c;
            input = input.substr(1);

            if (c == '\'') {
                if (!input.empty() && input.at(0) == '\'') {
                    value += '\'';
                    input = input.substr(1);
                } else {
                    terminated = true;
                    break;
                }
            }
            if (c == '\n') line++;
        }

        if (terminated) {
            type = STRING;
        } else {
            type = UNDEFINED;
        }
    } else if (input.at(0) == '#') {
        value += '#';
        if (input.size() > 1 && input.at(1) == '|') {
            value += '|';
            input = input.substr(2);
            bool terminated = false;

            while (!input.empty()) {
                char c = input.at(0);
                value += c;
                input = input.substr(1);
                if (c == '\n') line++;

                if (c == '|' && !input.empty() && input.at(0) == '#') {
                    value += '#';
                    input = input.substr(1);
                    terminated = true;
                    break;
                }
            }

            if (terminated) {
                type = COMMENT;
            } else {
                type = UNDEFINED;
            }
        } else {
            input = input.substr(1);
            while (!input.empty() && input.at(0) != '\n') {
                value += input.at(0);
                input = input.substr(1);
            }
            type = COMMENT;
        }
    }else {
        type = UNDEFINED;
        value = input.substr(0, 1);
        input = input.substr(1);
    }

    return Token(type, value, line);
  }





  void removeWhiteSpace(){
    while (isspace(input.at(0))){
            input = input.substr(1);
    }
    
  }
};
