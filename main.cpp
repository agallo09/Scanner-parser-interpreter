#include "Scanner.h"
#include "Token.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char*argv[]) {
  ifstream in(argv[1]);
    string input((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    Scanner s(input);
    int totalTokens = 0;
    Token t = s.scanToken();
      while (t.getType() != END) {
        cout << t.toString() << endl;
        totalTokens ++;
        t = s.scanToken();
    }

    // Print END token too
    cout << t.toString() << endl;
    totalTokens++;
    cout << "Total tokens = " << totalTokens << endl;
    return 0;
}