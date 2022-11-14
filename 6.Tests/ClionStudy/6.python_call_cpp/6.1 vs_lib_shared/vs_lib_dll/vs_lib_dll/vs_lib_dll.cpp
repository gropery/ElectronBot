// vs_lib_dll.cpp : 定义 DLL 的导出函数。
//


#include "vs_lib_dll.h"


// 这是导出变量的一个示例
VSLIBDLL_API int nvslibdll=0;

// 这是导出函数的一个示例。
VSLIBDLL_API int fnvslibdll(void)
{
    return 0;
}

// 这是已导出类的构造函数。
Cvslibdll::Cvslibdll()
{
    return;
}

int Cvslibdll::getid()
{
    return PID + VID;
}

/******************************/
int fun1(int x)
{
    return x + 1;
}

Cvslibdll* Cvslibdll_new() {
    return new Cvslibdll;
}

int my_getid(Cvslibdll* c) {
    return c->getid();
}
