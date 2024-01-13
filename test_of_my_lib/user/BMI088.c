#include "BMI088.h"

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
/**
 * @brief 读BMI088加速度计的一位寄存器
 * @param reg 寄存器ID
 * @retval 返回读到的数据
*/
uint8_t BMI088_Accel_Read_Single_Reg(uint8_t reg){
    uint8_t data_feedback;
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
    uint8_t dummy_rx_data;
    uint8_t tx_data=((reg) | 0x80);
    HAL_SPI_TransmitReceive(&hspi1,&tx_data,&dummy_rx_data,1,1000);
    uint8_t tx_data_1=0x55;
    HAL_SPI_TransmitReceive(&hspi1,&tx_data_1,&dummy_rx_data,1,1000);
    HAL_SPI_TransmitReceive(&hspi1,&tx_data_1,&data_feedback,1,1000);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
    return data_feedback;
}
/**
 * @brief 写一位寄存器
 * @param reg 寄存器地址
 * @param data 写入的数据
*/
static void BMI088_Write_Single_Reg(uint8_t reg, uint8_t data){
    uint8_t dummy_rx_data;
    HAL_SPI_TransmitReceive(&hspi1,&reg,&dummy_rx_data,1,1000);
    HAL_SPI_TransmitReceive(&hspi1,&data,&dummy_rx_data,1,1000);
}
/**
 * @brief 向BMI088的加速度计写一位寄存器
 * @param reg 寄存器地址
 * @param data 写入的数据
*/
void BMI088_Accel_Write_Single_Reg(uint8_t reg,uint8_t data){
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
    BMI088_Write_Single_Reg(reg,data);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
}

static uint8_t write_BMI088_accel_reg_data_error[6][3] =
    {
        {BMI088_ACC_PWR_CTRL, BMI088_ACC_ENABLE_ACC_ON, BMI088_ACC_PWR_CTRL_ERROR},
        {BMI088_ACC_PWR_CONF, BMI088_ACC_PWR_ACTIVE_MODE, BMI088_ACC_PWR_CONF_ERROR},
        {BMI088_ACC_CONF,  BMI088_ACC_NORMAL| BMI088_ACC_800_HZ | BMI088_ACC_CONF_MUST_Set, BMI088_ACC_CONF_ERROR},
        {BMI088_ACC_RANGE, BMI088_ACC_RANGE_3G, BMI088_ACC_RANGE_ERROR},
        {BMI088_INT1_IO_CTRL, BMI088_ACC_INT1_IO_ENABLE | BMI088_ACC_INT1_GPIO_PP | BMI088_ACC_INT1_GPIO_LOW, BMI088_INT1_IO_CTRL_ERROR},
        {BMI088_INT_MAP_DATA, BMI088_ACC_INT1_DRDY_INTERRUPT, BMI088_INT_MAP_DATA_ERROR}

};
/**
 * @brief BMI088加速度计初始化配置并检查
 * @retval 检查结果,无错误会返回0x00,有错误会返回相应错误对应的值
*/
uint8_t BMI088_Accel_Init(){
    uint8_t res =BMI088_Accel_Read_Single_Reg(BMI088_ACC_CHIP_ID);
    delay_us(150);
    res =BMI088_Accel_Read_Single_Reg(BMI088_ACC_CHIP_ID);
    delay_us(150);
    BMI088_Accel_Write_Single_Reg(BMI088_ACC_SOFTRESET,BMI088_ACC_SOFTRESET_VALUE);
    delay_us(80);
    BMI088_Accel_Read_Single_Reg(BMI088_ACC_CHIP_ID);
    delay_us(150);
    BMI088_Accel_Read_Single_Reg(BMI088_ACC_CHIP_ID);
    delay_us(150);
    //check who you are
    if (res != BMI088_ACC_CHIP_ID_VALUE)
    {
        return BMI088_NO_SENSOR;
    }

    //set accel sencor config and check
    for (uint8_t write_reg_num = 0; write_reg_num < 6; write_reg_num++)
    {

        BMI088_Accel_Write_Single_Reg(write_BMI088_accel_reg_data_error[write_reg_num][0], write_BMI088_accel_reg_data_error[write_reg_num][1]);
        delay_us(150);
        uint8_t res=BMI088_Accel_Read_Single_Reg(write_BMI088_accel_reg_data_error[write_reg_num][0]);
        delay_us(150);

        if (res != write_BMI088_accel_reg_data_error[write_reg_num][1])
        {
            return write_BMI088_accel_reg_data_error[write_reg_num][2];
        }
    }
    return BMI088_NO_ERROR;
}
/**
 * @brief 读BMI088陀螺仪的一位寄存器
 * @param reg 寄存器ID
 * @retval 返回读到的数据
*/
uint8_t BMI088_Gyro_Read_Single_Reg(uint8_t reg){
    uint8_t data_feedback;
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
    uint8_t dummy_rx_data;
    uint8_t tx_data=((reg) | 0x80);
    HAL_SPI_TransmitReceive(&hspi1,&tx_data,&dummy_rx_data,1,1000);
    uint8_t tx_data_1=0x55;
    HAL_SPI_TransmitReceive(&hspi1,&tx_data_1,&data_feedback,1,1000);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
    return data_feedback;
}
/**
 * @brief 向BMI088的陀螺仪写一位寄存器
 * @param reg 寄存器地址
 * @param data 写入的数据
*/
void BMI088_Gyro_Write_Single_Reg(uint8_t reg,uint8_t data){
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
    BMI088_Write_Single_Reg(reg,data);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
}

