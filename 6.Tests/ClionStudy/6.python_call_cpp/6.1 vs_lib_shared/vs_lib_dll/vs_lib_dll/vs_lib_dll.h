// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 VSLIBDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// VSLIBDLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef VSLIBDLL_EXPORTS
#define VSLIBDLL_API  __declspec(dllexport)
#else
#define VSLIBDLL_API __declspec(dllimport)
#endif

// 此类是从 dll 导出的
class VSLIBDLL_API Cvslibdll {
public:
	Cvslibdll(void);
	// TODO: 在此处添加方法。
	int PID = 1001;
	int VID = 2002;

	int getid();
};

extern VSLIBDLL_API int nvslibdll;

VSLIBDLL_API int fnvslibdll(void);

extern "C"
{
	VSLIBDLL_API int fun1(int x);
	VSLIBDLL_API Cvslibdll* Cvslibdll_new();
	VSLIBDLL_API int my_getid(Cvslibdll* c);
}

