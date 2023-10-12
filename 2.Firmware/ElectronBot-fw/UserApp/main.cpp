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

    // 初次上电,设置舵机位置至模型位置0点,并获得舵机机械角度值,并转换为模型位置值,存储于electron.joint[].angle
    electron.UpdateJointAngle(electron.joint[1], 0);
    electron.UpdateJointAngle(electron.joint[2], 0);
    electron.UpdateJointAngle(electron.joint[3], 0);
    electron.UpdateJointAngle(electron.joint[4], 0);
    electron.UpdateJointAngle(electron.joint[5], 0);
    electron.UpdateJointAngle(electron.joint[6], 0);

    float t = 0;

    while (true)
    {
#if 1
        // 一帧图像经过4次传输,每次传输60*240*3,然后更新lcd显示
        // 因为此MCU只有128KB的RAM,加上pingpong操作,因此需要4次才能传输1帧完整图像
        for (int p = 0; p < 4; p++)
        {
            // 通过USB发送6个舵机模型角度值,每个舵机角度由4Byte组成,有效数据总共为24B,填充至32B发送
            for (int j = 0; j < 6; j++)
                for (int i = 0; i < 4; i++)
                {
                    auto* b = (unsigned char*) &(electron.joint[j + 1].angle);
                    electron.usbBuffer.extraDataTx[j * 4 + i + 1] = *(b + i);
                }
            electron.SendUsbPacket(electron.usbBuffer.extraDataTx, 32);

            // 通过USB单次接收60*240*3+32字节数据,USB单包接收数据为512字节,1/4帧显示图像的数据为 60*240*3字节
            // 所以最后一次USB接收数据字节 = (60*240*3+32) - 512*84 = 224 Bytes
            // 以下语句表示持续等待,直到接收完整的1/4帧数据,及32B的额外6个舵机角度值数据
            electron.ReceiveUsbPacketUntilSizeIs(224); // last packet is 224bytes

            // 解析PC下发的数据,如果ExtraData[0]=1,则使能各个舵机
            uint8_t* ptr = electron.GetExtraDataRxPtr();
            if (isEnabled != (bool) ptr[0])
            {
                isEnabled = ptr[0];
                electron.SetJointEnable(electron.joint[1], isEnabled);
                electron.SetJointEnable(electron.joint[2], isEnabled);
                electron.SetJointEnable(electron.joint[3], isEnabled);
                electron.SetJointEnable(electron.joint[4], isEnabled);
                electron.SetJointEnable(electron.joint[5], isEnabled);
                electron.SetJointEnable(electron.joint[6], isEnabled);
            }
            // 解析PC下发的数据,存储6个舵机模型角度值到jointSetPoints[]数组
            for (int j = 0; j < 6; j++)
            {
                jointSetPoints[j] = *((float*) (ptr + 4 * j + 1));
            }

            // 解析PC下发的数据,液晶显示
            while (electron.lcd->isBusy);
            if (p == 0)
                // 写入第一个1/4帧数据,写LCD命令非append
                electron.lcd->WriteFrameBuffer(electron.GetLcdBufferPtr(),
                                               60 * 240 * 3);
            else
                // 写入后个3/4帧数据,写LCD命令需要append
                electron.lcd->WriteFrameBuffer(electron.GetLcdBufferPtr(),
                                               60 * 240 * 3, true);
        }
        HAL_Delay(1);
#endif


        t += 0.01;

        // 设置舵机位置为PC下发的解析出的舵机角度,然后读取舵机当前机械角度值,并转换为模型位置值,存储于electron.joint[].angle
        electron.UpdateJointAngle(electron.joint[1], jointSetPoints[0]);
        electron.UpdateJointAngle(electron.joint[2], jointSetPoints[1]);
        electron.UpdateJointAngle(electron.joint[3], jointSetPoints[2]);
        electron.UpdateJointAngle(electron.joint[4], jointSetPoints[3]);
        electron.UpdateJointAngle(electron.joint[5], jointSetPoints[4]);
        electron.UpdateJointAngle(electron.joint[6], jointSetPoints[5]);

        HAL_Delay(1);

//      electron.UpdateJointAngle(electron.joint[ANY], 65 + 75 * std::sin(t));

        printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
               jointSetPoints[0], jointSetPoints[1], jointSetPoints[2],
               jointSetPoints[3], jointSetPoints[4], jointSetPoints[5]);
    }
}

extern "C"
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi)
{
    electron.lcd->isBusy = false;
}