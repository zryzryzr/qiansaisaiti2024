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
extern uint16_t	LCD_DIR;			//0：竖屏  1：横屏  默认竖屏
extern uint16_t	LCD_height;
extern uint16_t	LCD_width;



//TFTLCD地址结构体
typedef struct
{
	vuint16_t LCD_CMD;//MUST vuint16_t,NOT uint16_t
	vuint16_t LCD_DATA;//MUST vuint16_t,NOT uint16_t
}TFTLCD_TypeDef;


//使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A6作为数据命令区分线 
//注意设置时STM32内部会右移一位对齐! 111 1110=0X7E			    
#define TFTLCD_BASE        ((uint32_t)(0x6C000000 | 0x0000007E))
#define TFTLCD             ((TFTLCD_TypeDef *) TFTLCD_BASE)
  

//画笔颜色
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
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)
#define Blue         	 0x001F


void LCD_WriteCmd(uint16_t cmd);
void LCD_WriteData(uint16_t data);
void LCD_WriteCmdData(uint16_t cmd,uint16_t data);
void LCD_WriteData_Color(uint16_t color);

void TFTLCD_Init(void); //初始化
void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);//设置窗口
void LCD_Display_Dir(uint8_t dir);//设置屏幕显示方向
void LCD_Clear(uint16_t Color);//清屏
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
void LCD_ShowPicture(u16 x, u16 y, u16 wide, u16 high,u8 *pic);//显示图片
void LCD_ShowPicture_Segmented(u16 x, u16 y, u16 wide, u16 high, u8 *pic, u32 segment_size);
void LCD_ShowPicture_Segmented2(u16 wide, u16 high, u8 *pic, u32 segment_size);
#endif  
