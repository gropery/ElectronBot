#include <cstdint>
#include <iostream>

using namespace std;

#define API __declspec(dllexport)

class Obj
{
    int m_x;
public:
    Obj(int x): m_x(x) {}
    int get_x() const { return m_x; }
};

extern "C" {

API Obj* CreateObj() {
    return new Obj(5);
}

API int DoSomething(Obj* ptr, uint8_t t[][3][3]) {
    for(uint8_t i=0; i < 3; ++i) {
        for(uint8_t j=0; j < 3; ++j) {
            for(uint8_t k=0; k < 3; ++k) {
                cout << static_cast<uint32_t>(t[i][j][k]) << ' ';
                t[i][j][k] *= 2;
            }
            cout << endl;
        }
        cout << endl;
    }
    return ptr->get_x();
}

}