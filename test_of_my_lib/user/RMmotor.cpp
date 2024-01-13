#include "RMmotor.hpp"
/**
 * @brief 接收can数据到该RM电机对象中，直接放在can的中断回调函数中使用
 * @param receivedID 该报文的ID
 * @param data 该报文数据的指针
*/
void RMmotor::receive_data(uint8_t receivedID,uint8_t* data){
    if(receivedID==ID){
        angle=(((int16_t)data[0])<<8)+data[1];
        speed=(((int16_t)data[2])<<8)+data[3];
        torque=(((int16_t)data[4])<<8)+data[5];
        temperature=data[7];
    }
}
/**
 * @brief 速度环pid计算
 * @param target 目标转速
 * @param feedback 反馈转速
 * @param P 比例系数
 * @param I 积分系数
 * @param D 微分(差分)系数
 * @param i_max 积分输出最大值
 * @param total_max 总输出最大值
 * @param mode 模式 0:积分限幅 1:积分分离 2:微分先行
 * @retval 返回计算得到的扭矩值
*/
float RMmotor::pid_calcu_speed(float target,float feedback,float P,float I,float D,float i_max,float total_max,int mode){
    pid_error_speed=target-feedback;
    if(mode==0){
        P_out=P*pid_error_speed;
        I_out+=I*pid_error_speed;
        if(I_out>=i_max){
            I_out=i_max;
        }
        D_out=D*pid_last_error_speed;
    }
    pid_last_error_speed=pid_error_speed;
    if(total_max<(P_out+I_out+D_out)){
        return total_max;
    }
    return (P_out+I_out+D_out); 
}

void send_to_motors(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4,uint8_t lable,CAN_HandleTypeDef* hcan_){
        int16_t A=10000;
        int16_t B=0;
        int16_t C=0;
        int16_t D=0;
        uint8_t data[8];
        
        data[0]=(A>>8);
        data[1]=A;
        data[2]=(D>>8);
        data[3]=D;
        data[4]=(C>>8);
        data[5]=C;
        data[6]=(B>>8);
        data[7]=B;
        CAN_SEND_Data(&hcan1,0x1ff,data,8);
}
