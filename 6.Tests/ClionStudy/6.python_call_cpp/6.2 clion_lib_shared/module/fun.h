#define API __declspec(dllexport)

class Test{
private:
    double _calculate(int a, double b);
public:
    int PID = 1001;
    int VID = 2002;

    double calculate(int a, double b, char c[], int * d, double * e, char ** f);
    int getid();
};

// 封装C接口
extern "C"{

// 导出函数
API Test* test_new();
API double my_calculate(Test* t, int a, double b, char c[], int * d, double * e, char ** f);
API int my_getid(Test* t);

}