#include "message_task.h"
#include "NRF24L01.h"
#include "drv_uart.h"
#include "imu_task.h"
#include "spring_motor_task.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"
extern  RMmotor spring_motor;
extern  RMmotor spring_motor_2;
extern  float yaw;
extern  float xxangle;
float debug__;
float debug2;
float debug3;
float test_data[3];
DataPack datapack={-1,-2,-3,-4,-5,-6};
	uint8_t TxData[32] = {0,0,0,0};
uint32_t analog_data=0;
GPIO_PinState current_state_1=GPIO_PIN_SET;
GPIO_PinState current_state_2=GPIO_PIN_SET;
void message_task()
{
	datapack.Vx=(int8_t)870 - (int8_t)(spring_motor_2.angle/6);
	datapack.Vy=(int8_t)910 - (int8_t)(spring_motor.angle/6);
	// datapack.yaw=yaw/1.5f;
	// datapack.pitch=xxangle/1.5f;
	TxData[0] = 0x5A;
	for(int i=0;i<4;i++)
	{
		TxData[1+i]=*((char*)&yaw+i);
	}
	for(int i=0;i<4;i++)
	{
		TxData[5+i]=*((char*)&xxangle+i);
	}
	if(datapack.Vx<20 && datapack.Vx>-20) 
	{TxData[9] = 0;}
	else{TxData[9]=(uint8_t)datapack.Vx;}
	if(datapack.Vy<20 && datapack.Vy>-20) 
	{TxData[10] = 0;}
	else
	{
	TxData[10] = (uint8_t)datapack.Vy;}
	if(analog_data<350 && analog_data>100)
	{
		TxData[11]=0;
	}
	if(analog_data<=100)
	{
		TxData[11]=(uint8_t)(-1);
	}
	if(analog_data>=350 && analog_data<=600)
	{
		TxData[11]=1;
	}
	current_state_1=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5);
	current_state_2=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4);
	if(current_state_1==GPIO_PIN_RESET)TxData[12]=1;
	else TxData[12]=0;
	if(current_state_2==GPIO_PIN_RESET)TxData[13]=1;
	else TxData[13]=0;
	TxData[14]=0x6A;
       //uint8_t TxData[32];
        //NRF24L01_TxPacket(TxData);
       if(NRF24L01_TxPacket(TxData)==TX_OK)		
		{
			debug__=666666;
		}
	imu_task();
}

void message_task_init()
{
	Init_NRF24L01();
 //SCK_L;
         if(NRF24L01_Check())
	{
		debug2 = 1;
	}
	TX_Mode();
	UART_IT_Init(&huart3,3,12);
	imu_task_init();
	HAL_ADC_Start_DMA(&hadc1,&analog_data,1);
}

//收数据
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan){
	CAN_RxHeaderTypeDef header;
	uint8_t data[8]; //用于接收数据
	HAL_CAN_GetRxMessage(hcan,CAN_FILTER_FIFO1,&header,data);
    
    receive_data(header.StdId,data,&spring_motor);
    receive_data(header.StdId,data,&spring_motor_2);
}
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
	CAN_RxHeaderTypeDef header;
	uint8_t data[8]; //用于接收数据
	HAL_CAN_GetRxMessage(hcan,CAN_FILTER_FIFO0,&header,data);
    
    receive_data(header.StdId,data,&spring_motor);
    receive_data(header.StdId,data,&spring_motor_2);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart==&huart3)
	{
		debug2++;
		analyse_buf_data_float(3,test_data,3,0xAB);
		UART_IT_Init(&huart3,3,12);
	}
}



