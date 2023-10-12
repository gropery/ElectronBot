#include <iostream>
#include <vector>
#include "fun.h"

double Test::_calculate(int a, double b){
    double res = a+b;
    std::cout<<"res: "<<res<<std::endl;
    return res;
}

double Test::calculate(int a, double b, char c[], int * d, double * e, char ** f){
    std::cout<<"a: "<<a<<std::endl;
    std::cout<<"b: "<<b<<std::endl;
    std::cout<<"c: "<<c<<std::endl;
    std::cout<<"d: "<<d[0]<<d[1]<<std::endl;
    std::cout<<"e: "<<e[0]<<e[1]<<std::endl;
    std::cout<<"f: "<<f[0]<<f[1]<<std::endl;
    return this->_calculate(a, b);
}

int Test::getid()
{
    return PID+VID;
}

// 创建对象
Test* test_new(){
    return new Test;
}
double my_calculate(Test* t, int a, double b, char c[], int * d, double * e, char ** f){
    return t->calculate(a, b,c,d,e,f);
}
int my_getid(Test* t)
{
    return t->getid();
}

