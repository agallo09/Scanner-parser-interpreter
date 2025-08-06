#pragma once
#include <vector>
#include <sstream>
#include "Predicate.h"

class Rule {
private:
    Predicate headPredicate;
    std::vector<Predicate> bodyPredicates;

public:
    Rule() = default;
    Rule(const Predicate& head) : headPredicate(head) {}

    void addBodyPredicate(const Predicate& pred) {
        bodyPredicates.push_back(pred);
    }

    const Predicate& getHead() const {
        return headPredicate;
    }

    const std::vector<Predicate>& getBody() const {
        return bodyPredicates;
    }

    std::string toString() const {
        std::stringstream out;
        out << headPredicate.toString() << " :- ";
        for (size_t i = 0; i < bodyPredicates.size(); ++i) {
            out << bodyPredicates[i].toString();
            if (i < bodyPredicates.size() - 1) {
                out << ",";
            }
        }
        return out.str();
    }
};