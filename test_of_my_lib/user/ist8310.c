#include "ist8310.h"

/*
微秒延时初始化
*/
static uint32_t delay_Init(void)
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
/*
delay us
*/
static void delay_us(volatile uint32_t au32_microseconds)
{
  uint32_t au32_initial_ticks = DWT->CYCCNT;
  uint32_t au32_ticks = (HAL_RCC_GetHCLKFreq() / 1000000);
  au32_microseconds *= au32_ticks;
  while ((DWT->CYCCNT - au32_initial_ticks) < au32_microseconds-au32_ticks);
}


IST8310_PIN RSTN;
IST8310_PIN DRYD;
I2C_HandleTypeDef* hi2c_ist8310;

static const uint8_t ist8310_write_reg_data_error[4][3] ={
        {0x0B, 0x08, 0x01},     
        {0x41, 0x09, 0x02},     
        {0x42, 0xC0, 0x03},     
        {0x0A, 0x0B, 0x04}
};    

/*
ist8310磁力计初始化，依次输入RSTN,DRYN的gpio类型和引脚号，i2c句柄类型指针
*/
void ist8310_init(GPIO_TypeDef *gpio_type_rstn,uint16_t GPIO_Pin_rstn,
GPIO_TypeDef *gpio_type_dryn,uint16_t GPIO_Pin_dyrn,I2C_HandleTypeDef* hi2c){
    RSTN.gpio_type=gpio_type_rstn;
    RSTN.GPIO_Pin=GPIO_Pin_rstn;
    DRYD.gpio_type=gpio_type_dryn;
    DRYD.GPIO_Pin=GPIO_Pin_dyrn;
    hi2c_ist8310=hi2c;

    delay_Init();
    HAL_GPIO_WritePin(RSTN.gpio_type,RSTN.GPIO_Pin,GPIO_PIN_RESET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(RSTN.gpio_type,RSTN.GPIO_Pin,GPIO_PIN_SET);
    HAL_Delay(50);
    uint8_t result = 0;
    HAL_I2C_Mem_Read(hi2c_ist8310,0x0e<<1,0x00,I2C_MEMADD_SIZE_8BIT,&result,1,10);
        for(int i=0;i<4;i++){
            uint8_t data=ist8310_write_reg_data_error[i][1];
            HAL_I2C_Mem_Write(hi2c_ist8310,0x0e<<1,ist8310_write_reg_data_error[i][0],
            I2C_MEMADD_SIZE_8BIT,&data,1,10);
            delay_us(150);
            uint8_t result_;
            HAL_I2C_Mem_Read(hi2c_ist8310,0x0e<<1,0x00,I2C_MEMADD_SIZE_8BIT,&result_,1,10);
            delay_us(150);
        }
}
float mag[3]={0.0f,0.0f,0.0f};
/*
在中断中调用，用于接收数据，返回3位float型数组指针
*/
float* ist8310_read(){
    uint8_t buffer_ist[6];
    HAL_I2C_Mem_Read(hi2c_ist8310, 0x0e<<1,0x03,I2C_MEMADD_SIZE_8BIT,buffer_ist,6,10);
    int16_t temp_ist8310_data = 0;
    temp_ist8310_data = (int16_t)((buffer_ist[1] << 8) | buffer_ist[0]);
    mag[0] = 0.3f * temp_ist8310_data;
    temp_ist8310_data = (int16_t)((buffer_ist[3] << 8) | buffer_ist[2]);
    mag[1] = 0.3f * temp_ist8310_data;
    temp_ist8310_data = (int16_t)((buffer_ist[5] << 8) | buffer_ist[4]);
    mag[2] = 0.3f * temp_ist8310_data;
    return mag;
}


