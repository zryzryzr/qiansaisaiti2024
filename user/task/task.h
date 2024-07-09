#ifndef _TASK_H_
#define _TASK_H_
#include "main.h"
#include "font_show.h"
/*�˽ṹ���Ǹ����������Ŀ��ر�־λ*/
typedef struct KAIGUAN_FLAG
{
	uint8_t	choushui;
	uint8_t jiashi;
	uint8_t deng;
	uint8_t fengshan;
	uint8_t fengshan_jiare;	
	uint8_t jiaoshui;
}on_off;

typedef struct SET_DATA
{
	float	soil_humidity;
	float water_level;
	float Humidity;
	float Temperature;
}setdata;
extern on_off shuzhi;
extern setdata setting;
void Now_Data(void);
void Set_Data(void);
void KaiGuan(void);
#endif

