#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Token.h"
#include "Parameter.h"
#include "Predicate.h"
#include "Rule.h"
#include "DatalogProgram.h"

class Parser {
private:
    std::vector<Token> tokens;
    size_t current = 0;

    const Token& currentToken() const {
        if (current >= tokens.size()) {
            return tokens.back();
        }
        return tokens[current];
    }

    void advance() {
        if (current < tokens.size()) current++;
    }

    void match(TokenType expected) {
        if (currentToken().getType() == expected) {
            advance();
        } else {
            throw currentToken(); 
        }
    }

public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens) {}
    DatalogProgram datalogProgram() {
        DatalogProgram dprog;

        match(SCHEMES);
        match(COLON);
        dprog.addScheme(scheme());
        schemeList(dprog);

        match(FACTS);
        match(COLON);
        factList(dprog);

        match(RULES);
        match(COLON);
        ruleList(dprog);

        match(QUERIES);
        match(COLON);
        dprog.addQuery(query());
        queryList(dprog);

        match(END);

        return dprog;
    }

private:

    void schemeList(DatalogProgram& dprog) {
        if (currentToken().getType() == ID) {
            dprog.addScheme(scheme());
            schemeList(dprog);
        }
        // else lambda (do nothing)
    }

    Predicate scheme() {
        Predicate pred(currentToken().toString()); 
        std::string name = currentToken().toString(); 
        name = currentToken().getValue();
        match(ID);
        Predicate predObj(name);
        match(LEFT_PAREN);
        predObj.addParameter(Parameter(currentToken().getValue(), false));
        match(ID);
        idList(predObj);
        match(RIGHT_PAREN);
        return predObj;
    }

    void idList(Predicate& pred) {
        if (currentToken().getType() == COMMA) {
            match(COMMA);
            pred.addParameter(Parameter(currentToken().getValue(), false));
            match(ID);
            idList(pred);
        }
        // else lambda 
    }

    // grammar: factList -> fact factList | lambda
    void factList(DatalogProgram& dprog) {
        if (currentToken().getType() == ID) {
            dprog.addFact(fact());
            factList(dprog);
        }
        // else lambda
    }

    Predicate fact() {
        std::string name = currentToken().getValue();
        match(ID);
        Predicate pred(name);

        match(LEFT_PAREN);
        pred.addParameter(Parameter(currentToken().getValue(), true)); 
        match(STRING);
        stringList(pred);
        match(RIGHT_PAREN);
        match(PERIOD);

        return pred;
    }

    void stringList(Predicate& pred) {
        if (currentToken().getType() == COMMA) {
            match(COMMA);
            pred.addParameter(Parameter(currentToken().getValue(), true));
            match(STRING);
            stringList(pred);
        }
        // else lambda
    }

    void ruleList(DatalogProgram& dprog) {
        if (currentToken().getType() == ID) {
            dprog.addRule(rule());
            ruleList(dprog);
        }
        // else lambda
    }

    Rule rule() {
        Predicate head = headPredicate();
        match(COLON_DASH);
        Predicate firstPred = predicate();
        Rule r(head);
        r.addBodyPredicate(firstPred);
        predicateList(r);
        match(PERIOD);

        return r;
    }

    Predicate headPredicate() {
        std::string name = currentToken().getValue();
        match(ID);
        Predicate pred(name);
        match(LEFT_PAREN);
        pred.addParameter(Parameter(currentToken().getValue(), false));
        match(ID);
        idList(pred);
        match(RIGHT_PAREN);
        return pred;
    }

    Predicate predicate() {
        std::string name = currentToken().getValue();
        match(ID);
        Predicate pred(name);
        match(LEFT_PAREN);
        pred.addParameter(parameter());
        parameterList(pred);
        match(RIGHT_PAREN);
        return pred;
    }

    void predicateList(Rule& rule) {
        if (currentToken().getType() == COMMA) {
            match(COMMA);
            rule.addBodyPredicate(predicate());
            predicateList(rule);
        }
        // else lambda
    }

    void parameterList(Predicate& pred) {
        if (currentToken().getType() == COMMA) {
            match(COMMA);
            pred.addParameter(parameter());
            parameterList(pred);
        }
        // else lambda
    }

    Parameter parameter() {
        if (currentToken().getType() == STRING) {
            Parameter p(currentToken().getValue(), true);
            match(STRING);
            return p;
        } else if (currentToken().getType() == ID) {
            Parameter p(currentToken().getValue(), false);
            match(ID);
            return p;
        } else {
            throw currentToken();
        }
    }

    void queryList(DatalogProgram& dprog) {
        if (currentToken().getType() == ID) {
            dprog.addQuery(query());
            queryList(dprog);
        }
        // else lambda
    }

    Predicate query() {
        Predicate pred = predicate();
        match(Q_MARK);
        return pred;
    }
};