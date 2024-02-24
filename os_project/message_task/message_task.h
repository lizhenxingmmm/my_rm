#ifndef MESSAGE_TASK_H
#define MESSAGE_TASK_H

#include "RMmotor.h"

extern RMmotor gimbal_motor;
extern RMmotor spring_motor;
extern RMmotor spring_motor_2;


void message_task_init(void);
void message_task(void);

#endif
