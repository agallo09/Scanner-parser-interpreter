#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Token.h"
#include "Parameter.h"
#include "Predicate.h"
#include "Rule.h"
#include "DatalogProgram.h"
#include "Scheme.h"
#include "Tuple.h"


class Relation {

 private:

  string name;
  Scheme scheme;
  set<Tuple> tuples;

 public:

  Relation(const string& name, const Scheme& scheme)
    : name(name), scheme(scheme) { }

  void addTuple(const Tuple& tuple) {
    tuples.insert(tuple);
  }
  bool empty() const { return tuples.empty(); }
  
  int size() const { return tuples.size(); }

  
  string toString() const {
        stringstream out;
        for (const Tuple& tuple : tuples) {
            out << tuple.toString(scheme) << endl;
        }
        return out.str();
    }
  Relation select(int index, const string& value) const {
        Relation result(name, scheme);
        for (const Tuple& tuple : tuples) {
            if (tuple.at(index) == value) {
                result.addTuple(tuple);
            }
        }
        return result;
    }
    Relation select(int index1, int index2) const {
    Relation result(name, scheme);
    for (const Tuple& tuple : tuples) {
        if (tuple.at(index1) == tuple.at(index2)) {
            result.addTuple(tuple);
        }
    }
    return result;
}
  Relation project(const vector<int>& columns) const {
    vector<string> newSchemeNames;
    for (int index : columns) {
        newSchemeNames.push_back(scheme.at(index));
    }
    Scheme newScheme(newSchemeNames);
     Relation result(name, newScheme);

    for (const Tuple& tuple : tuples) {
        vector<string> projectedValues;
        for (int index : columns) {
            projectedValues.push_back(tuple.at(index));
        }
        Tuple projectedTuple(projectedValues);
        result.addTuple(projectedTuple);
    }

    return result;
}
    Relation rename(const vector<string>& newNames) const {
        Scheme newScheme(newNames);

        Relation result(name, newScheme);

        for (const Tuple& tuple : tuples) {
            result.addTuple(tuple);
        }

        return result;
    }
    const std::string& getName() const {
    return name;
}
    

};