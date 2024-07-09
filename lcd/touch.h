#ifndef _touch_H
#define _touch_H


#include "tftlcd.h"




#define TP_PRES_DOWN 0x80  //����������	  
#define TP_CATH_PRES 0x40  //�а��������� 
#define CT_MAX_TOUCH 10    //������֧�ֵĵ���,�̶�Ϊ5��

//������������
typedef struct
{
	u8 (*init)(void);			//��ʼ��������������
	u8 (*scan)(u8);				//ɨ�败����.0,��Ļɨ��;1,��������;	 
	void (*adjust)(void);		//������У׼ 
	u16 x[CT_MAX_TOUCH]; 		//��ǰ����
	u16 y[CT_MAX_TOUCH];		//�����������10������,����������x[0],y[0]����:�˴�ɨ��ʱ,����������,��
								//x[9],y[9]�洢��һ�ΰ���ʱ������. 
	u16 sta;					//�ʵ�״̬ 
								//b15:����1/�ɿ�0; 
	                            //b14:0,û�а�������;1,�а�������. 
								//b13~b10:����
								//b9~b0:���ݴ��������µĵ���(0,��ʾδ����,1��ʾ����)
/////////////////////������У׼����(����������ҪУ׼)//////////////////////								
	float xfac;					
	float yfac;
	short xoff;
	short yoff;	   
//�����Ĳ���,��������������������ȫ�ߵ�ʱ��Ҫ�õ�.
//b0:0,����(�ʺ�����ΪX����,����ΪY�����TP)
//   1,����(�ʺ�����ΪY����,����ΪX�����TP) 
//b1~6:����.
//b7:0,������
//   1,������ 
	u8 touchtype;
}_m_tp_dev;

extern _m_tp_dev tp_dev;	 	//������������touch.c���涨��

//������оƬ��������	   
//#define PEN  		PBin(1)  	//T_PEN
//#define DOUT 		PBin(2)   	//T_MISO
//#define TDIN 		PFout(11)  	//T_MOSI
//#define TCLK 		PBout(0)  	//T_SCK
//#define TCS  		PCout(13)  	//T_CS  

/*XPT2046
PB0  SCLK==DCLK
PC13 CS
PF11 MOSI==DIN
PB2  MISO==TDOUT
PB1  TPEN
*/
//SPI��д����
#define	TCLK_0	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0, GPIO_PIN_RESET)
#define TCLK_1	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0, GPIO_PIN_SET)
#define TCS_0		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13, GPIO_PIN_RESET)
#define	TCS_1		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13, GPIO_PIN_SET)
#define	TDIN_0	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_11, GPIO_PIN_RESET)
#define	TDIN_1	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_11, GPIO_PIN_SET)
#define	DOUT		HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)
#define	PEN		HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)




void delayus(uint8_t x);//����������
void TP_Write_Byte(u8 num);						//�����оƬд��һ������
u16 TP_Read_AD(u8 CMD);							//��ȡADת��ֵ
u16 TP_Read_XOY(u8 xy);							//���˲��������ȡ(X/Y)
u8 TP_Read_XY(u16 *x,u16 *y);					//˫�����ȡ(X+Y)
u8 TP_Read_XY2(u16 *x,u16 *y);					//����ǿ�˲���˫���������ȡ
void TP_Drow_Touch_Point(u16 x,u16 y,u16 color);//��һ������У׼��
void TP_Draw_Big_Point(u16 x,u16 y,u16 color);	//��һ�����
void TP_Save_Adjdata(void);						//����У׼����
u8 TP_Get_Adjdata(void);						//��ȡУ׼����
void Touch_Adjust(void);							//������У׼
void TP_Adj_Info_Show(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2,u16 x3,u16 y3,u16 fac);//��ʾУ׼��Ϣ
//������/������ ���ú���
u8 TP_Scan(u8 tp);								//ɨ��
u8 TOUCH_init(void);								//��ʼ��
//����������
u8 CST716_Init(void);
u8 CST716_Scan(u8 mode);
#endif
