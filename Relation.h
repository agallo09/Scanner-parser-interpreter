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
  Relation project(const vector<int>& columns) const {
    // Create new Scheme by selecting only the columns specified
    vector<string> newSchemeNames;
    for (int index : columns) {
        newSchemeNames.push_back(scheme.at(index));
    }
    Scheme newScheme(newSchemeNames);

    // Create new Relation with the new scheme
    Relation result(name, newScheme);

    // For each tuple, create a projected tuple and add it
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
    // Create a new Scheme with the new column names
    Scheme newScheme(newNames);

    // Create a new Relation with the same name and tuples, but new scheme
    Relation result(name, newScheme);

    // Add all existing tuples to the new relation
    for (const Tuple& tuple : tuples) {
        result.addTuple(tuple);
    }

    return result;
}

};