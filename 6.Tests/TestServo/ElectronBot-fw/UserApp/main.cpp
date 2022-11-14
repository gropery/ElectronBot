#include <cmath>
#include "common_inc.h"
#include "screen.h"
#include "robot.h"


Robot electron(&hspi1, &hi2c1);
float jointSetPoints[6];
bool isEnabled = false;
/* Thread Definitions -----------------------------------------------------*/


/* Timer Callbacks -------------------------------------------------------*/


/* Default Entry -------------------------------------------------------*/
void Main(void)
{
    HAL_Delay(2000);
    electron.lcd->Init(Screen::DEGREE_0);
    electron.lcd->SetWindow(0, 239, 0, 239);

#if 0
    // 0.先只链接一个舵机,不设置地址,测试硬件和舵机固件是否OK

    // 1.确保广播Joint的变量正确,直接更新 UpdateJointSAngle
    //  可能会因为角度不在变量范围内不发送指令.(详细阅读代码)
    electron.joint[0].id=0;
    electron.joint[0].angleMin = 0;
    electron.joint[0].angleMax = 180;
    electron.joint[0].angle = 0;
    electron.joint[0].modelAngelMin = -90;
    electron.joint[0].modelAngelMax = 90;
    electron.joint[0].inverted = false;

    // 2.使用广播地址使能
    electron.SetJointEnable(electron.joint[0], true);

    // 3.此时能看到舵机做往复运动
    while (true)
    {
        for (int i = -90; i < 90; ++i) {
            float angle = i;
            electron.UpdateJointAngle(electron.joint[0], angle);
            HAL_Delay(20);
        }
        for (int i = 90; i > -90; --i) {
            float angle = i;
            electron.UpdateJointAngle(electron.joint[0], angle);
            HAL_Delay(20);
        }
    }
#endif

#if 0
    // 0.当能够正常用广播地址控制单个舵机后,现在进行舵机的ID设置
    // 还是只链接一个舵机

    // 1.比如需要把这个舵机ID设置为2

    // 2.确保广播Joint的变量正确,直接更新 UpdateJointSAngle
    //  可能会因为角度不在变量范围内不发送指令.(详细阅读代码)
    electron.joint[0].id=0;                 //这里还是用广播
    electron.joint[0].angleMin = 0;
    electron.joint[0].angleMax = 180;
    electron.joint[0].angle = 0;
    electron.joint[0].modelAngelMin = -90;
    electron.joint[0].modelAngelMax = 90;
    electron.joint[0].inverted = false;

    electron.SetJointId(electron.joint[0], 2);

    HAL_Delay(1000);
    // 3.等待舵机参数保存.将主控joint[0].id改为2
    electron.joint[0].id=2;                 //这里改为新地址
    electron.joint[0].angleMin = 0;
    electron.joint[0].angleMax = 180;
    electron.joint[0].angle = 0;
    electron.joint[0].modelAngelMin = -90;
    electron.joint[0].modelAngelMax = 90;
    electron.joint[0].inverted = false;

    // 4.使用新设置的2地址通讯
    electron.SetJointEnable(electron.joint[0], true);

    while (true)
    {
        for (int i = -90; i < 90; ++i) {
            float angle = i;
            electron.UpdateJointAngle(electron.joint[0], angle);
            HAL_Delay(20);
        }
        for (int i = 90; i > -90; --i) {
            float angle = i;
            electron.UpdateJointAngle(electron.joint[0], angle);
            HAL_Delay(20);
        }
    }
    // 5.之后的程序就不需要步骤2中的设置地址了
    // 只需要用2地址,就能通讯上这个舵机,其他的舵机也是一样
#endif

#if 1
    // 0.链接6个所有舵机,测试硬件和舵机固件是否OK
    electron.joint[0].angleMin = 0;
    electron.joint[0].angleMax = 180;
    electron.joint[0].angle = 0;
    electron.joint[0].modelAngelMin = -90;
    electron.joint[0].modelAngelMax = 90;
    electron.joint[0].inverted = false;

    // 2.使用广播地址使能
    electron.joint[0].id=2;
    electron.SetJointEnable(electron.joint[0], true);
    electron.joint[0].id=4;
    electron.SetJointEnable(electron.joint[0], true);
    electron.joint[0].id=6;
    electron.SetJointEnable(electron.joint[0], true);
    electron.joint[0].id=8;
    electron.SetJointEnable(electron.joint[0], true);
    electron.joint[0].id=10;
    electron.SetJointEnable(electron.joint[0], true);
    electron.joint[0].id=12;
    electron.SetJointEnable(electron.joint[0], true);

    // 3.此时能看到所有舵机做往复运动
    while (true)
    {

        for (int i = -90; i < 90; ++i) {
            float angle = i;
            electron.joint[0].id=2;
            electron.UpdateJointAngle(electron.joint[0], angle);
            electron.joint[0].id=4;
            electron.UpdateJointAngle(electron.joint[0], angle);
            electron.joint[0].id=6;
            electron.UpdateJointAngle(electron.joint[0], angle);
            electron.joint[0].id=8;
            electron.UpdateJointAngle(electron.joint[0], angle);
            electron.joint[0].id=10;
            electron.UpdateJointAngle(electron.joint[0], angle);
            electron.joint[0].id=12;
            electron.UpdateJointAngle(electron.joint[0], angle);
            HAL_Delay(20);
        }
        for (int i = 90; i > -90; --i) {
            float angle = i;
            electron.joint[0].id=2;
            electron.UpdateJointAngle(electron.joint[0], angle);
            electron.joint[0].id=4;
            electron.UpdateJointAngle(electron.joint[0], angle);
            electron.joint[0].id=6;
            electron.UpdateJointAngle(electron.joint[0], angle);
            electron.joint[0].id=8;
            electron.UpdateJointAngle(electron.joint[0], angle);
            electron.joint[0].id=10;
            electron.UpdateJointAngle(electron.joint[0], angle);
            electron.joint[0].id=12;
            electron.UpdateJointAngle(electron.joint[0], angle);
            HAL_Delay(20);
        }

    }
#endif

}


extern "C"
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi)
{
    electron.lcd->isBusy = false;
}