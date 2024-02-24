#include "ADCtest.h"

/*
1.2V标准Vrefint与实际测得的Vrefint的比值
voltage_vrefint_proportion=1.2/average ; average为200次对Vrefint测量的平均值
*/
volatile float voltage_vrefint_proportion = 8.0586080586080586080586080586081e-4f;
/**
 * @brief 获取ADC测量值
 * @param ADCx ADC类型
 * @param ch 通道
 * @retval 读取到的ADC值
*/
static uint16_t ADCx_get_CHx_value(ADC_HandleTypeDef *ADCx, uint32_t ch){
    ADC_ChannelConfTypeDef the_channel;
    the_channel.Channel=ch;
    the_channel.Rank=1;
    the_channel.SamplingTime=ADC_SAMPLETIME_3CYCLES;
    if (HAL_ADC_ConfigChannel(ADCx, &the_channel) != HAL_OK)
    {
        Error_Handler();
    }
    HAL_ADC_Start(ADCx);
    HAL_ADC_PollForConversion(ADCx, 10);
    return (uint16_t)HAL_ADC_GetValue(ADCx);
}
/**
 * @brief 初始化voltage_vrefint_proportion，使测量更精确
*/
void Vrefint_Init(void)
{
    uint8_t i = 0;
    uint32_t total_adc = 0;
    for(i = 0; i < 200; i++)
    {
        total_adc += ADCx_get_CHx_value(&hadc1, ADC_CHANNEL_VREFINT);
    }

    voltage_vrefint_proportion = 200 * 1.2f / total_adc;
}
/**
 * @brief 读取C板电源电压
 * @retval C板电源电压
*/
float get_battery_voltage(void)
{
    float voltage;
    uint16_t adcx = 0;

    adcx = ADCx_get_CHx_value(&hadc3, ADC_CHANNEL_8);
    //(22千欧+ 200千欧)/ 22千欧 = 10.090909090909090909090909090909
    voltage =  (float)adcx * voltage_vrefint_proportion * 10.090909090909090909090909090909f;

    return voltage;
}
/**
 * @brief 读取C板温度
 * @retval C板温度
*/
float get_temperature(void)
{
    uint16_t adcx = 0;
    float temperature;
    adcx = ADCx_get_CHx_value(&hadc1, ADC_CHANNEL_TEMPSENSOR);
    temperature = (float)adcx * voltage_vrefint_proportion;
    temperature = (temperature - 0.76f) * 400.0f + 25.0f;

    return temperature;
}


