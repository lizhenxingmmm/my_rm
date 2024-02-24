#include "drv_uart.h"


uint8_t uart_rx_buf1[1000];
uint8_t uart_rx_buf2[1000];
uint8_t uart_rx_buf3[1000];
uint8_t uart_rx_buf4[1000];
uint8_t uart_rx_buf5[1000];
uint8_t uart_rx_buf6[1000];

uint8_t uart_tx_buf[1000];

/**
 * @brief uart发送float型数据(阻塞式通信)
 * @param data float数组(发送数据)
 * @param size 数组大小（有多少个float数）
 * @param lable 数据包标志，标记数据包开头位置，接收方可以识别包头，防止数据错位
*/
void uart_send_data_float(UART_HandleTypeDef* huart_,float data[],uint16_t size,uint8_t lable)
{
    uart_tx_buf[0]=lable;
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<4;j++)
        {
            uart_tx_buf[j+i*4+1]=*((char*)(&(data[i]))+j);
        }
    }
    HAL_UART_Transmit(huart_,uart_tx_buf,1+4*size,10);
}
/**
 * @brief 启用中断接收(非空闲中断)
 * @param huart_ uart句柄指针
 * @param num 数据缓冲区编号
 * @param size 要接收的数据大小(byte)(不包含数据包标志)
 * @note  非DMAcirculer模式需要在中断中重新启用
*/
void UART_IT_Init(UART_HandleTypeDef* huart_,uint8_t num,uint16_t size)
{
    HAL_UART_Receive_IT(&huart3,uart_rx_buf3,2*size+1);
}
/**
 * @brief uart接收float型数据(阻塞式通信)
 * @param num 数据缓冲区编号
 * @param data float数组(接收数据)
 * @param size 数组大小（有多少个float数）
 * @param lable 数据包标志，标记数据包开头位置，接收方可以识别包头，防止数据错位
*/
void analyse_buf_data_float(uint8_t num,float data[],uint16_t size,uint8_t lable)
{
    
    if(num==3)
    {
        uint16_t flag;
        for(int i=0;i<size*8+1;i++)
        {
            if(uart_rx_buf3[i]==lable)
            {
                flag=i;
                break;
            }
        }
        for(int i=0;i<size;i++)
        {
            for(int j=0;j<4;j++)
            {
                *((char*)(&data[i])+i)=uart_rx_buf3[flag+1+i];
            }
        }
    }
}
