#include "message_task.h"
#include "NRF24L01.h"
#include "usart.h"
uint8_t TxData[32]={0,0,0,0,0,0};
float debug;

uint8_t linux_data_buf[1000];
float rx_data[25];
void message_task()
{
    if(NRF24L01_TxPacket(TxData)==TX_OK)		
		{
			debug=666;
		}
	for(int j=0;j<25;j++)
	{
		for(int i=0;i<4;i++)
		{
			*((char*)(&(rx_data[j]))+i)=linux_data_buf[i+1+4*j];
		}
	}
}

void message_task_init()
{
	HAL_UART_Receive_IT(&huart6,linux_data_buf,17);
     Init_NRF24L01();
 //SCK_L;
         if(NRF24L01_Check())
	{
		debug=111;
	}
	//TX_Mode();
  TX_Mode();			//只收不发
}

//收数据
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan){
	CAN_RxHeaderTypeDef header;
	uint8_t data[8]; //用于接收数据
	HAL_CAN_GetRxMessage(hcan,CAN_FILTER_FIFO1,&header,data);
    receive_data(header.StdId,data,&gimbal_motor);
    receive_data(header.StdId,data,&spring_motor);
    receive_data(header.StdId,data,&spring_motor_2);
}
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
	CAN_RxHeaderTypeDef header;
	uint8_t data[8]; //用于接收数据
	HAL_CAN_GetRxMessage(hcan,CAN_FILTER_FIFO0,&header,data);
    receive_data(header.StdId,data,&gimbal_motor);
    receive_data(header.StdId,data,&spring_motor);
    receive_data(header.StdId,data,&spring_motor_2);
}


