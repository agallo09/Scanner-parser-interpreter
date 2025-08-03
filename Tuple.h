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


class Tuple  {

private:

  vector<string> values;

public:

  Tuple(const vector<string> values) : values(values) { }

  unsigned size() const {
    return values.size();
  }

  const string& at(int index) const {
    return values.at(index);
  }

  bool operator<(const Tuple t) const {
    return values < t.values;
  }

  // TODO: add more delegation functions as needed
   string toString(const Scheme& scheme) const {
        stringstream out;
        for (unsigned i = 0; i < scheme.size(); ++i) {
            if (i > 0) out << ", ";
            out << scheme.at(i) << "=" << values.at(i);
        }
        return out.str();
    }

  //get value for project 4

  const vector<string>& getValues() const {
    return values;
}
vector<string> toVector() const {
    return values;
}

};