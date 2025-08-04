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
        evaluateRules();  
        evaluateQueries();
        Relation evaluatePredicate(const Predicate& pred);
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
    
    //evaluate Predicate
    Relation evaluatePredicate(const Predicate& pred) {
    Relation rel = database.getRelation(pred.getName());
    std::map<std::string, int> seen;
    std::vector<int> proj;
    std::vector<std::string> rename;

    const auto& params = pred.getParameters();

    for (size_t i = 0; i < params.size(); ++i) {
        const std::string& val = params[i].getValue();
        if (params[i].getIsString()) {
            rel = rel.select(i, val);  // Constant string → selection
        } else {
            if (seen.count(val)) {
                rel = rel.select(seen[val], i);  // Same variable → equality selection
            } else {
                seen[val] = i;
                proj.push_back(i);       // New variable → remember index
                rename.push_back(val);   // ...and its name
            }
        }
    }

    // Only project/rename if there are variables
    if (!proj.empty()) {
        rel = rel.project(proj);
        rel = rel.rename(rename);
    }

    return rel;
}
    //evaluate facts
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
    
    //evaluate queties
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

    //evaluate rules, new
    void evaluateRules() {
    std::cout << "Rule Evaluation" << std::endl;

    int passes = 0;
    bool changed;

    do {
        changed = false;
        passes++;

        for (const Rule& rule : program.getRules()) {
            std::cout << rule.toString() << std::endl;

            std::vector<Relation> predicateResults;

            for (const Predicate& pred : rule.getBody()) {
                predicateResults.push_back(evaluatePredicate(pred));
            }

            // Join all predicate results
            Relation combined = predicateResults[0];
            for (size_t i = 1; i < predicateResults.size(); ++i) {
                combined = combined.join(predicateResults[i]);
            }

            // Project attributes in head predicate order
            std::vector<std::string> headNames;
            for (const Parameter& param : rule.getHead().getParameters()) {
                headNames.push_back(param.getValue());
            }

            std::vector<int> indices;
            for (const std::string& name : headNames) {
                bool found = false;
                for (size_t i = 0; i < combined.getScheme().size(); ++i) {
                    if (combined.getScheme().at(i) == name) {
                        indices.push_back(i);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    std::cerr << "Error: Attribute '" << name << "' not found in joined relation scheme.\n";
                    std::cerr << "Joined scheme: ";
                    for (const auto& attr : combined.getScheme().toVector()) {
                        std::cerr << attr << " ";
                        }
                        std::cerr << std::endl;
                        throw std::runtime_error("Projection index build failed");
                    }
                }

            Relation projected = combined.project(indices);

            // Rename to match head scheme
            Relation renamed = projected.rename(database.getRelation(rule.getHead().getName()).getScheme().getNames());

            // Union with existing relation in the database
            Relation& target = database.getRelation(rule.getHead().getName());
            int before = target.size();
            target.unionWith(renamed);
            int after = target.size();

            // Output new tuples
            for (const Tuple& t : renamed.getTuples()) {
                if (!target.contains(t)) continue; // Only new ones
                std::cout << "  " << t.toString(target.getScheme()) << std::endl;
            }

            if (after > before) {
                changed = true;
            }
        }
    } while (changed);

    std::cout << std::endl;
    std::cout << "Schemes populated after " << passes << " passes through the Rules." << std::endl;
    std::cout << std::endl;
}


};