#ifndef PID_H_
#define PID_H_

#include "drv_can.h"
#include "serial_pilot.h"
#include "math.h"

//麦轮解算部分
class Macanum_wheels
{
private:
    
public:
    float wheel_r;                                      //轮半径
    float distance_AB;                                  //麦轮AB轮距
    float distance_BC;
    float PI;                                           //圆周率

    float motorA_RS;                                    //单位RPM
    float motorB_RS;
    float motorC_RS;
    float motorD_RS;
    Macanum_wheels();
    Macanum_wheels(float wheel_R,float dis_AB,float dis_AC);
    ~Macanum_wheels(){};
    void Macanum_solve(float Vx,float Vy,float yaw);
};

extern Macanum_wheels my_Macanum_num5;  //

//电机数据部分
class motor
{
private:
    
public:
    float angle;                                    //0~360°
    float speed;                                    //RPM
    float torque;                                   //实际是电流，单位应该是mA
    float temperature;                              //°C
    motor();
    ~motor(){};
    void receive(uint8_t *data);
};
extern motor my_motorA;
extern motor my_motorB;
extern motor my_motorC;
extern motor my_motorD;
extern motor my_motorY1;

class pid
{
private:
    float last_error;
    float error;
    float P;
    float I;
    float D;
    float I_out_max;
    float P_out;
    float I_out;
    float D_out;
    float total_out;
    float total_out_max;
public:
    pid();
    ~pid(){};
    pid(float P,float I,float D,float I_max,float total_max);
    float pid_calcu(float target,float feedback);                   //target,feedback单位PRM  返回输出扭矩
};



#endif
