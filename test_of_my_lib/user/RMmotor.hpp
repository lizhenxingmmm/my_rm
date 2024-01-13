/*
(0)前提:
依赖drv_can 在maincpp.cpp中调用
(1)send_to_motors(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4,uint16_t lable,CAN_HandleTypeDef* hcan_);
发送can报文给4个电机，lable是报文ID
(2)创建RMmotor对象(在maincpp.cpp中全局创建)
RMmotor motor(0x1ff,0x205);//例子
(3)motor.receive_data(uint16_t receivedID,uint8_t* data);
收数据
//例子
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan){
	CAN_RxHeaderTypeDef header;
	uint8_t data[8]; //用于接收数据(不知道为什么，这里不能是uint8_t* data;)
	HAL_CAN_GetRxMessage(hcan,CAN_FILTER_FIFO1,&header,data);
    motor.receive_data(header.StdId,data);
}
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
	CAN_RxHeaderTypeDef header;
	uint8_t data[8]; //用于接收数据(不知道为什么，这里不能是uint8_t* data;)
	HAL_CAN_GetRxMessage(hcan,CAN_FILTER_FIFO0,&header,data);
    motor.receive_data(header.StdId,data);
}

!!!注意！！！
GM6020电压控制——负载扭矩一定电机转速与电机电压成正比 n=C1*U-C2*T (U为电机电压,T为负载扭矩)
M3508和M2006——电机扭矩和电机电流成正比

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
        uint16_t lable;
        uint16_t ID;
        RMmotor(uint16_t lable,uint16_t ID){
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
        void receive_data(uint16_t receivedID,uint8_t* data);
};

void send_to_motors(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4,uint16_t lable,CAN_HandleTypeDef* hcan_);

#endif 
