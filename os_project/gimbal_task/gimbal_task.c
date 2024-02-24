#include "gimbal_task.h"
#include "RMmotor.h"
#include "pid.h"
#include "message_task.h"

RMmotor gimbal_motor;
pid_type_def gimbal_pid_angle;

float target;
float target_angle;
uint8_t serial_data;

void gimbal_task_init(void)
{
	CAN_Init(&hcan1);
    RMmotor_init(&gimbal_motor,0x1ff,0x205);
    PID_init(&gimbal_pid_angle,PID_POSITION,0.1f,0.0f,0.0f,300,100);
}

void gimbal_task(void)
{
	target_angle=spring_motor.angle;
    target=GM6020_speedcontrol(PID_calc_GM6020_angle(&gimbal_pid_angle,gimbal_motor.angle,target_angle),&gimbal_motor,80.0f,1.21f);
	send_to_motors(target,0,0,0,0x1ff,&hcan1);
}

