#ifndef _font_show_H
#define _font_show_H


#include "font_update.h"

void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size);			//�õ����ֵĵ�����
void LCD_Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode,uint16_t huabi_color, uint16_t beijing_color);
void LCD_ShowFontString(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode, uint16_t huabi_color, uint16_t beijing_color);
void LCD_ShowFontStringMid(u16 x,u16 y,u8*str,u8 size,u8 len);


#endif
