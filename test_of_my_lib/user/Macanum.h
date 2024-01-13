/*
使用说明:
(0) 前提:无
(1) Macanum_solve(float target_x,float target_y,float target_yaw,float index_yaw,float index_total);
麦轮解算，无标准物理单位，不需要测算轮距，通过调参实现目标效果
依次输入目标速度，x，y，yaw，转动速度参数，总速度参数(所有参数均无单位，通过调参达到预期速度)
            x             wheel B         C
            |                                 
            |
            |
y___________|                   A         D
坐标如图，x为前进方向
返回4位float型数组指针，依次为A,B,C,D轮转速
*/
#ifndef MACANUM_H
#define MACANUM_H

float* Macanum_solve(float target_x,float target_y,float target_yaw,float index_yaw,float index_total);

#endif
