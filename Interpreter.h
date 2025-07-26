#pragma once
#include "DatalogProgram.h"
#include "Database.h"

class Interpreter {
private:
    DatalogProgram program;
    Database database;

public:
    Interpreter(const DatalogProgram& program)
        : program(program) { }

        
    void evaluate() {
        evaluateSchemes();
        evaluateFacts();   
        evaluateQueries();
    } 

    void evaluateSchemes() {
        for (const Predicate& scheme : program.getSchemes()) {
            std::string name = scheme.getName(); 
             std::vector<std::string> attributes;

            for (const Parameter& param : scheme.getParameters()) {
                  attributes.push_back(param.getValue()); 
            }

            Scheme schemeObj(attributes);
            Relation relation(name, schemeObj);
            database.addRelation(relation);  
    }
    }
    void evaluateFacts() {
    for (const Predicate& fact : program.getFacts()) {
        std::string name = fact.getName();  
        std::vector<std::string> values;

        for (const Parameter& param : fact.getParameters()) {
            values.push_back(param.getValue());
        }

        Tuple tuple(values);
        database.getRelation(name).addTuple(tuple);
    }
    }
    void evaluateQueries() {
    for (const Predicate& query : program.getQueries()) {
        std::cout << query.toString() << "?";

        Relation relation = database.getRelation(query.getName());
        std::map<std::string, int> variableMap;
        std::vector<std::string> newNames;
        std::vector<int> projectIndices;

        const std::vector<Parameter>& params = query.getParameters();

        for (size_t i = 0; i < params.size(); ++i) {
            const std::string& value = params[i].getValue();

            if (params[i].getIsString()) {
                relation = relation.select(i, value);
            } else {
                if (variableMap.count(value)) {
                    relation = relation.select(variableMap[value], i);
                } else {
                    variableMap[value] = i;
                    projectIndices.push_back(i);
                    newNames.push_back(value);
                }
            }
        }

        relation = relation.project(projectIndices);
        relation = relation.rename(newNames);

        if (relation.empty()) {
            std::cout << " No" << std::endl;
        } else {
            std::cout << " Yes(" << relation.size() << ")" << std::endl;

            std::string output = relation.toString();
            std::istringstream stream(output);
            std::string line;
            while (std::getline(stream, line)) {
                if (!line.empty()) {
                    std::cout << "  " << line << std::endl;
                }
            }
        }
    }
}
};