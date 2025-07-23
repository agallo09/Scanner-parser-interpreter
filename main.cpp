#include "Scanner.h"
#include "Token.h"
#include "Parser.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "DatalogProgram.h"
#include "Scheme.h"
#include "Tuple.h"



using namespace std;

int main(int argc, char*argv[]) {
  
    vector<string> names = { "ID", "Name", "Major" };

  Scheme scheme(names);

  vector<string> values[] = {
    {"'42'", "'Ann'", "'CS'"},
    {"'32'", "'Bob'", "'CS'"},
    {"'64'", "'Ned'", "'EE'"},
    {"'16'", "'Jim'", "'EE'"},
  };

  for (auto& value : values) {
    Tuple tuple(value);
    cout << tuple.toString(scheme) << endl;
  }

}