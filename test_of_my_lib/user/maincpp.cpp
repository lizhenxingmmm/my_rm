#include "maincpp.hpp"

#include "main.h"
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "usart.h"

#include "PS2.h"
#include "i2c_oled.hpp"
#include "Macanum.h"
#include "ist8310.h"
#include "BMI088.h"
#include "ADCtest.h"
#include "drv_can.h"
#include "RMmotor.hpp"
#include "serialplot.hpp"

/* USER CODE BEGIN 0 */
RMmotor PTZ(0x1ff,0x205);
float* ist_data;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
  if(GPIO_Pin==DRYD.GPIO_Pin){
    ist_data=ist8310_read();
  }
}
float accel[3],gyro[3],temperature;
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan){
	CAN_RxHeaderTypeDef header;
	uint8_t data[8]; //用于接收数据(不知道为什么，这里不能是uint8_t* data;)(知道了，只定义指针没有开辟连续的数组空间)
	HAL_CAN_GetRxMessage(hcan,CAN_FILTER_FIFO1,&header,data);
  PTZ.receive_data(header.StdId,data);
}
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
	CAN_RxHeaderTypeDef header;
	uint8_t data[8]; //用于接收数据(不知道为什么，这里不能是uint8_t* data;)(知道了，只定义指针没有开辟连续的数组空间)
	HAL_CAN_GetRxMessage(hcan,CAN_FILTER_FIFO0,&header,data);
  PTZ.receive_data(header.StdId,data);
}
uint8_t rx_uart_data;
float target=0;
float P=0;
float I=0;
float D=0;
float play=0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if(rx_uart_data=='0'){
      target=0;
    }
    if(rx_uart_data=='1'){
      target=100;
    }
    if(rx_uart_data=='p'){
      P+=1;
    }
    if(rx_uart_data=='i'){
      I+=1;
    }
    if(rx_uart_data=='d'){
      D+=1;
    }
    if(rx_uart_data=='P'){
      P-=1;
    }
    if(rx_uart_data=='I'){
      I-=1;
    }
    if(rx_uart_data=='D'){
      D-=1;
    }
    if(rx_uart_data=='q'){
      play=1;
    }
    HAL_UART_Receive_DMA(&huart6,&rx_uart_data,1);
}
/* USER CODE END 0 */


void maincpp(){
/* USER CODE BEGIN 2 */
ist8310_init(GPIOG,GPIO_PIN_6,GPIOG,GPIO_PIN_3,&hi2c3);
PS2_Init(GPIOB,GPIO_PIN_12,GPIOB,GPIO_PIN_14,GPIOB,GPIO_PIN_15,GPIOB,GPIO_PIN_13);
OLED_Init(&hi2c2);
BMI088_Init();
while(1){
  BMI088_Init();
  BMI088_read(accel,gyro,&temperature);
  if(temperature!=-105){
    break;
  }
}
CAN_Init(&hcan1);
Vrefint_Init();
HAL_UART_Receive_DMA(&huart6,&rx_uart_data,1);
/* USER CODE END 2 */
/*infinit loop*/
while(1){
  send_to_serialplot_2(target,PTZ.speed,huart6);
    send_to_motors(PTZ.pid_calcu_speed(target,PTZ.speed,P,I,D,1000,25000,0),0,0,0,0x1ff,&hcan1);
    OLED_ShowNum_withsign(0,0,PTZ.angle,6,16,1);
    OLED_ShowNum_withsign(0,16,PTZ.speed,6,16,1);
    OLED_ShowNum_withsign(0,32,PTZ.torque,6,16,1);
    OLED_ShowNum_withsign(0,48,PTZ.temperature,6,16,1);
    OLED_ShowNum_withsign(64,0,get_battery_voltage(),6,16,1);
    OLED_ShowNum_withsign(64,16,P,6,16,1);
    OLED_ShowNum_withsign(64,32,I,6,16,1);
    OLED_ShowNum_withsign(64,48,D,6,16,1);
    if(play){
      OLED_Refresh();
      play=0;
    }
}
}


