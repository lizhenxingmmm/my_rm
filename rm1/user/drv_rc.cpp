/*
遥控器数据及其实际位姿，速度命令
*/
#include "drv_rc.h"


uint8_t rx_data[18];                    //接收到的原始2进制数据
void my_rc_Init(){
    for(int i=0;i<19;i++){
        rx_data[i]=0x00;
    }
    HAL_UART_Receive_DMA(&huart3,rx_data,18);
}

drv_rc my_rc;                                               //创建一个全局对象my_rc

drv_rc::drv_rc(){
    
    right_H=0;
    right_V=0;
    left_H=0;
    left_V=0;
    speed_Index=0.001f;
    Vx=0.0f;
    Vy=0.0f;
    yaw=0.0f;
    angle=180.0f;
    s1=2;
    s2=2;
}

/*
my_rc内部接收数据
*/
void drv_rc::get_cmd(){
    
    right_H=((int16_t)rx_data[0] | ((int16_t)rx_data[1]<<8))&0x07ff;
    right_V=(((int16_t)rx_data[1] >> 3) | ((int16_t)rx_data[2] << 5)) & 0x07FF;
    left_H=(((int16_t)rx_data[2] >> 6) | ((int16_t)rx_data[3] << 2) | ((int16_t)rx_data[4] << 10)) & 0x07FF;
    left_V=(((int16_t)rx_data[4] >> 1) | ((int16_t)rx_data[5]<<7)) & 0x07FF;
    s1=((rx_data[5]>>4)&0x000c)>>2;
    s2=((rx_data[5]>>4)&0x0003);
    
    Vx=((float)my_rc.left_V-1024.0f)*speed_Index;
    Vy=(1024.0f-(float)my_rc.left_H)*speed_Index;
    //angle=((float)my_rc.right_H-1024.0f)/3.666666f+180.0f;
    angle=((float)my_rc.right_V-1024.0f)/10;
    yaw=((float)right_H-1024.0f)/330;
    //yaw=1;
}
