/*
使用说明:
(0) 前提:
使能i2c(高速)(大疆c板需要使能i2c3,CLK:PA8,SDA:PC9)
使能一个外部中断gpio:上拉，下降沿触发中断
使能一个gpio_output:推挽输出,上拉,中等翻转速度,初始高电平
       (大疆c板中断口为PG3,输出口为PG6)
分别用于中断接收数据，重启ist8310
(1) ist8310_init(GPIO_TypeDef *gpio_type_rstn,uint16_t GPIO_Pin_rstn,
GPIO_TypeDef *gpio_type_dryn,uint16_t GPIO_Pin_dyrn,I2C_HandleTypeDef* hi2c);
初始化,依次输入RSTN,DRYN的gpio类型和引脚号，i2c句柄类型指针
(2) ist8310_read(void);
在中断中使用:
在main.c文件中定义一个全局变量
float* ist_data;用于接收数据
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
  if(GPIO_Pin==DRYD.GPIO_Pin){
    ist_data=ist8310_read();
  }
}
*/
#ifndef IST8310_H
#define IST8310_H

#include "gpio.h"
#include "i2c.h"
typedef struct 
{
    GPIO_TypeDef *gpio_type;
	uint16_t GPIO_Pin;
}IST8310_PIN;

extern IST8310_PIN DRYD;

void ist8310_init(GPIO_TypeDef *gpio_type_rstn,uint16_t GPIO_Pin_rstn,
GPIO_TypeDef *gpio_type_dryn,uint16_t GPIO_Pin_dyrn,I2C_HandleTypeDef* hi2c);
float* ist8310_read(void);


#endif
