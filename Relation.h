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
#include <unordered_map>
#include <utility> 


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
   static bool joinable(const Scheme& leftScheme, const Scheme& rightScheme,
                     const Tuple& leftTuple, const Tuple& rightTuple) {
    unordered_map<string, pair<int,int>> sharedAttributes;

    // shared attributes
    for (int i = 0; i < (int)leftScheme.size(); ++i) {
        const string& leftName = leftScheme.at(i);
        const string& leftValue = leftTuple.at(i);
        cout << "left name: " << leftName << " value: " << leftValue << endl;
        for (int j = 0; j < (int)rightScheme.size(); ++j) {
            const string& leftName = rightScheme.at(j);
            const string& leftValue = rightTuple.at(j);
            cout << "right name: " << leftName << " value: " << leftValue << endl;
            if (leftScheme.at(i) == rightScheme.at(j)) {
                sharedAttributes[leftScheme.at(i)] = {i, j};
            }
        }
    }
    // check if empty
    if (sharedAttributes.empty()) {
        return false; 
    }
    // Compare values at shared attribute indexes
    for (auto& pair : sharedAttributes) {
        const string& attr = pair.first;
        const std::pair<int, int>& indexes = pair.second;
        int leftIdx = indexes.first;
        int rightIdx = indexes.second;
        if (leftTuple.at(leftIdx) != rightTuple.at(rightIdx)) {
            return false; 
        }
    }

    return true; 
    }

    Relation join(const Relation& right) {
        const Relation& left = *this;

    // Loop over the left tuples
    for (const Tuple& leftTuple : left.tuples) {
        cout << "left tuple: " << leftTuple.toString(left.scheme) << endl;

        // Loop over the right tuples
        for (const Tuple& rightTuple : right.tuples) {
            cout << "right tuple: " << rightTuple.toString(right.scheme) << endl;

            // This is just the debug version. Actual joining logic will come later.
        }
    }

    return right;
    }
    

};