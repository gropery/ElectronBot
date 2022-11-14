#ifndef SERVODRIVE_FW_MOTOR_H
#define SERVODRIVE_FW_MOTOR_H

#include <cstdint>

class Motor
{
public:
    Motor()
    {}

    const float DCE_INTEGRAL_LIMIT = 500;

    struct DCE_t
    {
        float kp;
        float kv;
        float ki;
        float kd;
        float setPointPos;
        float setPointVel;
        float integralPos;
        float integralVel;
        float lastError;
        float output;
    };
    DCE_t dce;

    uint16_t adcValAtAngleMin;  //角度最小时对应的adc最小值 250
    uint16_t adcValAtAngleMax;  //角度最大时对应的adc最大值 3000
    float angle;
    float velocity;
    float mechanicalAngleMin;   //机械最小角度 0
    float mechanicalAngleMax;   //机械最大角度 180


    void UpdateVelocity();
    void SetEnable(bool _enable);
    void SetTorqueLimit(float _percent);
    float CalcDceOutput(float _inputPos, float _inputVel);
    void SetPwm(int16_t _pwm);

private:
    bool isEnabled;
    float lastAngle;
    float limitAngleMin;
    float limitAngleMax;
    float limitVelocity;
    float limitTorque;          // 转矩限制 0.5*1000
};

#endif
