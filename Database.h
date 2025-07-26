#pragma once
#include <string>
#include <map>
#include "Relation.h"
#include "Scheme.h"
class Database {
private:
    std::map<std::string, Relation> relations;

public:
    void addRelation(const Relation& relation) {
    relations.insert(std::make_pair(relation.getName(), relation));
    }

    const Relation* getRelation(const std::string& name) const {
        auto it = relations.find(name);
        if (it != relations.end()) {
            return &(it->second);
        } else {
            return nullptr; 
        }
    }
    Relation& getRelation(const std::string& name) {
        auto it = relations.find(name);
            if (it == relations.end()) {
                throw std::runtime_error("Relation '" + name + "' not found in database.");
            }
            return it->second;
    }
};