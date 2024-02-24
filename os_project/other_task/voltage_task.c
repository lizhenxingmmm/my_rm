#include "voltage_task.h"
#include "ADCtest.h"

float current_battery_voltage;
void voltage_task_init(void)
{
    Vrefint_Init();
}

void voltage_task(void)
{
    current_battery_voltage=get_battery_voltage();
}
