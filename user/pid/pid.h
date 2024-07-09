#ifndef __PID_H__
#define __PID_H__

/* 包含头文件 -------------------------------------------------------------------------------*/
#include "main.h"

/* 宏定义 -----------------------------------------------------------------------------------*/
#define ROUND_TO_INT16(x) ((int16_t)(x) + 0.5f) >= (x) ? ((int16_t)(x) + 1) : ((int16_t)(x))

/* 结构体定义 -------------------------------------------------------------------------------*/
typedef struct
{
    __IO float SetPoint; 						// 设定目标 Desired Value
    __IO float SumError; 						// 误差累计
    __IO float Result;							//产生PWM的值
    __IO float Proportion; 			    // 比例常数 Proportional Const
    __IO float Integral;   			    // 积分常数 Integral Const
    __IO float Derivative; 			    // 微分常数 Derivative Const
    __IO float ThisError;  			    // 本次偏差
    __IO float LastError;  			    // 上次偏差
    __IO float PrevError;  			    // 上上次偏差
    __IO float Position_Bias;
		__IO float Increase;
		__IO float flag;
} PID_TypeDef;

/* 变量定义 ---------------------------------------------------------------------------------*/
extern PID_TypeDef wendu;//温度PID
extern PID_TypeDef wendu_jiang;//温度PID
extern PID_TypeDef shidu;//湿度PID
extern PID_TypeDef turangshidu;//土壤湿度PID
extern PID_TypeDef shuiwei;//水位PID
extern uint8_t zidong_star;// 全部自动控制PID的开关

/* 函数声明 ---------------------------------------------------------------------------------*/
void 					 PID_float_Init(PID_TypeDef *PID, float PID_Target, float PID_P, float PID_I, float PID_D);
void 				   PID_Init(PID_TypeDef *PID, int PID_Target, float PID_P, float PID_I, float PID_D);
int16_t 			 PID_wendu(PID_TypeDef *PID, float PID_Value);
int16_t 			 PID_shidu(PID_TypeDef *PID, float PID_Value);
int16_t 			 PID_turangshidu(PID_TypeDef *PID, float PID_Value);
int16_t 			 PID_shuiwei(PID_TypeDef *PID, float PID_Value);
int16_t PID_wendu_tongfeng(PID_TypeDef *PID, float PID_Value);
#endif		     
