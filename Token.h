#pragma once
#include <string>
#include<sstream>
using namespace std;

enum TokenType{
    COMMA,PERIOD, Q_MARK,LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY, ADD,  SCHEMES, FACTS, RULES, QUERIES, ID, STRING, COMMENT, UNDEFINED, END
    };

class Token{
    private:
        TokenType type; 
        string value;
        int line;

    string typeName(TokenType type) const {
        switch(type){
            case COMMA:
                return "COMMA";
                break;
            case PERIOD:
                return "PERIOD";
                break;
            case Q_MARK:
                return "Q_MARK";
                break;
            case LEFT_PAREN:
                return "LEFT_PAREN";
                break;
            case RIGHT_PAREN:
                return "RIGHT_PAREN";
                break;
            case COLON:
                return "COLON";
                break;
            case COLON_DASH:
                return "COLON_DASH";
                break;
            case SCHEMES:
                return "SCHEMES";
                break;
            case FACTS:
                return "FACTS";
                break;
            case RULES:
                return "RULES";
                break;    
            case QUERIES:
                return "QUERIES";
                break;
            case ID:
                return "ID";
                break;
            case STRING:
                return "STRING";
                break;
            case COMMENT:
                return "COMMENT";
                break;
            case UNDEFINED:
                return "UNDEFINED";
                break;
            case END:
                return "END";
                break;
            default:
                return "UNKNOWN";
                break;
        }
  }
    
  public:
    Token(TokenType type, string value, int line): type(type), value(value), line(line)  {};

    string toString() const {
    stringstream out;
    out << "(" << typeName(type) << "," << "\"" << value << "\"" << "," << line << ")";
    return out.str();
  }
  TokenType getType() const {
    return type;
}
string getValue() const {
    return value;
}

};
