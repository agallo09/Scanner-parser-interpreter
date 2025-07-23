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

};