#pragma once
#include <string>
#include <sstream>

class Parameter {
private:
    std::string value;
    bool isString;  

public:
    Parameter(const std::string& val, bool isStr) : value(val), isString(isStr) {}
    std::string toString() const {
        if (isString) {
            return value ;
        }
        return value;
    }

    const std::string& getValue() const { return value; }
    bool getIsString() const { return isString; }
};