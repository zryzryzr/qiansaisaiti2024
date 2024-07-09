#include "font_update.h"
  
#include "flash.h"  
#include "tftlcd.h"  
#include "string.h"



								  
//�ֿ�����ռ�õ�����������С(3���ֿ�+unigbk��+�ֿ���Ϣ=3238700�ֽ�,Լռ791��W25QXX����)
#define FONTSECSIZE	 	791
//�ֿ�����ʼ��ַ 
#define FONTINFOADDR 	1024*1024*12 					//��12M��ַ�Ժ�ʼ����ֿ�
														//ǰ��12M��fatfsռ����.
														//12M�Ժ����3���ֿ�+UNIGBK.BIN,�ܴ�С3.09M,���ֿ�ռ����,���ܶ�!
														//15.10M�Ժ�,�û���������ʹ��.����������100K�ֽڱȽϺ�.
														//ǰ��4M��fatfsռ����.
														
														
//���������ֿ������Ϣ����ַ����С��
_font_info ftinfo;

//�ֿ����ڴ����е�·��
u8*const GBK_PATH[5]=
{
"/FONT/UNIGBK.BIN",	//UNIGBK.BIN�Ĵ��λ��
"/FONT/GBK12.FON",	//GBK12�Ĵ��λ��
"/FONT/GBK16.FON",	//GBK16�Ĵ��λ��
"/FONT/GBK24.FON",	//GBK24�Ĵ��λ��
"/FONT/GBK32.FON",	//GBK32�Ĵ��λ��
}; 
//����ʱ����ʾ��Ϣ
u8*const UPDATE_REMIND_TBL[5]=
{
"Updating UNIGBK.BIN",	//��ʾ���ڸ���UNIGBK.bin
"Updating GBK12.FON ",	//��ʾ���ڸ���GBK12
"Updating GBK16.FON ",	//��ʾ���ڸ���GBK16
"Updating GBK24.FON ",	//��ʾ���ڸ���GBK24
"Updating GBK32.FON ",	//��ʾ���ڸ���GBK32
}; 




//��ʼ������
//����ֵ:0,�ֿ����.
//		 ����,�ֿⶪʧ
u8 font_init(void)
{		
	u8 t=0;
//	EN25QXX_Init();  
	while(t<10)//������ȡ10��,���Ǵ���,˵��ȷʵ��������,�ø����ֿ���
	{
		t++;
		EN25QXX_Read((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));//����ftinfo�ṹ������
		if(ftinfo.fontok==0XAA)break;
		HAL_Delay(20);
	}
	if(ftinfo.fontok!=0XAA)return 1;
	return 0;		    
}
