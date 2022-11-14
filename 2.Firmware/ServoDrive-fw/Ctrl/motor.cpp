#include "motor.h"
#include "tim.h"

// 输入角度位置和速度,计算得出应设置的pwm
float Motor::CalcDceOutput(float _inputPos, float _inputVel)
{
    float errorPos = _inputPos - dce.setPointPos;
    float errorVel = _inputVel - dce.setPointVel;
    float deltaPos = errorPos - dce.lastError;
    dce.lastError = errorPos;
    dce.integralPos += errorPos;
    if (dce.integralPos > DCE_INTEGRAL_LIMIT) dce.integralPos = DCE_INTEGRAL_LIMIT;
    else if (dce.integralPos < -DCE_INTEGRAL_LIMIT) dce.integralPos = -DCE_INTEGRAL_LIMIT;
    dce.integralVel += errorVel;
    if (dce.integralVel > DCE_INTEGRAL_LIMIT) dce.integralVel = DCE_INTEGRAL_LIMIT;
    else if (dce.integralVel < -DCE_INTEGRAL_LIMIT) dce.integralVel = -DCE_INTEGRAL_LIMIT;

    dce.output = dce.kp * errorPos +
                 dce.ki * dce.integralPos + dce.kv * dce.integralVel +
                 dce.kd * deltaPos;

    if (dce.output > limitTorque) dce.output = limitTorque;
    else if (dce.output < -limitTorque) dce.output = -limitTorque;

    return dce.output;
}

// PWM输出设置
void Motor::SetPwm(int16_t _pwm)
{
    if (isEnabled)
    {
        //_pwm==0 输出恒低; _pwm==1000输出恒高
        if (_pwm >= 0)
        {
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, _pwm > 1000 ? 1000 : _pwm);
        } else
        {
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, _pwm < -1000 ? 1000 : -_pwm);
        }
    } else
    {
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
    }
}

// 设置转矩的设置(输入参数 _percent 介于0-1之间)
void Motor::SetTorqueLimit(float _percent)
{
    if (_percent > 1)_percent = 1;
    else if (_percent < 0)_percent = 0;

    limitTorque = _percent * 1000;
}


void Motor::UpdateVelocity()
{
    velocity = angle - lastAngle;
    lastAngle = angle;
}

//设置是否开启舵机
void Motor::SetEnable(bool _enable)
{
    isEnabled = _enable;
}

