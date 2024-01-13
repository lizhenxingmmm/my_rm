/*
(0)前提:
配置:使能uart通信
使能DMA,中断
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
*/
#ifndef SERIALPLOT_HPP
#define SERIALPLOT_HPP

#include "usart.h"

void send_to_serialplot_1(float num,UART_HandleTypeDef huart_);
void send_to_serialplot_2(float num1,float num2,UART_HandleTypeDef huart_);


#endif
