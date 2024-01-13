#include "RMmotor.hpp"
/**
 * @brief 接收can数据到该RM电机对象中，直接放在can的中断回调函数中使用
 * @param receivedID 该报文的ID
 * @param data 该报文数据的指针
*/
void RMmotor::receive_data(uint16_t receivedID,uint8_t* data){
    if(receivedID==ID){
        angle=(((int16_t)data[0])<<8)+data[1];
        speed=(int16_t)((((int16_t)data[2])<<8)+data[3]);
        torque=(int16_t)((((int16_t)data[4])<<8)+data[5]);
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
 * @retval 返回计算得到的电流值或电压值
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

/**
 * @brief 发送can报文给4个电机(大疆RM电机的控制)
 * @param motor1 第一个电机要输出的电流或电压(m3508电流，gm6020电压)
 * @param motor2 第二个电机要输出的电流或电压(m3508电流，gm6020电压)
 * @param motor3 第三个电机要输出的电流或电压(m3508电流，gm6020电压)
 * @param motor4 第四个电机要输出的电流或电压(m3508电流，gm6020电压)
 * @param lable 报文ID或者说标识符
 * @param hcan_ 选择的can类型，这里是句柄类型的指针
*/
void send_to_motors(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4,uint16_t lable,CAN_HandleTypeDef* hcan_){
        uint8_t data[8];
        data[0]=(motor1>>8);
        data[1]=motor1;
        data[2]=(motor2>>8);
        data[3]=motor2;
        data[4]=(motor3>>8);
        data[5]=motor3;
        data[6]=(motor4>>8);
        data[7]=motor4;
        CAN_SEND_Data(hcan_,lable,data,8);
}
