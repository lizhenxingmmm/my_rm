/*
(0) 配置:
(大疆C板)(用基准电压1.2伏)
ADC1 勾选Temperature Sensor Channel,Vrefint Channel
采样频率设置为 PCLK2/4，采样位数为 12 位，数据设置为右对齐，其余均保持默认
ADC3 勾选IN8 对应PF10
(1) Vrefint_Init(void);
初始化voltage_vrefint_proportion，使测量更精确
(2) get_battery_voltage(void);
(3) get_temperature(void);
*/
#ifndef ADCTEST_H
#define ADCTEST_H

#include "main.h"
#include "adc.h"

void Vrefint_Init(void);
float get_battery_voltage(void);
float get_temperature(void);



#endif
