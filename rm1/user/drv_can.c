/*
初始化：
CAN_Init(要用can几)
发数据
CAN_SEND_Data(&can几，报文ID，数据指针，长度)；
收信息
配掩码，fifo中断接收数据
*/
#include "drv_can.h"



void CAN_Init(CAN_HandleTypeDef *hcan){
HAL_CAN_Start (hcan );
__HAL_CAN_ENABLE_IT (hcan,CAN_IT_RX_FIFO0_MSG_PENDING);
__HAL_CAN_ENABLE_IT (hcan,CAN_IT_RX_FIFO1_MSG_PENDING);
if (hcan->Instance == CAN1)
    {
        
        CAN_Filter_Mask_Config(hcan, CAN_FILTER(0) | CAN_FIFO_0 | CAN_STDID | CAN_DATA_TYPE, 0, 0);
        CAN_Filter_Mask_Config(hcan, CAN_FILTER(1) | CAN_FIFO_1 | CAN_STDID | CAN_DATA_TYPE, 0, 0);
    }
    else if (hcan->Instance == CAN2)
    {
        
        CAN_Filter_Mask_Config(hcan, CAN_FILTER(14) | CAN_FIFO_0 | CAN_STDID | CAN_DATA_TYPE, 0, 0);
        CAN_Filter_Mask_Config(hcan, CAN_FILTER(15) | CAN_FIFO_1 | CAN_STDID | CAN_DATA_TYPE, 0, 0);
    }
}
/////////////////////////////////////////////////////////////
uint8_t CAN_SEND_Data(CAN_HandleTypeDef *hcan,uint16_t ID,uint8_t *DATA,uint16_t Length){
CAN_TxHeaderTypeDef tx_header;
	uint32_t used_mailbox;
	assert_param (hcan!=NULL);
	
	tx_header.StdId =ID;											//DATA is a pointer
	tx_header.ExtId =0;
	tx_header.IDE =0;
	tx_header.RTR =0;
	tx_header.DLC =Length;
	return(HAL_CAN_AddTxMessage (hcan ,&tx_header ,DATA ,&used_mailbox));
}
/////////////////////////////////////////////////////////////////////////////
void CAN_Filter_Mask_Config(CAN_HandleTypeDef *hcan,uint8_t object_param,uint32_t ID,uint32_t mask_ID){
	CAN_FilterTypeDef can_filter_init;
	assert_param (hcan!=NULL);
	if((object_param &0x02)){
	 can_filter_init.FilterIdHigh =ID<<3<<16;																				//I cant understand the code.Just copy;
	 can_filter_init.FilterIdLow =ID<<3|((object_param &0x03))<<1;
		can_filter_init.FilterMaskIdHigh =mask_ID <<3<<16;
		can_filter_init.FilterMaskIdLow =mask_ID<<3|((object_param &0x03))<<1; 
	}
	else{
		can_filter_init.FilterIdHigh =ID<<5;																				//I cant understand the code.Just copy;
	  can_filter_init.FilterIdLow =ID<<3|((object_param &0x03))<<1;
		can_filter_init.FilterMaskIdHigh =mask_ID <<5;
		can_filter_init.FilterMaskIdLow =mask_ID<<3|((object_param &0x03))<<1; 
	}
	
	can_filter_init.FilterBank =object_param >>3;
	can_filter_init.FilterFIFOAssignment=((object_param >>2)&0x01);
	can_filter_init.FilterActivation =ENABLE ;
	can_filter_init.FilterMode =CAN_FILTERMODE_IDMASK;
	can_filter_init.FilterScale =CAN_FILTERSCALE_32BIT;
	can_filter_init.SlaveStartFilterBank =14;
	HAL_CAN_ConfigFilter (hcan,&can_filter_init );
}

//void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
//		CAN_RxHeaderTypeDef header;
//		uint8_t data[8];
//		HAL_CAN_GetRxMessage(hcan,CAN_FILTER_FIFO0,&header ,data);
//}
// void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan){
// 	CAN_RxHeaderTypeDef header;
// 	uint8_t can_data[8];
// 	HAL_CAN_GetRxMessage(hcan,CAN_FILTER_FIFO1 ,&header ,can_data);
// }
