#pragma once
#include "Node.h"
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>
using namespace std;

class Graph {
private:
    map<int, Node> nodes;

    void dfs(int node, set<int>& visited, vector<int>& postorder) const {
        visited.insert(node);
        for (int nbr : nodes.at(node).getAdjacent())
            if (visited.find(nbr) == visited.end())
                dfs(nbr, visited, postorder);
        postorder.push_back(node);
    }

    void dfsSCC(int node, set<int>& visited, vector<int>& component) const {
        visited.insert(node);
        component.push_back(node);
        for (int nbr : nodes.at(node).getAdjacent())
            if (visited.find(nbr) == visited.end())
                dfsSCC(nbr, visited, component);
    }

public:
    Graph(int size) {
        for (int i = 0; i < size; i++)
            nodes[i] = Node();
    }

    void addEdge(int from, int to) {
        nodes[from].addEdge(to);
    }

    string toString() const {
        ostringstream out;
        vector<int> keys;
        for (auto& [id, node] : nodes)
            keys.push_back(id);
        sort(keys.begin(), keys.end());

        for (int id : keys) {
            out << "R" << id << ":";
            string adjStr = nodes.at(id).toString();
            if (!adjStr.empty()) out << adjStr;
            out << "\n";
        }
        return out.str();
    }

    Graph reverseGraph() const {
        Graph reversed((int)nodes.size());
        for (auto& [from, node] : nodes)
            for (int to : node.getAdjacent())
                reversed.addEdge(to, from);
        return reversed;
    }

    vector<int> dfsForestOnReverse() const {
        Graph reversed = reverseGraph();
        set<int> visited;
        vector<int> postorder;
        for (auto& [node, _] : reversed.nodes) {
            if (visited.find(node) == visited.end())

                reversed.dfs(node, visited, postorder);
        }
        return postorder;
    }

    vector<vector<int>> computeSCCs() const {
        vector<int> postorder = dfsForestOnReverse();
        set<int> visited;
        vector<vector<int>> sccs;
        for (auto it = postorder.rbegin(); it != postorder.rend(); ++it) {
            int node = *it;
            if (visited.find(node) == visited.end()) {
                vector<int> component;
                dfsSCC(node, visited, component);
                sccs.push_back(component);
            }
        }
        return sccs;
    }
    const map<int, Node>& getNodes() const { return nodes; }
};