#include "serial_pilot.h"

void send_to_serialpilot(float num,UART_HandleTypeDef huart_){
    uint8_t data[5]={0xAB};
    for(int i=1;i<5;i++){
        data[i]=*((char *)(&num)+(i-1));
    }
    HAL_UART_Transmit(&huart_,data,5,10);                   //10ms无所谓，没有特殊含义，可以为任意值
}

void send_to_serialpilot_2(float num1,float num2,UART_HandleTypeDef huart_){
    uint8_t data[256]={0xAB};
    for(int i=1;i<5;i++){
        data[i]=*((char *)(&num1)+(i-1));
    }
    for(int i=5;i<9;i++){
        data[i]=*((char *)(&num2)+(i-5));
    }
     HAL_UART_Transmit(&huart_,data,9,10);                   //10ms无所谓，没有特殊含义，可以为任意值
     HAL_Delay(0);
}
