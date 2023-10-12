#include <opencv2/imgproc/types_c.h>
#include "electron_low_level.h"
#include "USBInterface.h"


/**
 * 开启独立线程运行SyncTask
 * @return
 */
bool ElectronLowLevel::Sync()
{
    if (isConnected)
    {
        if (syncTaskHandle.joinable()) //等待线程syncTaskHandle完成
            syncTaskHandle.join();

        syncTaskHandle = std::thread(SyncTask, this);

        timeStamp++;
        return true;
    }

    return false;
}

/**
 * 从opencv中获取图像数据,resize成屏幕大小,转换RGB格式,填充至图像待发送buffer
 * @param _mat
 */
void ElectronLowLevel::SetImageSrc(const cv::Mat &_mat)
{
    cv::Mat temp;
    resize(_mat, temp, cv::Size(240, 240));
    cvtColor(temp, temp, CV_BGRA2RGB);
    std::memcpy(frameBufferTx[pingPongWriteIndex], temp.data, 240 * 240 * 3);
}


void ElectronLowLevel::SetImageSrc(const string &_filePath)
{
    cv::Mat temp = cv::imread(_filePath);
    resize(temp, temp, cv::Size(240, 240));
    cvtColor(temp, temp, CV_BGRA2RGB);
    std::memcpy(frameBufferTx[pingPongWriteIndex], temp.data, 240 * 240 * 3);
}


void ElectronLowLevel::SetExtraData(uint8_t* _data, uint32_t _len)
{
    if (_len <= 32)
        memcpy(extraDataBufferTx[pingPongWriteIndex], _data, _len);
}


uint8_t* ElectronLowLevel::GetExtraData(uint8_t* _data)
{
    if (_data != nullptr)
        memcpy(_data, extraDataBufferRx, 32);

    return extraDataBufferRx;
}

/**
 * 从下位机接收数据包
 * @param _buffer :数据缓存
 * @param _packetCount :待接收数据包个数
 * @param _packetSize :数据包大小
 * @return 1:成功 0:失败
 */
bool ElectronLowLevel::ReceivePacket(uint8_t* _buffer, uint32_t _packetCount, uint32_t _packetSize)
{
    uint32_t packetCount = _packetCount;
    uint32_t ret;
    do
    {
        do
        {
            ret = USB_BulkReceive(0, EP1_IN, reinterpret_cast<char*>(_buffer), _packetSize, 100);
        } while (ret != _packetSize);

        packetCount--;
    } while (packetCount > 0);

    return packetCount == 0;
}

/**
 * 发送数据包至下位机
 * @param _buffer :数据缓存
 * @param _packetCount :待发送数据包个数
 * @param _packetSize :数据包大小
 * @return 1:成功 0:失败
 */
bool ElectronLowLevel::TransmitPacket(uint8_t* _buffer, uint32_t _packetCount, uint32_t _packetSize)
{
    uint32_t packetCount = _packetCount;
    uint32_t dataOffset = 0;
    uint32_t ret;
    do
    {
        do
        {
            ret = USB_BulkTransmit(0, EP1_OUT,
                                   reinterpret_cast<char*>(_buffer) + dataOffset,
                                   _packetSize, 100);
        } while (!ret);

        dataOffset += _packetSize;
        packetCount--;
    } while (packetCount > 0);

    return packetCount == 0;
}

/**
 * 连接USB设备
 * @return ture:成功连接 false:连接失败
 */
bool ElectronLowLevel::Connect()
{
    int devNum = USB_ScanDevice(USB_PID, USB_VID);

    if (devNum > 0)
    {
        if (USB_OpenDevice(0))
        {
            isConnected = true;
            timeStamp = 0;

            return true;
        }
    }

    return false;
}

/**
 * 断开连接usb设备
 * @return ture:断开成功 false:断开失败
 */
bool ElectronLowLevel::Disconnect()
{
    // 等待进程执行完毕
    if (syncTaskHandle.joinable())
        syncTaskHandle.join();

    if (isConnected && USB_CloseDevice(0))
    {
        isConnected = false;
        return true;
    }

    return false;
}

/**
 * 独立线程中运行以下程序
 * 1.获取当前应操作的的pingpong缓存
 * 2.每帧图像分4 p传输,每p发送 1/4帧数据(60*240*3) + 6个舵机角度值(32B) = 43200 + 32 = 43232 Bytes
 * 3.每p中首先读取下位机发送过来的32Byte舵机角度数据,存储于extraDataBufferRx
 * 4.然后从frameBufferTx中获取43008Bytes图像数据,发送84个512Byte的包发送至下位机
 * 5.然后从frameBufferTx中获取1/4帧图像剩下的192Bytes(43200-43008),再拼接舵机的32B,组成224B,发送1次224Bytes的包至下位机
 * 6.轮询步骤2-5,直至1帧图像发送完毕
 * @param _obj
 */
void ElectronLowLevel::SyncTask(ElectronLowLevel* _obj)
{
    uint32_t frameBufferOffset = 0;
    uint8_t index = _obj->pingPongWriteIndex;
    _obj->pingPongWriteIndex = _obj->pingPongWriteIndex == 0 ? 1 : 0;
    for (int p = 0; p < 4; p++)
    {
        // Wait MCU request & receive 32bytes extra data
        _obj->ReceivePacket(reinterpret_cast<uint8_t*>(_obj->extraDataBufferRx),
                            1, 32);

        // Transmit buffer
        _obj->TransmitPacket(reinterpret_cast<uint8_t*>(_obj->frameBufferTx[index]) + frameBufferOffset,
                             84, 512);
        frameBufferOffset += 43008;

        // Fill frame tail & extra data
        memcpy(_obj->usbBuffer200, reinterpret_cast<uint8_t*>(_obj->frameBufferTx[index]) + frameBufferOffset,
               192);
        memcpy(_obj->usbBuffer200 + 192, reinterpret_cast<uint8_t*>(_obj->extraDataBufferTx[index]), 32);

        // Transmit frame tail & extra data
        _obj->TransmitPacket(_obj->usbBuffer200, 1, 224);
        frameBufferOffset += 192;
    }
}

/**
 *  设置6个舵机的模型角度float值,每个float转换成4B unsigned char类型,缓存于extraDataBufferTx
 * @param _j1
 * @param _j2
 * @param _j3
 * @param _j4
 * @param _j5
 * @param _j6
 * @param _enable
 */
void ElectronLowLevel::SetJointAngles(float _j1, float _j2, float _j3, float _j4, float _j5, float _j6,
                                      bool _enable)
{
    float jointAngleSetPoints[6];

    jointAngleSetPoints[0] = _j1;
    jointAngleSetPoints[1] = _j2;
    jointAngleSetPoints[2] = _j3;
    jointAngleSetPoints[3] = _j4;
    jointAngleSetPoints[4] = _j5;
    jointAngleSetPoints[5] = _j6;

    extraDataBufferTx[pingPongWriteIndex][0] = _enable ? 1 : 0;
    for (int j = 0; j < 6; j++)
        for (int i = 0; i < 4; i++)
        {
            auto* b = (unsigned char*) &(jointAngleSetPoints[j]);
            extraDataBufferTx[pingPongWriteIndex][j * 4 + i + 1] = *(b + i);
        }
}


/**
 * 从extraDataBufferRx中转换得到6个舵机的float值
 * @param _jointAngles 返回的6个舵机角度值
 */
void ElectronLowLevel::GetJointAngles(float* _jointAngles)
{
    for (int j = 0; j < 6; j++)
    {
        _jointAngles[j] = *((float*) (extraDataBufferRx + 4 * j + 1));
    }
}

