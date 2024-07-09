#ifndef _tftlcd_H
#define _tftlcd_H	
#include "main.h"

typedef __IO uint32_t  vuint32_t;
typedef __IO uint16_t vuint16_t;
typedef __IO uint8_t  vuint8_t;

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t


extern uint16_t LCD_ID;
extern uint16_t	LCD_DIR;			//0������  1������  Ĭ������
extern uint16_t	LCD_height;
extern uint16_t	LCD_width;



//TFTLCD��ַ�ṹ��
typedef struct
{
	vuint16_t LCD_CMD;//MUST vuint16_t,NOT uint16_t
	vuint16_t LCD_DATA;//MUST vuint16_t,NOT uint16_t
}TFTLCD_TypeDef;


//ʹ��NOR/SRAM�� Bank1.sector4,��ַλHADDR[27,26]=11 A6��Ϊ�������������� 
//ע������ʱSTM32�ڲ�������һλ����! 111 1110=0X7E			    
#define TFTLCD_BASE        ((uint32_t)(0x6C000000 | 0x0000007E))
#define TFTLCD             ((TFTLCD_TypeDef *) TFTLCD_BASE)
  

//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
#define Blue         	 0x001F


void LCD_WriteCmd(uint16_t cmd);
void LCD_WriteData(uint16_t data);
void LCD_WriteCmdData(uint16_t cmd,uint16_t data);
void LCD_WriteData_Color(uint16_t color);

void TFTLCD_Init(void); //��ʼ��
void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);//���ô���
void LCD_Display_Dir(uint8_t dir);//������Ļ��ʾ����
void LCD_Clear(uint16_t Color);//����
void LCD_DrawFRONT_COLOR(uint16_t x,uint16_t y,uint16_t color);
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_setPixel(uint16_t x,uint16_t y,uint16_t color);
void LCD_setPixelB(uint16_t Xpos, uint16_t Ypos,uint16_t color,uint16_t R);
void Touch_Test_Point(uint16_t Xpos, uint16_t Ypos,uint16_t color);
void Touch_Big_Point(uint16_t x,uint16_t y,uint16_t color);void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode, uint16_t huabi_color1, uint16_t beijing_color1);
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p, uint16_t huabi_color, uint16_t beijing_color);
void LCD_ShowFontHZ(uint16_t x, uint16_t y, uint8_t *cn, uint16_t huabi_color, uint16_t beijing_color);
void LCD_DrawLine_Color(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);
void LCD_Fill(uint16_t xState,uint16_t yState,uint16_t xEnd,uint16_t yEnd,uint16_t color);
void LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color);
void LCD_ShowPicture(u16 x, u16 y, u16 wide, u16 high,u8 *pic);//��ʾͼƬ
void LCD_ShowPicture_Segmented(u16 x, u16 y, u16 wide, u16 high, u8 *pic, u32 segment_size);
void LCD_ShowPicture_Segmented2(u16 wide, u16 high, u8 *pic, u32 segment_size);
#endif  
