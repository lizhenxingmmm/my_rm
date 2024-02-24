#include "imu_task.h"
#include "JY901.h"
#include "usart.h"

void imu_task_init(void)
{
    User_USART_Init(&JY901_data);
    __HAL_UART_ENABLE_IT(&huart4,UART_IT_IDLE);   
	//HAL_UART_Receive_DMA(&huart4,(uint8_t *)RXbuf,sizeof(RXbuf)); 
    HAL_UART_Receive_DMA(&huart4,JY901_data.RxBuffer,RXBUFFER_LEN);
}
float yaw=0;
float xxangle=0;
void imu_task(void)
{
    yaw = JY901_data.angle.angle[2];
    xxangle = JY901_data.angle.angle[1];
}

