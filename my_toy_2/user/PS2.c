#include "PS2.h"


static PS2_PIN CLK;
static PS2_PIN CS;
static PS2_PIN CMD;
static PS2_PIN DAT;
u8 cmd[2]={0x01,0x42};	
u8 data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
u8 return_data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
/**
*@brief PS2初始化，请分别输入CLK,CS,CMD,DAT的GPIO口的类型和引脚编号.
配置要求：CLK,CMD,CS为GPIO_OUTPUT_MODE，高速，推挽输出，悬空。DAT为GPIO_INPUT_MODE，下拉
**/
void PS2_Init(GPIO_TypeDef *gpio_clk,uint16_t GPIO_Pin_clk,GPIO_TypeDef *gpio_cs,
uint16_t GPIO_Pin_cs,GPIO_TypeDef *gpio_cmd,uint16_t GPIO_Pin_cmd,GPIO_TypeDef *gpio_dat,uint16_t GPIO_Pin_dat)
{
	delay_Init();
	CLK.gpio_type=gpio_clk;
	CLK.GPIO_Pin=GPIO_Pin_clk;
	CS.gpio_type=gpio_cs;
	CS.GPIO_Pin=GPIO_Pin_cs;
	CMD.gpio_type=gpio_cmd;
	CMD.GPIO_Pin=GPIO_Pin_cmd;
	DAT.gpio_type=gpio_dat;
	DAT.GPIO_Pin=GPIO_Pin_dat;

	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_EnterConfing();		
	PS2_TurnOnAnalogMode();	
	PS2_VibrationMode();	
	PS2_ExitConfing();		

	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
}

void PS2_Cmd(u8 CMD_)
{
	volatile u16 ref=0x01;
	data[1] = 0;
	for(ref=0x01;ref<0x0100;ref<<=1)
	{
		if(ref&CMD_)
		{
			HAL_GPIO_WritePin(CMD.gpio_type,CMD.GPIO_Pin,GPIO_PIN_SET);                  
		}
		else HAL_GPIO_WritePin(CMD.gpio_type,CMD.GPIO_Pin,GPIO_PIN_RESET); 

		HAL_GPIO_WritePin(CLK.gpio_type,CLK.GPIO_Pin,GPIO_PIN_SET);                         
		delay_us(8);
		HAL_GPIO_WritePin(CLK.gpio_type,CLK.GPIO_Pin,GPIO_PIN_RESET);
		delay_us(8);
		HAL_GPIO_WritePin(CLK.gpio_type,CLK.GPIO_Pin,GPIO_PIN_SET);
		if(HAL_GPIO_ReadPin(DAT.gpio_type,DAT.GPIO_Pin))
			data[1] = ref|data[1];
	}
	delay_us(24);
}
/*
返回大小为9的uint8_t数组的指针
*/
uint8_t* PS2_ReadData(void)
{
	volatile u8 byte=0;
	volatile u16 ref=0x01;
	HAL_GPIO_WritePin(CS.gpio_type,CS.GPIO_Pin,GPIO_PIN_RESET);
	PS2_Cmd(cmd[0]); 
	PS2_Cmd(cmd[1]); 
	for(byte=2;byte<9;byte++)          
	{
		for(ref=0x01;ref<0x100;ref<<=1)
		{
			HAL_GPIO_WritePin(CLK.gpio_type,CLK.GPIO_Pin,GPIO_PIN_SET);
			delay_us(8);
			HAL_GPIO_WritePin(CLK.gpio_type,CLK.GPIO_Pin,GPIO_PIN_RESET);
			delay_us(8);
			HAL_GPIO_WritePin(CLK.gpio_type,CLK.GPIO_Pin,GPIO_PIN_SET);
		      if(HAL_GPIO_ReadPin(DAT.gpio_type,DAT.GPIO_Pin))
		      data[byte] = ref|data[byte];
		}
        delay_us(24);
	}
	HAL_GPIO_WritePin(CS.gpio_type,CS.GPIO_Pin,GPIO_PIN_SET);
	for(int i=0;i<9;i++){
		return_data[i]=data[i];
	}
	PS2_ClearData();
	return return_data;
}

