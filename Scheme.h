#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Token.h"
#include "Parameter.h"
#include "Predicate.h"
#include "Rule.h"
#include "DatalogProgram.h"

class Scheme {

private:

  vector<string> names;

public:

  Scheme(const vector<string> names) : names(names) { }

  unsigned size() const {
    return names.size();
  }
  const string& at(int index) const {
    return names.at(index);
  }
  
  // get names for project 4
  const vector<string>& getNames() const {
    return names;
}
std::vector<std::string> toVector() const {
    return names;
}

};