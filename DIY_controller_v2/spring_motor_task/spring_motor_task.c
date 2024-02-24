#include "spring_motor_task.h"
#include "RMmotor.h"
#include "math.h"
#include "pid.h"

RMmotor spring_motor;
RMmotor spring_motor_2;

float neutral_angle;
float elastic_force;
float resistance;//阻力,不是电阻

float neutral_angle_2;
float elastic_force_2;
float resistance_2;//阻力,不是电阻

float motor1;
float motor2;

pid_type_def pid_spring_motor1;
pid_type_def pid_spring_motor1_speed;
pid_type_def pid_spring_motor2;
pid_type_def pid_spring_motor2_speed;
int motor1_speed;
int motor2_speed;
void spring_motor_task_init()
{
    neutral_angle=3850;
    neutral_angle_2=4850;
    //113刚好抵消电机本身阻力(不同电机不同参数,最好分别调,换电池可能也有影响)
    resistance=-250;
    resistance_2=-250;
    CAN_Init(&hcan1);
    RMmotor_init(&spring_motor,0x200,0x202);
    RMmotor_init(&spring_motor_2,0x200,0x201);

    PID_init(&pid_spring_motor1,PID_POSITION,1.f,0.f,10.f,10000,3000);
    PID_init(&pid_spring_motor1_speed,PID_POSITION,30.f,0.1f,0,10000,3000);
    PID_init(&pid_spring_motor2,PID_POSITION,1.f,0.f,10.f,10000,3000);
    PID_init(&pid_spring_motor2_speed,PID_POSITION,50.f,0.5f,0,10000,3000);
}

void spring_motor_task()
{
    motor1=PID_calc_GM6020_angle(&pid_spring_motor1,spring_motor.angle,3923);
    motor2=PID_calc_GM6020_angle(&pid_spring_motor2,spring_motor_2.angle,3750);
    motor1_speed = PID_calc(&pid_spring_motor1_speed,-spring_motor.speed,motor1);
    motor2_speed = PID_calc(&pid_spring_motor2_speed,spring_motor_2.speed,motor2);
    send_to_motors(motor2_speed,motor1_speed,0,0,0x200,&hcan1);
}

