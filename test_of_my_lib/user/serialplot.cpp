#include "serialplot.hpp"
/**
 * @brief 发送1个浮点数到serialplot
*/
void send_to_serialplot_1(float num,UART_HandleTypeDef huart_){
    uint8_t data[5]={0xAB};
    for(int i=1;i<5;i++){
        data[i]=*((char *)(&num)+(i-1));
    }
    HAL_UART_Transmit(&huart_,data,5,10); 
}
/**
 * @brief 发送2个浮点数到serialplot
*/
void send_to_serialplot_2(float num1,float num2,UART_HandleTypeDef huart_){
    uint8_t data[256]={0xAB};
    for(int i=1;i<5;i++){
        data[i]=*((char *)(&num1)+(i-1));
    }
    for(int i=5;i<9;i++){
        data[i]=*((char *)(&num2)+(i-5));
    }
     HAL_UART_Transmit(&huart_,data,9,10);
}

