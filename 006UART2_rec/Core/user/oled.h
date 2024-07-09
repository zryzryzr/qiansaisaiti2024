


#ifndef OLED_H
#define OLED_H



extern unsigned char F16x16[];
extern const unsigned char F6x8[][6];
extern const unsigned char F8X16[];
extern unsigned char BMP[];




void OLED_Clear(void);  
void OLED_Init(void);
void OLED_P6x8Str(unsigned char x, unsigned char y,const char  ch[]);
void OLED_number(unsigned char x, unsigned char y,unsigned long n);
void OLED_time(unsigned char x, unsigned char y);
void OLED_P8x16Str(unsigned char x, unsigned char y,const char  ch[]);
void OLED_P16x16Ch(unsigned char x, unsigned char y, unsigned char N);
void OLED_Ch(unsigned char x, unsigned char y, unsigned char N,const unsigned char CH[]);
void OLED_BMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,const unsigned char BMP[]);  
void OLED_int(unsigned char x, unsigned char y,unsigned int n);




#endif 

