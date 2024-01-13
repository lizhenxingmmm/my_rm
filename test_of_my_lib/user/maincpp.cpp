#include "maincpp.hpp"

#include "main.h"
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

#include "PS2.h"
#include "i2c_oled.hpp"
#include "Macanum.h"
#include "ist8310.h"
#include "BMI088.h"
#include "ADCtest.h"
#include "drv_can.h"
#include "RMmotor.hpp"


/* USER CODE BEGIN 0 */
float* ist_data;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
  if(GPIO_Pin==DRYD.GPIO_Pin){
    ist_data=ist8310_read();
  }
}
float accel[3],gyro[3],temperature;
int16_t angle;
int16_t speed;
int16_t torque;
int16_t temperature_motor;
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan){
	CAN_RxHeaderTypeDef header;
	uint8_t data[8]; //用于接收数据(不知道为什么，这里不能是uint8_t* data;)
	HAL_CAN_GetRxMessage(hcan,CAN_FILTER_FIFO1,&header,data);
  int16_t _angle=(((int16_t)data[0])<<8)+data[1];
  int16_t _speed=(((int16_t)data[2])<<8)+data[3];
  int16_t _torque=(((int16_t)data[4])<<8)+data[5];
  int16_t _temperature=data[7];
  angle=_angle;
  speed=_speed;
  torque=_torque;
  temperature_motor=_temperature;
  HAL_GPIO_WritePin(GPIOH,GPIO_PIN_10,GPIO_PIN_SET);
}
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
	CAN_RxHeaderTypeDef header;
	uint8_t data[8]; //用于接收数据(不知道为什么，这里不能是uint8_t* data;)
	HAL_CAN_GetRxMessage(hcan,CAN_FILTER_FIFO0,&header,data);
  int16_t _angle=(((int16_t)data[0])<<8)+data[1];
  int16_t _speed=(((int16_t)data[2])<<8)+data[3];
  int16_t _torque=(((int16_t)data[4])<<8)+data[5];
  int16_t _temperature=data[7];
  angle=_angle;
  speed=_speed;
  torque=_torque;
  temperature_motor=_temperature;
  HAL_GPIO_WritePin(GPIOH,GPIO_PIN_10,GPIO_PIN_SET);
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
/* USER CODE END 2 */
/*infinit loop*/
while(1){
    send_to_motors(0,0,0,0,0x1ff,&hcan1);
    OLED_ShowNum_withsign(0,0,angle,6,16,1);
    OLED_ShowNum_withsign(0,16,speed,6,16,1);
    OLED_ShowNum_withsign(0,32,torque,6,16,1);
    OLED_ShowNum_withsign(0,48,temperature_motor,6,16,1);
    OLED_ShowNum_withsign(64,0,get_battery_voltage(),6,16,1);
    OLED_Refresh();
}
}

