#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Token.h"
#include "Parameter.h"
#include "Predicate.h"
#include "Rule.h"
#include "DatalogProgram.h"

class Scheme : public vector<string> {

private:

  vector<string> names;

public:

  Scheme(vector<string> names) : names(names) { }

  unsigned size() const {
    return names.size();
  }
  const string& at(int index) const {
    return names.at(index);
  }

  // TODO: add more delegation functions as needed

};