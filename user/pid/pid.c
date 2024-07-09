#include "pid.h"

PID_TypeDef wendu;//温度PID
PID_TypeDef wendu_jiang;//温度PID
PID_TypeDef shidu;//湿度PID
PID_TypeDef turangshidu;//土壤湿度PID
PID_TypeDef shuiwei;//水位PID


uint8_t zidong_star = 0;// 全部自动控制PID的开关
#define PID_OUT_MAX 100
#define PID_OUT_MIN 0
/**
  * 函数功能: 初始化PID的目标值，比例项，积分项，微分项
  * 输入参数: PID_TARGET：目标值，PID_P：比例系数
             PID_I：积分系数　	，PID_D：微分系数
  * 返 回 值: 无
  * 说    明: 将PID的计算值初始化
  */
void PID_Init(PID_TypeDef *PID, int PID_Target, float PID_P, float PID_I, float PID_D)
{
    PID->SetPoint = PID_Target; // 目标速度
    PID->Proportion = PID_P;    // 比例系数
    PID->Integral = PID_I;      // 积分系数
    PID->Derivative = PID_D;    // 微分系数
}
/**
  * 函数功能: 初始化PID的目标值，比例项，积分项，微分项
  * 输入参数: PID_TARGET：目标值，PID_P：比例系数
             PID_I：积分系数　	，PID_D：微分系数
  * 返 回 值: 无
  * 说    明: 将PID的计算值初始化
  */
void PID_float_Init(PID_TypeDef *PID, float PID_Target, float PID_P, float PID_I, float PID_D)
{
    PID->SetPoint = PID_Target; // 目标速度
    PID->Proportion = PID_P;    // 比例系数
    PID->Integral = PID_I;      // 积分系数
    PID->Derivative = PID_D;    // 微分系数
}

	
/**
 * 函数功能: 位置式PID计算
 * 输入参数: PID_TypeDef* PID PID各式的参数 PID_Value 期望值
 * 返 回 值: 无
 * 说    明: 位置式式PID算法：u(k)=Kp*e(k) + Ki*Σe(k) + Kd*[e(k)-e(k-1)]
 */
int16_t PID_wendu(PID_TypeDef *PID, float PID_Value)
{
    PID->ThisError = PID->SetPoint - PID_Value;
    /*积分分离 -------------------------------------------------------------------*/
    if (PID->ThisError < -0.5 || PID->ThisError > 0.5)
		{
       PID->SumError += PID->ThisError;
		}
		else
		{
			PID->SumError = 0;
			return 0;
		}
    /*积分限幅 -------------------------------------------------------------------*/
    if ((PID->Integral != 0) && (PID->SumError > PID_OUT_MAX / PID->Integral))
        PID->SumError = PID_OUT_MAX / PID->Integral;
    if ((PID->Integral != 0) && (PID->SumError < PID_OUT_MIN / PID->Integral))
        PID->SumError = PID_OUT_MIN / PID->Integral;

    /*PID计算 --------------------------------------------------------------------*/
    PID->Result = PID->Proportion * PID->ThisError + PID->Integral * PID->SumError + PID->Derivative * (PID->ThisError - PID->LastError);
    /*误差保留 -------------------------------------------------------------------*/
    PID->LastError = PID->ThisError;
		
    /*输出限幅 -------------------------------------------------------------------*/
    if (PID->Result >= PID_OUT_MAX)
        PID->Result = PID_OUT_MAX;
    if (PID->Result <= PID_OUT_MIN)
        PID->Result = PID_OUT_MIN;
		 if (PID->ThisError > -0.5 && PID->ThisError < 0.5)
			 PID->Result=0;		
    return PID->Result;
}
/**
 * 函数功能: 位置式PID计算
 * 输入参数: PID_TypeDef* PID PID各式的参数 PID_Value 期望值
 * 返 回 值: 无
 * 说    明: 位置式式PID算法：u(k)=Kp*e(k) + Ki*Σe(k) + Kd*[e(k)-e(k-1)]
 */
int16_t PID_shidu(PID_TypeDef *PID, float PID_Value)
{
    PID->ThisError = PID->SetPoint - PID_Value;
    /*积分分离 -------------------------------------------------------------------*/
    if (PID->ThisError < -0.5 || PID->ThisError > 0.5)
		{
       PID->SumError += PID->ThisError;
		}
		else
		{
			PID->SumError = 0;
			return 0;
		}		
    /*积分限幅 -------------------------------------------------------------------*/
    if ((PID->Integral != 0) && (PID->SumError > PID_OUT_MAX / PID->Integral))
        PID->SumError = PID_OUT_MAX / PID->Integral;
    if ((PID->Integral != 0) && (PID->SumError < PID_OUT_MIN / PID->Integral))
        PID->SumError = PID_OUT_MIN / PID->Integral;

    /*PID计算 --------------------------------------------------------------------*/
    PID->Result = PID->Proportion * PID->ThisError + PID->Integral * PID->SumError + PID->Derivative * (PID->ThisError - PID->LastError);
    /*误差保留 -------------------------------------------------------------------*/
    PID->LastError = PID->ThisError;
		
    /*输出限幅 -------------------------------------------------------------------*/
    if (PID->Result >= PID_OUT_MAX)
        PID->Result = PID_OUT_MAX;
    if (PID->Result <= PID_OUT_MIN)
        PID->Result = PID_OUT_MIN;
		 if (PID->ThisError > -0.5 && PID->ThisError < 0.5)
			 PID->Result=0;		
    return PID->Result;
}


