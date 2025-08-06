#pragma once
#include "DatalogProgram.h"
#include "Database.h"
using namespace std;

class Node {

 private:

  set<int> adjacentNodeIDs;

 public:

  void addEdge(int adjacentNodeID) {
    adjacentNodeIDs.insert(adjacentNodeID);
  }

  std::string toString() const {
    std::ostringstream out;
    bool first = true;
    for (int id : adjacentNodeIDs) {
      if (!first) {
        out << ",";
      }
      out << "R" << id;
      first = false;
    }
    return out.str();
  }

};
