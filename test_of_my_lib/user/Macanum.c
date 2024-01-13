#include "Macanum.h"

float Maca_data[4];
/*
依次输入目标速度，x，y，yaw，转动速度参数，总速度参数(所有参数均无单位，通过调参达到预期速度)
            x             wheel B         C
            |                                 
            |
            |
y___________|                   A         D
坐标如图，x为前进方向
返回4位float型数组指针，依次为A,B,C,D轮转速
*/
float* Macanum_solve(float target_x,float target_y,float target_yaw,float index_yaw,float index_total){
    Maca_data[0]=(target_x+target_y-target_yaw*index_yaw)*index_total;
    Maca_data[1]=(target_x-target_y-target_yaw*index_yaw)*index_total;
    Maca_data[2]=(target_x+target_y+target_yaw*index_yaw)*index_total;
    Maca_data[3]=(target_x-target_y+target_yaw*index_yaw)*index_total;
    return Maca_data;
}
