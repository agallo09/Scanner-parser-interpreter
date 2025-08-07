#pragma once
#include <set>
#include <string>

#include <sstream>
using namespace std;

class Node {
private:
    set<int> adjacentNodeIDs;

public:
    void addEdge(int id) {
        adjacentNodeIDs.insert(id);
    }
    const set<int>& getAdjacent() const {
        return adjacentNodeIDs;
    }
    string toString() const {
        ostringstream out;
        bool first = true;
        for (int id : adjacentNodeIDs) {
            if (!first) out << ",";
            out << "R" << id;
            first = false;
        }
        return out.str();
    }
};