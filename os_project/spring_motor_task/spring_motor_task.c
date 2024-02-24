#include "spring_motor_task.h"
#include "RMmotor.h"
#include "math.h"

RMmotor spring_motor;
RMmotor spring_motor_2;

float neutral_angle;
float elastic_force;
float resistance;//阻力,不是电阻

float neutral_angle_2;
float elastic_force_2;
float resistance_2;//阻力,不是电阻

void spring_motor_task_init()
{
    neutral_angle=4096;
    neutral_angle_2=4096;
    //113刚好抵消电机本身阻力(不同电机不同参数,最好分别调,换电池可能也有影响)
    resistance=-250;
    resistance_2=-250;
    CAN_Init(&hcan1);
    RMmotor_init(&spring_motor,0x200,0x202);
    RMmotor_init(&spring_motor_2,0x200,0x201);
}

void spring_motor_task()
{
    if((neutral_angle-spring_motor.angle)>0)
    {
        elastic_force=700*sqrt(((double)(neutral_angle-spring_motor.angle))/1000);
    }
    else
    {
        elastic_force=-700*sqrt(((double)(spring_motor.angle-neutral_angle))/1000);
    }
    if(elastic_force>0)
    {
        elastic_force+=resistance;
    }
    else if(elastic_force<0)
    {
        elastic_force-=resistance;
    }
//-----------------------------------------------------------------------
    if((neutral_angle_2-spring_motor_2.angle)>0)
    {
        elastic_force_2=700*sqrt(((double)(neutral_angle_2-spring_motor_2.angle))/1000);
    }
    else
    {
        elastic_force_2=-700*sqrt(((double)(spring_motor_2.angle-neutral_angle_2))/1000);
    }
    if(elastic_force_2>0)
    {
        elastic_force_2+=resistance_2;
    }
    else if(elastic_force_2<0)
    {
        elastic_force_2-=resistance_2;
    }
    send_to_motors(elastic_force_2,elastic_force,0,0,0x200,&hcan1);
}

