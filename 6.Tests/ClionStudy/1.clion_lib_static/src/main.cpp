#include <iostream>
#include "main.h"
#include "fun.h"

using namespace std;

int main() {
    cout << "version=" << VERSION <<endl;
    int x = 100;
    cout << "x=" << x <<endl;
    int y = fun1(x);
    cout << "y=" << y <<endl;

}