void PS2_ShortPoll(void)
{
	HAL_GPIO_WritePin(CS.gpio_type,CS.GPIO_Pin,GPIO_PIN_RESET);
	delay_us(24);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x42);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x00);
	HAL_GPIO_WritePin(CS.gpio_type,CS.GPIO_Pin,GPIO_PIN_SET);
	delay_us(24);	
}

void PS2_EnterConfing(void)
{
    HAL_GPIO_WritePin(CS.gpio_type,CS.GPIO_Pin,GPIO_PIN_RESET);
	delay_us(24);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01);
	PS2_Cmd(0x00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	HAL_GPIO_WritePin(CS.gpio_type,CS.GPIO_Pin,GPIO_PIN_SET);
	delay_us(24);
}

void PS2_TurnOnAnalogMode(void)
{
	HAL_GPIO_WritePin(CS.gpio_type,CS.GPIO_Pin,GPIO_PIN_RESET);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x44);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01); 
	PS2_Cmd(0xEE); 
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	HAL_GPIO_WritePin(CS.gpio_type,CS.GPIO_Pin,GPIO_PIN_SET);
	delay_us(24);
}

void PS2_VibrationMode(void)
{
	HAL_GPIO_WritePin(CS.gpio_type,CS.GPIO_Pin,GPIO_PIN_RESET);
	delay_us(24);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x4D);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0X01);
	HAL_GPIO_WritePin(CS.gpio_type,CS.GPIO_Pin,GPIO_PIN_SET);
	delay_us(24);	
}

void PS2_ExitConfing(void)
{
    HAL_GPIO_WritePin(CS.gpio_type,CS.GPIO_Pin,GPIO_PIN_RESET);
	delay_us(24);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	HAL_GPIO_WritePin(CS.gpio_type,CS.GPIO_Pin,GPIO_PIN_SET);
	delay_us(24);
}
/*
PS2初始化之后，开启振动模式才能执行该函数
motor1，右边手柄电机，motor2，左边手柄电机
*/
void PS2_Vibration(u8 motor1, u8 motor2)
{
	HAL_GPIO_WritePin(CS.gpio_type,CS.GPIO_Pin,GPIO_PIN_RESET);
	delay_us(24);
  	PS2_Cmd(0x01);  
	PS2_Cmd(0x42);  
	PS2_Cmd(0X00);
	PS2_Cmd(motor1);
	PS2_Cmd(motor2);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	HAL_GPIO_WritePin(CS.gpio_type,CS.GPIO_Pin,GPIO_PIN_SET);
	delay_us(24);  
}

void PS2_ClearData()
{
	u8 a;
	for(a=0;a<9;a++)
		data[a]=0x00;
}

uint32_t delay_Init(void)
{
    /* Disable TRC */
    CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
    /* Enable TRC */
    CoreDebug->DEMCR |=  CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;
 
    /* Disable clock cycle counter */
    DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
    /* Enable  clock cycle counter */
    DWT->CTRL |=  DWT_CTRL_CYCCNTENA_Msk; //0x00000001;
 
    /* Reset the clock cycle counter value */
    DWT->CYCCNT = 0;
 
    /* 3 NO OPERATION instructions */
    __ASM volatile ("NOP");
    __ASM volatile ("NOP");
    __ASM volatile ("NOP");
 
    /* Check if clock cycle counter has started */
    if(DWT->CYCCNT)
    {
       return 0; /*clock cycle counter started*/
    }
    else
    {
      return 1; /*clock cycle counter not started*/
    }
}

void delay_us(volatile uint32_t au32_microseconds)
{
  uint32_t au32_initial_ticks = DWT->CYCCNT;
  uint32_t au32_ticks = (HAL_RCC_GetHCLKFreq() / 1000000);
  au32_microseconds *= au32_ticks;
  while ((DWT->CYCCNT - au32_initial_ticks) < au32_microseconds-au32_ticks);
}
