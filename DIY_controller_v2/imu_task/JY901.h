#ifndef JY901_H
#define JY901_H

#ifdef JY01A
extern "C" {
#endif

#include "main.h"

#define RXBUFFER_LEN 33		//接收9类数据，一共99位
typedef struct
{
	float angle[3];
}Angle;

typedef struct
{
	float a[3];
}Acc;

typedef struct
{
	float w[3];
}SGyro;


typedef struct//四元数
{ float q[4];
}SQ;

typedef struct//磁场输出
{
	float h[3];
}SMag;

typedef struct//气压高度
{
	float lPressure;
	float lAltitude;
}SPress;

typedef struct//经纬度
{
	float lLon;
	float lLat;
}SLonLat;
struct STime
{
	unsigned char ucYear;
	unsigned char ucMonth;
	unsigned char ucDay;
	unsigned char ucHour;
	unsigned char ucMinute;
	unsigned char ucSecond;
	unsigned short usMiliSecond;
};
struct SAcc//加速度
{
	short a[3];
	short T;
};
struct SGyro//角速度
{
	short w[3];
	short T;
};
struct SAngle//角度
{
	short Angle[3];
	short T;
};
struct SMag//磁场输出
{
	short h[3];
	short T;
};

struct SDStatus//端口状态数据输出
{
	short sDStatus[4];
};

struct SPress//气压高度
{
	long lPressure;
	long lAltitude;
};

struct SLonLat//经纬度
{
	long lLon;
	long lLat;
};

struct SGPSV
{
	short sGPSHeight;
	short sGPSYaw;
	long lGPSVelocity;
};
struct SQ //四元数
{ short q[4];
};


typedef struct User_USART
{
		uint8_t Rx_flag;											
		uint8_t Rx_len;												
		uint8_t frame_head;					//帧头
		uint8_t RxBuffer[RXBUFFER_LEN];		//接收缓冲
		Angle angle;						//角度
		Acc acc;								//加速度
		SGyro w;								//角速度
		SMag h;									//磁场
		SPress lPressure;   	  //气压
		SPress lAltitude;     	//高度
		SLonLat lLon;						//经度
		SLonLat lLat;						//维度
		SQ q; 									//四元数
}User_USART;

void User_USART_Init(User_USART *Data);
void JY901_Process();

extern User_USART JY901_data;
#ifdef JY01A
}
#endif

#endif /* __MAIN_H */

