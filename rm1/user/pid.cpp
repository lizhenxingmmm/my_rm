#include "pid.h"


Macanum_wheels my_Macanum;
/*
构造函数
*/
Macanum_wheels::Macanum_wheels()
{
    wheel_r=0.2f;
    distance_AB=0.4f;
    distance_BC=0.3f;
    PI=3.14159265f;
    motorA_RS=0.0f;
    motorB_RS=0.0f;
    motorC_RS=0.0f;
    motorD_RS=0.0f;
}
Macanum_wheels::Macanum_wheels(float wheel_R,float dis_AB,float dis_AC){
    wheel_r=wheel_R;
    distance_AB=dis_AB;
    distance_BC=dis_AC;
    PI=3.14159265f;
    motorA_RS=0.0f;
    motorB_RS=0.0f;
    motorC_RS=0.0f;
    motorD_RS=0.0f;
}
/*
麦轮解算
*/
void Macanum_wheels::Macanum_solve(float Vx,float Vy,float yaw){                            //装有19:1减速箱的motor
    motorA_RS=Vx+Vy-yaw*(distance_AB+distance_BC)/2;
    motorB_RS=Vx-Vy-yaw*(distance_AB+distance_BC)/2;
    motorC_RS=Vx+Vy+yaw*(distance_AB+distance_BC)/2;
    motorD_RS=Vx-Vy+yaw*(distance_AB+distance_BC)/2;
    motorA_RS=(motorA_RS/wheel_r)*2370/PI;
    motorB_RS=(motorB_RS/wheel_r)*2370/PI;
    motorC_RS=(motorC_RS/wheel_r)*2370/PI;
    motorD_RS=(motorD_RS/wheel_r)*2370/PI;
}
/*
can中断接受数据
*/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
    if(hcan==&hcan1){
	CAN_RxHeaderTypeDef header;
    uint8_t data[8];
    HAL_CAN_GetRxMessage(hcan,CAN_FILTER_FIFO0,&header,data);
    if(header.StdId==0x201){
        my_motorA.receive(data);
    }
    if(header.StdId==0x202){
        my_motorD.receive(data);
    }
    if(header.StdId==0x203){
        my_motorB.receive(data);
    }
    if(header.StdId==0x204){
        my_motorC.receive(data);
    }
    if(header.StdId==0x205){
        my_motorY1.receive(data);
    }
    }
}
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan){
    if(hcan==&hcan1){
	CAN_RxHeaderTypeDef header;
    uint8_t data[8];
    HAL_CAN_GetRxMessage(hcan,CAN_FILTER_FIFO1,&header,data);
    if(header.StdId==0x201){
        my_motorA.receive(data);
    }
    if(header.StdId==0x202){
        my_motorD.receive(data);
    }
    if(header.StdId==0x203){
        my_motorC.receive(data);
    }
    if(header.StdId==0x204){
        my_motorB.receive(data);
    }
    if(header.StdId==0x205){
        my_motorY1.receive(data);
    }
    }
}

/*
motor类的构造函数
*/
 motor my_motorA;
 motor my_motorB;
 motor my_motorC;
 motor my_motorD;
 motor my_motorY1;
motor::motor()
{
    angle=0.0f;
    speed=0.0f;
    torque=0.0f;
    temperature=25.0f;
}
void motor::receive(uint8_t *data){
    int16_t _angle=(((int16_t)data[0])<<8)+data[1];
    int16_t _speed=(((int16_t)data[2])<<8)+data[3];
    int16_t _torque=(((int16_t)data[4])<<8)+data[5];
    int16_t _temperature=data[7];
    angle=(float)_angle*360/8191.0f;
    speed=(float)_speed;
    torque=(float)_torque;
    temperature=(float)_temperature;
}
/*
pid类构造函数，初始化参数
*/
pid::pid()
{
    last_error=0.0f;
    error=0.0f;
    P=6.0f;
    I=0.1f;
    D=1.0f;
    I_out_max=800.0f;                                                  
    P_out=0.0f;
    I_out=0.0f;
    D_out=0.0f;
    total_out=0.0f;
    total_out_max=7000.0f;                                             
}
pid::pid(float P,float I,float D,float I_max,float total_max){
    last_error=0.0f;
    error=0.0f;
    this->P=P;
    this->I=I;
    this->D=D;
    I_out_max=I_max;                                                  
    P_out=0.0f;
    I_out=0.0f;
    D_out=0.0f;
    total_out=0.0f;
    total_out_max=total_max;  
}
float pid::pid_calcu(float target,float feedback){
    
    error=target-feedback;
    P_out=P*error;
    D_out=D*(error-last_error);
    if(I_out<I_out_max || I_out>-I_out_max){
        I_out+=I*error;
    }
    else{
        I_out=0.0f;
    }
    total_out=P_out+I_out+D_out;
    float final_out=(total_out<total_out_max)?total_out:total_out_max;
    last_error=error;
    return final_out;
   
}



