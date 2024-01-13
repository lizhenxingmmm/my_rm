/*
使用说明：
(0) 前提:
配置3个高速GPIO_out,1个GPIO_in，下拉
(1) PS2_Init
PS2初始化，请分别输入CLK,CS,CMD,DAT的GPIO口的类型和引脚编号.
配置要求：CLK,CMD,CS为GPIO_OUTPUT_MODE，高速，推挽输出，悬空。DAT为GPIO_INPUT_MODE，下拉
(2) uint8_t* PS2_ReadData(void)
返回PS2数据uint8_t型长度为9的数组，具体数据如下
byte 0 :
STM32（DO） : 0x01 ------------------------- [现在开始通信]
PS2手柄（DI） : 空 ---------------------------- [空]
byte 1 :
STM32（DO） : 0x42 -------------------------- [请求发送数据]
PS2手柄（DI） : 红灯0x73
            绿灯0X41 ---------------------[现在的ID]
byte 2 :
STM32（DO） : 空 ------------------------------ [空]
PS2手柄（DI） : 0X5A ------------------------- [数据来了]
byte 3 :
STM32（DO） : 0X00~0XFF ------------------ [右侧小震动电机是否开启]
PS2手柄（DI） : 00000000~11111111 ------- [SELECT、 L3 、 R3、 START 、 UP、 RIGHT、 DOWN、 LEFT 是否被按下，若被按下对应位为0]
byte 4 :
STM32（DO） : 0X00~0XFF ------------------ [左侧大震动电机振动幅度]
PS2手柄（DI） : 00000000~11111111 ------- [L2 、 R2、L1 、R1、△、○、╳、□ 是否被按下，若被按下对应位为0]
byte 5 :
STM32（DO） : 空 -------------------------------- [空]
PS2手柄（DI） : 0X00~0XFF ------------------ [左侧X轴摇杆模拟量]
byte 6 :
STM32（DO） : 空 -------------------------------- [空]
PS2手柄（DI） : 0X00~0XFF ------------------ [左侧Y轴摇杆模拟量]
byte 7 :
STM32（DO） : 空 -------------------------------- [空]
PS2手柄（DI） : 0X00~0XFF ------------------ [右侧X轴摇杆模拟量]
byte 8 :
STM32（DO） : 空 -------------------------------- [空]
PS2手柄（DI） : 0X00~0XFF ------------------ [右侧Y轴摇杆模拟量]
(3) delay_us
毫秒级延时
*/
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
static uint32_t delay_Init(void);
static void delay_us(volatile uint32_t au32_microseconds);	 

#endif
