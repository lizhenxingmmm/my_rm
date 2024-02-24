#ifndef DRV_UART_H
#define DRV_UART_H

#include "usart.h"

void uart_send_data_float(UART_HandleTypeDef* huart_,float data[],uint16_t size,uint8_t lable);
void UART_IT_Init(UART_HandleTypeDef* huart_,uint8_t num,uint16_t size);
void analyse_buf_data_float(uint8_t num,float data[],uint16_t size,uint8_t lable);

#endif
