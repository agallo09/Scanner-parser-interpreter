#pragma once
#include <string>
#include <vector>
#include <sstream>
#include "Parameter.h"

class Predicate {
private:
    std::string name;
    std::vector<Parameter> parameters;

public:
    Predicate() = default;
    Predicate(const std::string& n) : name(n) {}

    void addParameter(const Parameter& param) {
        parameters.push_back(param);
    }

    const std::string& getName() const {
        return name;
    }

    const std::vector<Parameter>& getParameters() const {
        return parameters;
    }

    std::string toString() const {
        std::stringstream out;
        out << name << "(";
        for (size_t i = 0; i < parameters.size(); ++i) {
            out << parameters[i].toString();
            if (i < parameters.size() - 1) {
                out << ",";
            }
        }
        out << ")";
        return out.str();
    }
};