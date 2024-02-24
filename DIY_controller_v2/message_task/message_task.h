#ifndef MESSAGE_TASK_H
#define MESSAGE_TASK_H

#include "RMmotor.h"

extern RMmotor spring_motor;
extern RMmotor spring_motor_2;

typedef struct 
{
    int8_t yaw;
    int8_t pitch;
    int8_t roll;
    int8_t Vx;
    int8_t Vy;
    int8_t w;
}DataPack;


void message_task_init(void);
void message_task(void);

#endif
