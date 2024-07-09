#include "pid.h"

PID_TypeDef wendu;//�¶�PID
PID_TypeDef wendu_jiang;//�¶�PID
PID_TypeDef shidu;//ʪ��PID
PID_TypeDef turangshidu;//����ʪ��PID
PID_TypeDef shuiwei;//ˮλPID


uint8_t zidong_star = 0;// ȫ���Զ�����PID�Ŀ���
#define PID_OUT_MAX 100
#define PID_OUT_MIN 0
/**
  * ��������: ��ʼ��PID��Ŀ��ֵ������������΢����
  * �������: PID_TARGET��Ŀ��ֵ��PID_P������ϵ��
             PID_I������ϵ����	��PID_D��΢��ϵ��
  * �� �� ֵ: ��
  * ˵    ��: ��PID�ļ���ֵ��ʼ��
  */
void PID_Init(PID_TypeDef *PID, int PID_Target, float PID_P, float PID_I, float PID_D)
{
    PID->SetPoint = PID_Target; // Ŀ���ٶ�
    PID->Proportion = PID_P;    // ����ϵ��
    PID->Integral = PID_I;      // ����ϵ��
    PID->Derivative = PID_D;    // ΢��ϵ��
}
/**
  * ��������: ��ʼ��PID��Ŀ��ֵ������������΢����
  * �������: PID_TARGET��Ŀ��ֵ��PID_P������ϵ��
             PID_I������ϵ����	��PID_D��΢��ϵ��
  * �� �� ֵ: ��
  * ˵    ��: ��PID�ļ���ֵ��ʼ��
  */
void PID_float_Init(PID_TypeDef *PID, float PID_Target, float PID_P, float PID_I, float PID_D)
{
    PID->SetPoint = PID_Target; // Ŀ���ٶ�
    PID->Proportion = PID_P;    // ����ϵ��
    PID->Integral = PID_I;      // ����ϵ��
    PID->Derivative = PID_D;    // ΢��ϵ��
}

	
/**
 * ��������: λ��ʽPID����
 * �������: PID_TypeDef* PID PID��ʽ�Ĳ��� PID_Value ����ֵ
 * �� �� ֵ: ��
 * ˵    ��: λ��ʽʽPID�㷨��u(k)=Kp*e(k) + Ki*��e(k) + Kd*[e(k)-e(k-1)]
 */
int16_t PID_wendu(PID_TypeDef *PID, float PID_Value)
{
    PID->ThisError = PID->SetPoint - PID_Value;
    /*���ַ��� -------------------------------------------------------------------*/
    if (PID->ThisError < -0.5 || PID->ThisError > 0.5)
		{
       PID->SumError += PID->ThisError;
		}
		else
		{
			PID->SumError = 0;
			return 0;
		}
    /*�����޷� -------------------------------------------------------------------*/
    if ((PID->Integral != 0) && (PID->SumError > PID_OUT_MAX / PID->Integral))
        PID->SumError = PID_OUT_MAX / PID->Integral;
    if ((PID->Integral != 0) && (PID->SumError < PID_OUT_MIN / PID->Integral))
        PID->SumError = PID_OUT_MIN / PID->Integral;

    /*PID���� --------------------------------------------------------------------*/
    PID->Result = PID->Proportion * PID->ThisError + PID->Integral * PID->SumError + PID->Derivative * (PID->ThisError - PID->LastError);
    /*���� -------------------------------------------------------------------*/
    PID->LastError = PID->ThisError;
		
    /*����޷� -------------------------------------------------------------------*/
    if (PID->Result >= PID_OUT_MAX)
        PID->Result = PID_OUT_MAX;
    if (PID->Result <= PID_OUT_MIN)
        PID->Result = PID_OUT_MIN;
		 if (PID->ThisError > -0.5 && PID->ThisError < 0.5)
			 PID->Result=0;		
    return PID->Result;
}
/**
 * ��������: λ��ʽPID����
 * �������: PID_TypeDef* PID PID��ʽ�Ĳ��� PID_Value ����ֵ
 * �� �� ֵ: ��
 * ˵    ��: λ��ʽʽPID�㷨��u(k)=Kp*e(k) + Ki*��e(k) + Kd*[e(k)-e(k-1)]
 */
int16_t PID_shidu(PID_TypeDef *PID, float PID_Value)
{
    PID->ThisError = PID->SetPoint - PID_Value;
    /*���ַ��� -------------------------------------------------------------------*/
    if (PID->ThisError < -0.5 || PID->ThisError > 0.5)
		{
       PID->SumError += PID->ThisError;
		}
		else
		{
			PID->SumError = 0;
			return 0;
		}		
    /*�����޷� -------------------------------------------------------------------*/
    if ((PID->Integral != 0) && (PID->SumError > PID_OUT_MAX / PID->Integral))
        PID->SumError = PID_OUT_MAX / PID->Integral;
    if ((PID->Integral != 0) && (PID->SumError < PID_OUT_MIN / PID->Integral))
        PID->SumError = PID_OUT_MIN / PID->Integral;

    /*PID���� --------------------------------------------------------------------*/
    PID->Result = PID->Proportion * PID->ThisError + PID->Integral * PID->SumError + PID->Derivative * (PID->ThisError - PID->LastError);
    /*���� -------------------------------------------------------------------*/
    PID->LastError = PID->ThisError;
		
    /*����޷� -------------------------------------------------------------------*/
    if (PID->Result >= PID_OUT_MAX)
        PID->Result = PID_OUT_MAX;
    if (PID->Result <= PID_OUT_MIN)
        PID->Result = PID_OUT_MIN;
		 if (PID->ThisError > -0.5 && PID->ThisError < 0.5)
			 PID->Result=0;		
    return PID->Result;
}


