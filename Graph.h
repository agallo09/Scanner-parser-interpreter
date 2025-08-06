#pragma once
#include "DatalogProgram.h"
#include "Database.h"
#include "Node.h"
using namespace std;
#include <map>
#include <string>
#include <sstream>


class Graph {

 private:

  map<int,Node> nodes;

 public:

  Graph(int size) {
    for (int nodeID = 0; nodeID < size; nodeID++)
      nodes[nodeID] = Node();
  }

  void addEdge(int fromNodeID, int toNodeID) {
    nodes[fromNodeID].addEdge(toNodeID);
  }
  std::string toString() const {
    std::ostringstream out;
    for (const auto& pair : nodes) {
      int nodeID = pair.first;
      const Node& node = pair.second;
      out << "R" << nodeID << ":" << node.toString() << "\n";
    }
    return out.str();
  }

};
