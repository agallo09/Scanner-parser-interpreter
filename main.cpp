#include "Scanner.h"
#include "Token.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char*argv[]) {
  ifstream in(argv[1]);
    string input((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    Scanner s(input);
    Token t = s.scanToken();
    cout << t.toString() << endl;
    return 0;
}