/**
 * ��������: λ��ʽPID����
 * �������: PID_TypeDef* PID PID��ʽ�Ĳ��� PID_Value ����ֵ
 * �� �� ֵ: ��
 * ˵    ��: λ��ʽʽPID�㷨��u(k)=Kp*e(k) + Ki*��e(k) + Kd*[e(k)-e(k-1)]
 */
int16_t PID_turangshidu(PID_TypeDef *PID, float PID_Value)
{
    PID->ThisError = PID->SetPoint - PID_Value;
    /*���ַ��� -------------------------------------------------------------------*/
    if (PID->ThisError < -2 || PID->ThisError > 2)
		{
       PID->SumError += PID->ThisError;
		}
		else
		{
			PID->SumError = 0;
			return 0;
		}
		
    /*�����޷� -------------------------------------------------------------------*/
    if ((PID->Integral != 0) && (PID->SumError > PID_OUT_MAX / PID->Integral))
        PID->SumError = PID_OUT_MAX / PID->Integral;
    if ((PID->Integral != 0) && (PID->SumError < PID_OUT_MIN / PID->Integral))
        PID->SumError = PID_OUT_MIN / PID->Integral;

    /*PID���� --------------------------------------------------------------------*/
    PID->Result = PID->Proportion * PID->ThisError + PID->Integral * PID->SumError + PID->Derivative * (PID->ThisError - PID->LastError);
    /*���� -------------------------------------------------------------------*/
    PID->LastError = PID->ThisError;
		
    /*����޷� -------------------------------------------------------------------*/
    if (PID->Result >= PID_OUT_MAX)
        PID->Result = PID_OUT_MAX;
    if (PID->Result <= PID_OUT_MIN)
        PID->Result = PID_OUT_MIN;

    return PID->Result;
}



/**
 * ��������: λ��ʽPID����
 * �������: PID_TypeDef* PID PID��ʽ�Ĳ��� PID_Value ����ֵ
 * �� �� ֵ: ��
 * ˵    ��: λ��ʽʽPID�㷨��u(k)=Kp*e(k) + Ki*��e(k) + Kd*[e(k)-e(k-1)]
 */
int16_t PID_shuiwei(PID_TypeDef *PID, float PID_Value)
{
    PID->ThisError = PID->SetPoint - PID_Value;
    /*���ַ��� -------------------------------------------------------------------*/
    if (PID->ThisError < -0.5 || PID->ThisError > 0.5)
		{
       PID->SumError += PID->ThisError;
		}
		else
		{
			PID->SumError = 0;	
		}		
    /*�����޷� -------------------------------------------------------------------*/
    if ((PID->Integral != 0) && (PID->SumError > PID_OUT_MAX / PID->Integral))
        PID->SumError = PID_OUT_MAX / PID->Integral;
    if ((PID->Integral != 0) && (PID->SumError < PID_OUT_MIN / PID->Integral))
        PID->SumError = PID_OUT_MIN / PID->Integral;

    /*PID���� --------------------------------------------------------------------*/
    PID->Result = PID->Proportion * PID->ThisError + PID->Integral * PID->SumError + PID->Derivative * (PID->ThisError - PID->LastError);
    /*���� -------------------------------------------------------------------*/
    PID->LastError = PID->ThisError;
		
    /*����޷� -------------------------------------------------------------------*/
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
 * ��������: λ��ʽPID����
 * �������: PID_TypeDef* PID PID��ʽ�Ĳ��� PID_Value ����ֵ
 * �� �� ֵ: ��
 * ˵    ��: λ��ʽʽPID�㷨��u(k)=Kp*e(k) + Ki*��e(k) + Kd*[e(k)-e(k-1)]
 */
int16_t PID_wendu_tongfeng(PID_TypeDef *PID, float PID_Value)
{
    PID->ThisError = PID_Value - PID->SetPoint;
    /*���ַ��� -------------------------------------------------------------------*/
    if (PID->ThisError > 0.5)
		{
       PID->SumError += PID->ThisError;
		}		
		else
		{
			PID->SumError = 0;
			return 0;
		}
    /*�����޷� -------------------------------------------------------------------*/
    if ((PID->Integral != 0) && (PID->SumError > PID_OUT_MAX / PID->Integral))
        PID->SumError = PID_OUT_MAX / PID->Integral;
    if ((PID->Integral != 0) && (PID->SumError < PID_OUT_MIN / PID->Integral))
        PID->SumError = PID_OUT_MIN / PID->Integral;

    /*PID���� --------------------------------------------------------------------*/
    PID->Result = PID->Proportion * PID->ThisError + PID->Integral * PID->SumError + PID->Derivative * (PID->ThisError - PID->LastError);
    /*���� -------------------------------------------------------------------*/
    PID->LastError = PID->ThisError;
		
    /*����޷� -------------------------------------------------------------------*/
    if (PID->Result >= PID_OUT_MAX)
        PID->Result = PID_OUT_MAX;
    if (PID->Result <= PID_OUT_MIN)
        PID->Result = PID_OUT_MIN;
		if (PID->ThisError > -0.5 && PID->ThisError < 0.5)
			 PID->Result=0;		
    return PID->Result;
}
