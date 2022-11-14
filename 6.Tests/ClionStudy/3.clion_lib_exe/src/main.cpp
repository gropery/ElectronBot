#include <iostream>
#include "main.h"
#include "module.h"


using namespace std;

int main() {
    cout << "version=" << VERSION <<endl;
    int x = 100;
    cout << "x=" << x <<endl;
    int y = module(x);
    cout << "y=" << y <<endl;
}
