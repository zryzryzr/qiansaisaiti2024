#ifndef _flash_H
#define _flash_H

#include "jianxie.h"


//EN25Xϵ��/Qϵ��оƬ�б�	   
//EN25Q80  ID  0XEF13
//EN25Q16  ID  0XEF14
//EN25Q32  ID  0XEF15
//EN25Q64  ID  0XEF16	
//EN25Q128 ID  0XEF17	
#define EN25Q80 	0XEF13 	
#define EN25Q16 	0XEF14
#define EN25Q32 	0XEF15
//#define EN25Q64 	0XEF16
#define EN25Q128	0XEF17
//#define EN25Q64 	0XC816
//#define EN25Q64 	0X1C16		//GD25QXX
//#define EN25Q64 	0X2016		//XM25QHXX
#define EN25Q64 	0XC216		//MXIC
//#define EN25Q128	0XC817
#define EN25Q256 	0XEF18

extern u16 EN25QXX_TYPE;					//����EN25QXXоƬ�ͺ�		   

#define	EN25QXX_CS 		PBout(14)  		//EN25QXX��Ƭѡ�ź�


//ָ���
#define EN25X_WriteEnable		0x06 
#define EN25X_WriteDisable		0x04 
#define EN25X_ReadStatusReg1	0x05 
#define EN25X_ReadStatusReg2	0x35 
#define EN25X_ReadStatusReg3	0x15 
#define EN25X_WriteStatusReg1   0x01 
#define EN25X_WriteStatusReg2   0x31 
#define EN25X_WriteStatusReg3   0x11 
#define EN25X_ReadData			0x03 
#define EN25X_FastReadData		0x0B 
#define EN25X_FastReadDual		0x3B 
#define EN25X_PageProgram		0x02 
#define EN25X_BlockErase		0xD8 
#define EN25X_SectorErase		0x20 
#define EN25X_ChipErase			0xC7 
#define EN25X_PowerDown			0xB9 
#define EN25X_ReleasePowerDown	0xAB 
#define EN25X_DeviceID			0xAB 
#define EN25X_ManufactDeviceID	0x90 
#define EN25X_JedecDeviceID		0x9F 
#define EN25X_Enable4ByteAddr   0xB7
#define EN25X_Exit4ByteAddr     0xE9

void EN25QXX_Init(void);
u16  EN25QXX_ReadID(void);  	    		//��ȡFLASH ID
u8 EN25QXX_ReadSR(u8 regno);             //��ȡ״̬�Ĵ��� 
void EN25QXX_4ByteAddr_Enable(void);     //ʹ��4�ֽڵ�ַģʽ
void EN25QXX_Write_SR(u8 regno,u8 sr);   //д״̬�Ĵ���
void EN25QXX_Write_Enable(void);  		//дʹ�� 
void EN25QXX_Write_Disable(void);		//д����
void EN25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void EN25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //��ȡflash
void EN25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//д��flash
void EN25QXX_Erase_Chip(void);    	  	//��Ƭ����
void EN25QXX_Erase_Sector(u32 Dst_Addr);	//��������
void EN25QXX_Wait_Busy(void);           	//�ȴ�����
void EN25QXX_PowerDown(void);        	//�������ģʽ
void EN25QXX_WAKEUP(void);				//����



#endif