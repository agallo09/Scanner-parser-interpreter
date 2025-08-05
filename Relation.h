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
#include <algorithm> 


class Relation {

 private:

  string name;
  Scheme scheme;
  set<Tuple> tuples;

    public:
    //constructor
    Relation(const string& name, const Scheme& scheme)
    : name(name), scheme(scheme) { }
    
    //basic methods
    void addTuple(const Tuple& tuple) {
        tuples.insert(tuple);
    }
    bool empty() const { return tuples.empty(); }
    int size() const { return tuples.size(); }

    //to string
    string toString() const {
        stringstream out;
        for (const Tuple& tuple : tuples) {
            out << tuple.toString(scheme) << endl;
        }
        return out.str();
    }
    
    // select
    Relation select(int index, const string& value) const {
        Relation result(name, scheme);
        for (const Tuple& tuple : tuples) {
            if (tuple.at(index) == value) {
                result.addTuple(tuple);
            }
        }
        return result;
    }
    
    // select 2
    Relation select(int index1, int index2) const {
    Relation result(name, scheme);
    for (const Tuple& tuple : tuples) {
        if (tuple.at(index1) == tuple.at(index2)) {
            result.addTuple(tuple);
        }
    }
    return result;
}
    
    // project
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
    
    // rename    
    Relation rename(const vector<string>& newNames) const {
        Scheme newScheme(newNames);

        Relation result(name, newScheme);

        for (const Tuple& tuple : tuples) {
            result.addTuple(tuple);
        }

        return result;
    }
    
    //get anme helping methjod
    const std::string& getName() const {
    return name;
}   
    
    //joinable
    static bool joinable(const Scheme& leftScheme, const Scheme& rightScheme, const Tuple& leftTuple, const Tuple& rightTuple) {
    unordered_map<string, pair< int,int> > sharedAttributes;

    // shared attributes
    for (int i = 0; i < (int)leftScheme.size(); ++i) {
        //const string& leftName = leftScheme.at(i);
        //const string& leftValue = leftTuple.at(i);
        // cout << "left name: " << leftName << " value: " << leftValue << endl;
        for (int j = 0; j < (int)rightScheme.size(); ++j) {
            //const string& leftName = rightScheme.at(j);
            //const string& leftValue = rightTuple.at(j);
            // cout << "right name: " << leftNam e << " value: " << leftValue << endl;
            if (leftScheme.at(i) == rightScheme.at(j)) {
            sharedAttributes[leftScheme.at(i)] = std::make_pair(i, j);
            }
        }
    }
    // check if empty
    if (sharedAttributes.empty()) {
        return true; 
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

    //join 
    Relation join(const Relation& right) {
    const Relation& left = *this;
    Scheme newScheme = combineSchemes(left.scheme, right.scheme);
    Relation result("joinResult", newScheme);

    for (const Tuple& leftTuple : left.tuples) {
        for (const Tuple& rightTuple : right.tuples) {
            if (joinable(left.scheme, right.scheme, leftTuple, rightTuple)) {
                Tuple newTuple = combineTuples(leftTuple, left.scheme, rightTuple, right.scheme);
                result.addTuple(newTuple);
            }
        }
    }

    return result;
    }

    //union
    bool unionWith(const Relation& other) {
    bool addedNewTuple = false;

    for (const Tuple& t : other.tuples) {
        if (tuples.insert(t).second) { 
            //std::cout << "  " << t.toString(scheme) << std::endl;
            addedNewTuple = true;
        }
    }

    return addedNewTuple;
}
    
    //combine schemes method
    Scheme combineSchemes(const Scheme& s1, const Scheme& s2) {
    std::vector<std::string> left = s1.toVector();
    std::vector<std::string> right = s2.toVector();
    std::vector<std::string> combined = left;

    for (const std::string& attr : right) {
        if (std::find(left.begin(), left.end(), attr) == left.end()) {
            combined.push_back(attr);
        }
    }

    return Scheme(combined);
}

    //combine tupls
    Tuple combineTuples(const Tuple& t1, const Scheme& s1, const Tuple& t2, const Scheme& s2) {
    std::vector<std::string> combined = t1.toVector();

    for (int i = 0; i < (int)s2.size(); ++i) {
        const string& attr = s2.at(i);
        bool isDuplicate = false;

        for (int j = 0; j < (int)s1.size(); ++j) {
            if (s1.at(j) == attr) {
                isDuplicate = true;
                break;
            }
        }

        if (!isDuplicate) {
            combined.push_back(t2.at(i));
        }
    }

    return Tuple(combined);
}
    
    //3 helping methods
    const Scheme& getScheme() const {
        return scheme;
    }

    const set<Tuple>& getTuples() const {
        return tuples;
    }

    bool contains(const Tuple& t) const {
        return tuples.count(t) > 0;
    }

};