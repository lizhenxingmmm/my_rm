#ifndef MY_CAN_H_
#define MY_CAN_H_



#include "main.h"
#include "can.h"


#define CAN_FILTER(x)((x)<<3)

#define CAN_FIFO_0 (0<<2)
#define CAN_FIFO_1 (1<<2)

#define CAN_STDID (0<<1)
#define CAN_EXTID (1<<1)

#define CAN_DATA_TYPE (0<<0)
#define CAN_REMOTE_TYPE (1<<0)



void CAN_Init(CAN_HandleTypeDef *hcan);
uint8_t CAN_SEND_Data(CAN_HandleTypeDef *hcan,uint16_t ID,uint8_t *DATA,uint16_t Length);
void CAN_Filter_Mask_Config(CAN_HandleTypeDef *hcan,uint8_t object_param,uint32_t ID,uint32_t mask_ID);

#endif





