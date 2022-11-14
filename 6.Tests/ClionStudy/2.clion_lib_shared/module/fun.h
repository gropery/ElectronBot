#ifndef CLION_DEMO_FUN_H
#define CLION_DEMO_FUN_H

#ifdef VSLIBDLL_EXPORTS
#define VSLIBDLL_API __declspec(dllexport)
#else
#define VSLIBDLL_API __declspec(dllimport)
#endif

VSLIBDLL_API int fun1(int x);

#endif //CLION_DEMO_FUN_H
