//
// Created by Administrator on 2022/10/21/021.
//

#ifndef CLION_DEMO_MODULE_H
#define CLION_DEMO_MODULE_H

#ifdef VSLIBDLL_EXPORTS
#define VSLIBDLL_API __declspec(dllexport)
#else
#define VSLIBDLL_API __declspec(dllimport)
#endif

VSLIBDLL_API int module(int x);

#endif //CLION_DEMO_MODULE_H
