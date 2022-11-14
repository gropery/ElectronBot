#ifndef ELECTRONBOTSDK_ELECTRONLOWLEVEL_H
#define ELECTRONBOTSDK_ELECTRONLOWLEVEL_H

#include <iostream>
#include <cstdio>
#include <thread>

#ifdef VSLIBDLL_EXPORTS
#define VSLIBDLL_API  __declspec(dllexport)
#else
#define VSLIBDLL_API __declspec(dllimport)
#endif

class VSLIBDLL_API ElectronLowLevel
{
public:
    ElectronLowLevel()
    = default;

    ElectronLowLevel(int _vid, int _pid) :
        USB_PID(_pid), USB_VID(_vid)
    {}

    bool Connect();
    bool Disconnect();
    bool Sync();
    void SetImageSrc(const uint8_t* _data);
//    void SetImageSrc(const std::string &_filePath);
    void SetExtraData(uint8_t* _data, uint32_t _len = 32);
    void SetJointAngles(float _j1, float _j2, float _j3, float _j4, float _j5, float _j6,
                        bool _enable = false);
    void GetJointAngles(float* _jointAngles);
    uint8_t* GetExtraData(uint8_t* _data = nullptr);

    int USB_VID = 0x1001;
    int USB_PID = 0x8023;
    bool isConnected = false;
    uint32_t timeStamp = 0;


private:
    uint8_t pingPongWriteIndex = 0;
    uint8_t usbBuffer200[200]{};
    uint8_t frameBufferTx[2][240 * 240 * 3]{};
    uint8_t extraDataBufferTx[2][32]{};
    uint8_t extraDataBufferRx[32]{};
    std::thread syncTaskHandle;

    static bool ReceivePacket(uint8_t* _buffer, uint32_t _packetCount, uint32_t _packetSize);
    static bool TransmitPacket(uint8_t* _buffer, uint32_t _packetCount, uint32_t _packetSize);
    static void SyncTask(ElectronLowLevel* _obj);
};


extern "C" {

VSLIBDLL_API ElectronLowLevel* CreateElectronLowLevel() {
    return new ElectronLowLevel;
}

VSLIBDLL_API int myConnect(ElectronLowLevel* e){
    return e->Connect();
}

VSLIBDLL_API int myDisconnect(ElectronLowLevel* e){
    return e->Disconnect();
}

VSLIBDLL_API int mySync(ElectronLowLevel* e){
    return e->Sync();
}

VSLIBDLL_API void mySetImageSrc(ElectronLowLevel* e, uint8_t arr[][240][3]){
    uint8_t *_data;
    _data = (uint8_t *) (void*) arr;

//    uint8_t _data[240*240*3];
//    uint32_t cnt=0;
//    for (int i = 0; i < 240; ++i) {
//        for (int j = 0; j < 240; ++j) {
//            for (int k = 0; k < 3; ++k) {
//                _data[cnt++] = arr[i][j][k];
//            }
//        }
//    }

    e->SetImageSrc(_data);
}

VSLIBDLL_API void mySetJointAngles(ElectronLowLevel* e, float *_jointAngles, int _enable){
    e->SetJointAngles(_jointAngles[0], _jointAngles[1], _jointAngles[2],
                      _jointAngles[3], _jointAngles[4], _jointAngles[5], (bool)_enable);
}

float _jointAngles[6];
VSLIBDLL_API float* myGetJointAngles(ElectronLowLevel* e){
    e->GetJointAngles(_jointAngles);
    return _jointAngles;
}

}

#endif