static uint8_t write_BMI088_gyro_reg_data_error[6][3] =
    {
        {BMI088_GYRO_RANGE, BMI088_GYRO_2000, BMI088_GYRO_RANGE_ERROR},
        {BMI088_GYRO_BANDWIDTH, BMI088_GYRO_1000_116_HZ | BMI088_GYRO_BANDWIDTH_MUST_Set, BMI088_GYRO_BANDWIDTH_ERROR},
        {BMI088_GYRO_LPM1, BMI088_GYRO_NORMAL_MODE, BMI088_GYRO_LPM1_ERROR},
        {BMI088_GYRO_CTRL, BMI088_DRDY_ON, BMI088_GYRO_CTRL_ERROR},
        {BMI088_GYRO_INT3_INT4_IO_CONF, BMI088_GYRO_INT3_GPIO_PP | BMI088_GYRO_INT3_GPIO_LOW, BMI088_GYRO_INT3_INT4_IO_CONF_ERROR},
        {BMI088_GYRO_INT3_INT4_IO_MAP, BMI088_GYRO_DRDY_IO_INT3, BMI088_GYRO_INT3_INT4_IO_MAP_ERROR}

};
/**
 * @brief BMI088陀螺仪初始化配置并检查
 * @retval 检查结果,无错误会返回0x00,有错误会返回相应错误对应的值
*/
uint8_t BMI088_Gyro_Init(){
    uint8_t res =BMI088_Gyro_Read_Single_Reg(BMI088_GYRO_CHIP_ID);
    delay_us(150);
    res =BMI088_Gyro_Read_Single_Reg(BMI088_GYRO_CHIP_ID);
    delay_us(150);
    BMI088_Gyro_Write_Single_Reg(BMI088_GYRO_SOFTRESET, BMI088_GYRO_SOFTRESET_VALUE);
    delay_us(80);
    BMI088_Gyro_Read_Single_Reg(BMI088_GYRO_CHIP_ID);
    delay_us(150);
    BMI088_Gyro_Read_Single_Reg(BMI088_GYRO_CHIP_ID);
    delay_us(150);
    //check who you are
    if (res != BMI088_GYRO_CHIP_ID_VALUE)
    {
        return BMI088_NO_SENSOR;
    }
    
    //set gyro sencor config and check
    for (uint8_t write_reg_num = 0; write_reg_num < 6; write_reg_num++)
    {

        BMI088_Gyro_Write_Single_Reg(write_BMI088_gyro_reg_data_error[write_reg_num][0], write_BMI088_accel_reg_data_error[write_reg_num][1]);
        delay_us(150);
        res=BMI088_Gyro_Read_Single_Reg(write_BMI088_gyro_reg_data_error[write_reg_num][0]);
        delay_us(150);

        if (res != write_BMI088_gyro_reg_data_error[write_reg_num][1])
        {
            return write_BMI088_gyro_reg_data_error[write_reg_num][2];
        }
    }
    return BMI088_NO_ERROR;
}
/**
 * @brief BMI088初始化
 * @retval 初始化成功返回0x00,有错误会返回相应错误对应的值
*/
uint8_t BMI088_Init(){
    delay_Init();
    uint8_t error = BMI088_NO_ERROR;
    error=error | BMI088_Accel_Init();
    error=error | BMI088_Gyro_Init();
    return error;
}
/**
 * @brief 读多位寄存器
 * @param reg 寄存器地址
 * @param buf 接收数据缓冲区的数组指针
 * @param len 读取长度
*/
static void BMI088_Read_Multi_Reg(uint8_t reg, uint8_t *buf, uint8_t len){
    uint8_t tx_data=((reg) | 0x80);
    uint8_t dummy_rx_data;
    HAL_SPI_TransmitReceive(&hspi1,&tx_data,&dummy_rx_data,1,1000);
    uint8_t tx_data_1=0x55;
    for(uint8_t i=0;i<len;i++){
        HAL_SPI_TransmitReceive(&hspi1,&tx_data_1,buf,1,1000);
        buf++;
    }
}
/**
 * @brief 读BMI088加速度计的多位寄存器
 * @param reg 寄存器地址
 * @param buffer 接收数据缓冲区的数组指针
 * @param len 读取长度
*/
void BMI088_Accel_Read_Multi_Reg(uint8_t reg,uint8_t* buffer,uint8_t len){
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
    uint8_t tx_data=((reg) | 0x80);
    uint8_t dummy_rx_data;
    HAL_SPI_TransmitReceive(&hspi1,&tx_data,&dummy_rx_data,1,1000);
    BMI088_Read_Multi_Reg(reg,buffer,len);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
}
/**
 * @brief 读BMI088陀螺仪的多位寄存器
 * @param reg 寄存器地址
 * @param buffer 接收数据缓冲区的数组指针
 * @param len 读取长度
*/
void BMI088_Gyro_Read_Multi_Reg(uint8_t reg,uint8_t* buffer,uint8_t len){
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
    BMI088_Read_Multi_Reg(reg,buffer,len);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
}

