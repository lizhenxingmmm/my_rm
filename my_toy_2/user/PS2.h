#ifndef  __PS2_H
#define  __PS2_H


#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "gpio.h"

typedef uint16_t u16;
typedef uint8_t u8;

typedef struct 
{
	GPIO_TypeDef *gpio_type;
	uint16_t GPIO_Pin;
}PS2_PIN;

void PS2_Init(GPIO_TypeDef *gpio_clk,uint16_t GPIO_Pin_clk,GPIO_TypeDef *gpio_cs,
		uint16_t GPIO_Pin_cs,GPIO_TypeDef *gpio_cmd,uint16_t GPIO_Pin_cmd,GPIO_TypeDef *gpio_dat,uint16_t GPIO_Pin_dat);
uint8_t* PS2_ReadData(void); 
void PS2_ClearData();
void PS2_Cmd(u8 CMD);	
void PS2_ShortPoll(void);
void PS2_EnterConfing(void);
void PS2_TurnOnAnalogMode(void);
void PS2_VibrationMode(void);
void PS2_ExitConfing(void);
void PS2_Vibration(u8 motor1, u8 motor2);
uint32_t delay_Init(void);
void delay_us(volatile uint32_t au32_microseconds);	 

#endif
