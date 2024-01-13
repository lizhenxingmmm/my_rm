/*
(0)前提:
依赖drv_can
*/
#ifndef RMMOTOR_HPP
#define RMMOTOR_HPP

#include "drv_can.h"

class RMmotor{
    public:
        float angle;
        float speed;
        float torque;
        float temperature;
        float pid_error_speed;
        float pid_error_angle;
        float pid_last_error_speed;
        float pid_last_error_angle;
        float P_out;
        float I_out;
        float D_out;
        uint8_t lable;
        uint8_t ID;
        RMmotor(uint8_t lable,uint8_t ID){
        angle=0;
        speed=0;
        torque=0;
        temperature=0;
        pid_error_speed=0;
        pid_error_angle=0;
        pid_last_error_speed=0;
        pid_last_error_angle=0;
        P_out=0;
        I_out=0;
        D_out=0;
        this->lable=lable;
        this->ID=ID;
        }
        float pid_calcu_speed(float target,float feedback,float P,float I,float D,float i_max,float total_max,int mode);
        float pid_calcu_angle(float target,float feedback,float P,float I,float D,float i_max,float total_max,int mode);
        void receive_data(uint8_t receivedID,uint8_t* data);
};

void send_to_motors(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4,uint8_t lable,CAN_HandleTypeDef* hcan_);

#endif 
