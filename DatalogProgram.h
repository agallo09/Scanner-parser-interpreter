#pragma once
#include <vector>
#include <set>
#include <string>
#include <sstream>
#include "Predicate.h"
#include "Rule.h"

class DatalogProgram {
private:
    std::vector<Predicate> schemes;
    std::vector<Predicate> facts;
    std::vector<Rule> rules;
     std::vector<Predicate> queries;
    std::set<std::string> domain;  // string from the fact category

public:
    void addScheme(const Predicate& scheme) {
        schemes.push_back(scheme);
    }

    void addFact(const Predicate& fact) {
        facts.push_back(fact);
        for (const auto& param : fact.getParameters()) {
            if (param.getIsString()) {
                domain.insert(param.getValue());
            }
        }
    }

    void addRule(const Rule& rule) {
        rules.push_back(rule);
    }

    void addQuery(const Predicate& query) {
        queries.push_back(query);
    }

    const std::vector<Predicate>& getSchemes() const {
        return schemes;
    }

    const std::vector<Predicate>& getFacts() const {
        return facts;
    }

    const std::vector<Rule>& getRules() const {
        return rules;
    }

    const std::vector<Predicate>& getQueries() const {
        return queries;
    }

    const std::set<std::string>& getDomain() const {
        return domain;
    }

    std::string toString() const {
        std::stringstream out;

        out << "Schemes(" << schemes.size() << "):" << std::endl;
        for (const auto& s : schemes) {
            out << "  " << s.toString() << std::endl;
        }

        out << "Facts(" << facts.size() << "):" << std::endl;
        for (const auto& f : facts) {
            out << "  " << f.toString() << "." << std::endl;  // Facts end with a period
        }

        out << "Rules(" << rules.size() << "):" << std::endl;
        for (const auto& r : rules) {
            out << "  " << r.toString() << std::endl;
        }

        out << "Queries(" << queries.size() << "):" << std::endl;
        for (const auto& q : queries) {
            out << "  " << q.toString() << "?" << std::endl;  // Queries end with a question mark
        }

        out << "Domain(" << domain.size() << "):" << std::endl;
        for (const auto& d : domain) {
            out << "  " << d << "" << std::endl;
        }

        return out.str();
    }
};