/**
 * 函数功能: 位置式PID计算
 * 输入参数: PID_TypeDef* PID PID各式的参数 PID_Value 期望值
 * 返 回 值: 无
 * 说    明: 位置式式PID算法：u(k)=Kp*e(k) + Ki*Σe(k) + Kd*[e(k)-e(k-1)]
 */
int16_t PID_turangshidu(PID_TypeDef *PID, float PID_Value)
{
    PID->ThisError = PID->SetPoint - PID_Value;
    /*积分分离 -------------------------------------------------------------------*/
    if (PID->ThisError < -2 || PID->ThisError > 2)
		{
       PID->SumError += PID->ThisError;
		}
		else
		{
			PID->SumError = 0;
			return 0;
		}
		
    /*积分限幅 -------------------------------------------------------------------*/
    if ((PID->Integral != 0) && (PID->SumError > PID_OUT_MAX / PID->Integral))
        PID->SumError = PID_OUT_MAX / PID->Integral;
    if ((PID->Integral != 0) && (PID->SumError < PID_OUT_MIN / PID->Integral))
        PID->SumError = PID_OUT_MIN / PID->Integral;

    /*PID计算 --------------------------------------------------------------------*/
    PID->Result = PID->Proportion * PID->ThisError + PID->Integral * PID->SumError + PID->Derivative * (PID->ThisError - PID->LastError);
    /*误差保留 -------------------------------------------------------------------*/
    PID->LastError = PID->ThisError;
		
    /*输出限幅 -------------------------------------------------------------------*/
    if (PID->Result >= PID_OUT_MAX)
        PID->Result = PID_OUT_MAX;
    if (PID->Result <= PID_OUT_MIN)
        PID->Result = PID_OUT_MIN;

    return PID->Result;
}



/**
 * 函数功能: 位置式PID计算
 * 输入参数: PID_TypeDef* PID PID各式的参数 PID_Value 期望值
 * 返 回 值: 无
 * 说    明: 位置式式PID算法：u(k)=Kp*e(k) + Ki*Σe(k) + Kd*[e(k)-e(k-1)]
 */
int16_t PID_shuiwei(PID_TypeDef *PID, float PID_Value)
{
    PID->ThisError = PID->SetPoint - PID_Value;
    /*积分分离 -------------------------------------------------------------------*/
    if (PID->ThisError < -0.5 || PID->ThisError > 0.5)
		{
       PID->SumError += PID->ThisError;
		}
		else
		{
			PID->SumError = 0;	
		}		
    /*积分限幅 -------------------------------------------------------------------*/
    if ((PID->Integral != 0) && (PID->SumError > PID_OUT_MAX / PID->Integral))
        PID->SumError = PID_OUT_MAX / PID->Integral;
    if ((PID->Integral != 0) && (PID->SumError < PID_OUT_MIN / PID->Integral))
        PID->SumError = PID_OUT_MIN / PID->Integral;

    /*PID计算 --------------------------------------------------------------------*/
    PID->Result = PID->Proportion * PID->ThisError + PID->Integral * PID->SumError + PID->Derivative * (PID->ThisError - PID->LastError);
    /*误差保留 -------------------------------------------------------------------*/
    PID->LastError = PID->ThisError;
		
    /*输出限幅 -------------------------------------------------------------------*/
    if (PID->Result >= PID_OUT_MAX)
        PID->Result = PID_OUT_MAX;
    if (PID->Result <= PID_OUT_MIN)
        PID->Result = PID_OUT_MIN;
		if (PID->ThisError > -0.5 && PID->ThisError < 0.5)
		{
			 PID->Result=0;
		}
    return PID->Result;
}




/**
 * 函数功能: 位置式PID计算
 * 输入参数: PID_TypeDef* PID PID各式的参数 PID_Value 期望值
 * 返 回 值: 无
 * 说    明: 位置式式PID算法：u(k)=Kp*e(k) + Ki*Σe(k) + Kd*[e(k)-e(k-1)]
 */
int16_t PID_wendu_tongfeng(PID_TypeDef *PID, float PID_Value)
{
    PID->ThisError = PID_Value - PID->SetPoint;
    /*积分分离 -------------------------------------------------------------------*/
    if (PID->ThisError > 0.5)
		{
       PID->SumError += PID->ThisError;
		}		
		else
		{
			PID->SumError = 0;
			return 0;
		}
    /*积分限幅 -------------------------------------------------------------------*/
    if ((PID->Integral != 0) && (PID->SumError > PID_OUT_MAX / PID->Integral))
        PID->SumError = PID_OUT_MAX / PID->Integral;
    if ((PID->Integral != 0) && (PID->SumError < PID_OUT_MIN / PID->Integral))
        PID->SumError = PID_OUT_MIN / PID->Integral;

    /*PID计算 --------------------------------------------------------------------*/
    PID->Result = PID->Proportion * PID->ThisError + PID->Integral * PID->SumError + PID->Derivative * (PID->ThisError - PID->LastError);
    /*误差保留 -------------------------------------------------------------------*/
    PID->LastError = PID->ThisError;
		
    /*输出限幅 -------------------------------------------------------------------*/
    if (PID->Result >= PID_OUT_MAX)
        PID->Result = PID_OUT_MAX;
    if (PID->Result <= PID_OUT_MIN)
        PID->Result = PID_OUT_MIN;
		if (PID->ThisError > -0.5 && PID->ThisError < 0.5)
			 PID->Result=0;		
    return PID->Result;
}