void BMI088_read(float accel[3],float gyro[3],float* temperature){
    uint8_t buf[8] = {0, 0, 0, 0, 0, 0};
    int16_t bmi088_raw_temp;

    BMI088_Accel_Read_Multi_Reg(BMI088_ACCEL_XOUT_L,buf,6);
    bmi088_raw_temp = (int16_t)((buf[1]) << 8) | buf[0];
    accel[0] = bmi088_raw_temp * 0.0008974358974f;
    bmi088_raw_temp = (int16_t)((buf[3]) << 8) | buf[2];
    accel[1] = bmi088_raw_temp * 0.0008974358974f;
    bmi088_raw_temp = (int16_t)((buf[5]) << 8) | buf[4];
    accel[2] = bmi088_raw_temp * 0.0008974358974f;

    BMI088_Gyro_Read_Multi_Reg(BMI088_GYRO_CHIP_ID, buf, 8);
    if(buf[0] == BMI088_GYRO_CHIP_ID_VALUE)
    {
        bmi088_raw_temp = (int16_t)((buf[3]) << 8) | buf[2];
        gyro[0] = bmi088_raw_temp * 0.00106526443603169529841533860381f;
        bmi088_raw_temp = (int16_t)((buf[5]) << 8) | buf[4];
        gyro[1] = bmi088_raw_temp * 0.00106526443603169529841533860381f;
        bmi088_raw_temp = (int16_t)((buf[7]) << 8) | buf[6];
        gyro[2] = bmi088_raw_temp * 0.00106526443603169529841533860381f;
    }

    BMI088_Accel_Read_Multi_Reg(BMI088_TEMP_M,buf,2);
    bmi088_raw_temp = (int16_t)((buf[0] << 3) | (buf[1] >> 5));
    if (bmi088_raw_temp > 1023)
    {
        bmi088_raw_temp -= 2048;
    }
    *temperature = bmi088_raw_temp * 0.125f + 23.0f;
}
