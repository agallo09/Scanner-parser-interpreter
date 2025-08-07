#pragma once
#include "DatalogProgram.h"
#include "Database.h"
#include "Graph.h"
#include "Node.h"
#include <iostream>
#include <map>
#include <vector>
using namespace std;

class Interpreter {
private:
    DatalogProgram program;
    Database database;
    Graph dependencyGraph;

public:
    Interpreter(const DatalogProgram& program)
        : program(program), dependencyGraph(makeGraph(program.getRules())) { }

    void evaluate() {
        evaluateSchemes();
        evaluateFacts();

        // Get SCCs from the graph
        vector<vector<int>> sccs = dependencyGraph.computeSCCs();

        // Evaluate rules per SCC
        evaluateRulesPerSCC(sccs, dependencyGraph);

        evaluateQueries();
    }

    void evaluateSchemes() {
        for (const Predicate& scheme : program.getSchemes()) {
            vector<string> attributes;
            for (const Parameter& param : scheme.getParameters())
                attributes.push_back(param.getValue());

            Scheme schemeObj(attributes);
            Relation relation(scheme.getName(), schemeObj);
            database.addRelation(relation);
        }
    }

    void evaluateFacts() {
        for (const Predicate& fact : program.getFacts()) {
            vector<string> values;
            for (const Parameter& param : fact.getParameters())
                values.push_back(param.getValue());

            Tuple tuple(values);
            database.getRelation(fact.getName()).addTuple(tuple);
        }
    }

    Relation evaluatePredicate(const Predicate& pred) {
        Relation rel = database.getRelation(pred.getName());
        map<string, int> seen;
        vector<int> proj;
        vector<string> rename;
        const auto& params = pred.getParameters();

        for (size_t i = 0; i < params.size(); ++i) {
            string val = params[i].getValue();
            if (params[i].getIsString()) {
                rel = rel.select(i, val);
            } else {
                if (seen.count(val))
                    rel = rel.select(seen[val], i);
                else {
                    seen[val] = i;
                    proj.push_back(i);
                    rename.push_back(val);
                }
            }
        }
        if (!proj.empty()) {
            rel = rel.project(proj);
            rel = rel.rename(rename);
        }
        return rel;
    }

    void evaluateQueries() {
        cout<<endl;
        cout << "Query Evaluation" << endl;
        for (const Predicate& query : program.getQueries()) {
            cout << query.toString() << "?";
            Relation relation = database.getRelation(query.getName());

            map<string, int> varMap;
            vector<string> newNames;
            vector<int> projectIndices;

            const auto& params = query.getParameters();
            for (size_t i = 0; i < params.size(); ++i) {
                string val = params[i].getValue();
                if (params[i].getIsString()) {
                    relation = relation.select(i, val);
                } else {
                    if (varMap.count(val))
                        relation = relation.select(varMap[val], i);
                    else {
                        varMap[val] = i;
                        projectIndices.push_back(i);
                        newNames.push_back(val);
                    }
                }
            }

            relation = relation.project(projectIndices);
            relation = relation.rename(newNames);

            if (relation.empty()) {
                cout << " No" << endl;
            } else {
                cout << " Yes(" << relation.size() << ")" << endl;
                istringstream iss(relation.toString());
                string line;
                while (getline(iss, line)) {
                    if (!line.empty())
                        cout << "  " << line << endl;
                }
            }
        }
    }

    void evaluateRulesPerSCC(const vector<vector<int>>& sccs, const Graph& graph) {
    cout << "Dependency Graph" << endl;

    vector<int> allNodes;
    for (const auto& scc : sccs)
        for (int r : scc)
            allNodes.push_back(r);
    sort(allNodes.begin(), allNodes.end());

    for (int r : allNodes) {
        cout << "R" << r << ":";
        const auto& adj = graph.getNodes().at(r).getAdjacent();
        size_t count = 0;
        for (int nbr : adj) {
            cout << "R" << nbr;
            if (++count < adj.size()) cout << ",";
        }
        cout << "\n";
    }

    cout << endl;
    cout << "Rule Evaluation" << endl;

        for (auto scc : sccs) {
        sort(scc.begin(), scc.end());
        cout << "SCC: ";
        
        for (size_t i = 0; i < scc.size(); ++i) {
            cout << "R" << scc[i];
            if (i != scc.size() - 1) cout << ",";
        }
        cout << endl;


        bool isTrivial = (scc.size() == 1);
        bool selfLoop = false;

        if (isTrivial) {
            int r = scc[0];
            for (const Predicate& bodyPred : program.getRules()[r].getBody()) {
                if (bodyPred.getName() == program.getRules()[r].getHead().getName()) {
                    selfLoop = true;
                    break;
                }
            }
        }

        int passes = 0;
        bool changed;
        do {
            changed = false;
            passes++;

            for (int r : scc) {
                const Rule& rule = program.getRules()[r];
                cout << rule.toString() << "." << endl;

                vector<Relation> predResults;
                for (const Predicate& pred : rule.getBody())
                    predResults.push_back(evaluatePredicate(pred));

                Relation combined = predResults[0];
                for (size_t i = 1; i < predResults.size(); ++i)
                    combined = combined.join(predResults[i]);

                vector<string> headNames;
                for (const Parameter& param : rule.getHead().getParameters())
                    headNames.push_back(param.getValue());

                vector<int> indices;
                for (const string& name : headNames) {
                    bool found = false;
                    for (size_t i = 0; i < combined.getScheme().size(); ++i) {
                        if (combined.getScheme().at(i) == name) {
                            indices.push_back(i);
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                        throw runtime_error("Projection index build failed");
                }

                Relation projected = combined.project(indices);
                Relation renamed = projected.rename(database.getRelation(rule.getHead().getName()).getScheme().getNames());

                Relation& target = database.getRelation(rule.getHead().getName());

                vector<Tuple> newTuples;
                for (const Tuple& t : renamed.getTuples())
                    if (!target.contains(t)) newTuples.push_back(t);

                bool added = target.unionWith(renamed);
                if (added) {
                    changed = true;
                    for (const Tuple& t : newTuples)
                        cout << "  " << t.toString(target.getScheme()) << endl;
                }
            }
        } while (changed && (!isTrivial || selfLoop));

        cout << passes << " passes: ";
        vector<int> sortedScc = scc;
        sort(sortedScc.begin(), sortedScc.end());
        for (size_t i = 0; i < sortedScc.size(); ++i) {
            cout << "R" << sortedScc[i];
            if (i != sortedScc.size() - 1) cout << ",";
        }
        cout << endl;
    }
}

    Graph makeGraph(const vector<Rule>& rules) {
        int n = (int)rules.size();
        Graph graph(n);

        for (int fromID = 0; fromID < n; ++fromID) {
            const Rule& fromRule = rules[fromID];
            for (const Predicate& bodyPred : fromRule.getBody()) {
                for (int toID = 0; toID < n; ++toID) {
                    const Rule& toRule = rules[toID];
                    if (bodyPred.getName() == toRule.getHead().getName()) {
                        graph.addEdge(fromID, toID);
                    }
                }
            }
        }
        return graph;
    }
};