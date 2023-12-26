#ifndef MY_RC_H_
#define MY_RC_H_

#include "main.h"
#include "usart.h"



class drv_rc
{
private:
    
    
public:
    
    uint16_t right_H;                        //遥控器右边控制杆水平模拟量  
    uint16_t right_V;                        //右边竖直
    uint16_t left_H;                         //左边水平
    uint16_t left_V;                         //左边竖直
    uint8_t s1;
    uint8_t s2;                             //1或3

    float speed_Index;                      //速度参数
    float Vx;                               //命令x（前进正方向）线速度 m/s
    float Vy;                               //命令y（左移正方向）线速度 m/s
    float yaw;                              //角速度 rad/s
    float angle;                            //云台角度
    drv_rc();
    ~drv_rc(){};
    void get_cmd();
    
};

void my_rc_Init();

extern drv_rc my_rc;

extern uint8_t rx_data[18];                    //接收到的原始2进制数据

#endif
