#ifndef SERIAL_PILOT_H_
#define SERIAL_PILOT_H_

#include "main.h"
#include "usart.h"
/*
还可以加更多
*/
void send_to_serialpilot(float,UART_HandleTypeDef);
void send_to_serialpilot_2(float,float,UART_HandleTypeDef);
// void send_to_serialpilot(float,float,float,UART_HandleTypeDef);
// void send_to_serialpilot(float,float,float,float,UART_HandleTypeDef);


#endif
