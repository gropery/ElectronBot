#include <cmath>
#include "common_inc.h"
#include "screen.h"
#include "robot.h"


Robot electron(&hspi1, &hi2c1);
/* Thread Definitions -----------------------------------------------------*/


/* Timer Callbacks -------------------------------------------------------*/


/* Default Entry -------------------------------------------------------*/
void Main(void)
{
    HAL_Delay(2000);
    electron.lcd->Init(Screen::DEGREE_0);
    electron.lcd->SetWindow(0, 239, 0, 239);

    for (int i = 0; i < 60*240*3; ++i) {
        if(i%3==0) //R
            electron.usbBuffer.rxData[0][i] = float(i)/(60*240*3)*255;
        else
            electron.usbBuffer.rxData[0][i] = 0;
    }
    electron.lcd->WriteFrameBuffer(electron.usbBuffer.rxData[0],
                                   60 * 240 * 3);

    for (int i = 1; i < 60*240*3; ++i) {
        if(i%3==1) //G
            electron.usbBuffer.rxData[0][i] = float(i)/(60*240*3)*255;
        else
            electron.usbBuffer.rxData[0][i] = 0;
    }
    electron.lcd->WriteFrameBuffer(electron.usbBuffer.rxData[0],
                                   60 * 240 * 3, true);

    for (int i = 2; i < 60*240*3; ++i) {
        if(i%3==2) //B
            electron.usbBuffer.rxData[0][i] = float(i)/(60*240*3)*255;
        else
            electron.usbBuffer.rxData[0][i] = 0;
    }
    electron.lcd->WriteFrameBuffer(electron.usbBuffer.rxData[0],
                                   60 * 240 * 3, true);

    for (int i = 0; i < 60*240*3; ++i) {
        electron.usbBuffer.rxData[0][i] = 255;
    }
    electron.lcd->WriteFrameBuffer(electron.usbBuffer.rxData[0],
                                   60 * 240 * 3, true);

    while (true)
    {
        // 一帧图像经过4次传输,每次传输60*240*3,然后更新lcd显示
        // 因为此MCU只有128KB的RAM,加上pingpong操作,因此需要4次才能传输1帧完整图像
        for (int p = 0; p < 4; p++)
        {
            electron.SendUsbPacket(electron.usbBuffer.extraDataTx, 32);
            // USB单次接收数据512字节, 1/4帧图像的数据为 60*240*3字节,
            // 所以最后一次USB接收数据字节 = (60*240*3+32) - 512*84 = 224 Bytes
            // 以下语句表示持续等待,直到接收完整的1/4帧数据
            electron.ReceiveUsbPacketUntilSizeIs(224); // last packet is 224bytes

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
    }
}


extern "C"
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi)
{
    electron.lcd->isBusy = false;
}