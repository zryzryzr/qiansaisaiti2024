/***************************************************************************

***************************************************************************/



#include "oled.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c1;

extern uint64_t runtime;









#define BLACK 0
#define WHITE 1






void Write_IIC_Command(uint8_t IIC_Command)
{
	HAL_I2C_Mem_Write(&hi2c1,0x78,0x00,1,&IIC_Command,1,0x1000);
}


void Write_IIC_Data(uint8_t IIC_Data)
{
	HAL_I2C_Mem_Write(&hi2c1,0x78,0x40,1,&IIC_Data,1,0x1000);
}


//设置起始点坐标
void IIC_SetPos(uint8_t x, uint8_t y)
{
  uint8_t dat[3]={(0xb0+y),(((x&0xf0)>>4)|0x10),((x&0x0f)|0x01)};
	HAL_I2C_Mem_Write(&hi2c1,0x78,0x00,1,dat,3,0x1000);
}

//清屏
void OLED_Clear(void)
{
	uint8_t m,n;
	for(m=0;m<8;m++)
	{
		uint8_t dat[3]={(0xb0+m),0x00,0x10};
		HAL_I2C_Mem_Write(&hi2c1,0x78,0x00,1,dat,3,0x1000);
		for(n=0;n<128;n++)
		{
			Write_IIC_Data(0x00);
		}
	}
}

void OLED_Init(void)
{
	uint8_t command[25]={0xAE,0x00,0x10,0x40,0xB0,0x81,0xCF,0xA1,0xA6,0xA8,
									 0x3F,0xC8,0xD3,0x00,0xD5,0x80,0xD9,0xF1,0xDA,0x12,
									 0xDB,0x40,0x8D,0x14,0xAF};
	HAL_I2C_Mem_Write(&hi2c1,0x78,0x00,1,command,25,0x100);
	OLED_Clear();
}




/***************功能描述：显示6*8一组标准ASCII字符串    显示的坐标（x,y），x为列，y为页(字符行)范围0～7****************/
void OLED_P6x8Str(unsigned char x, unsigned char y,const char  ch[])
{
    unsigned char c=0,i=0,j=0;      
    while (ch[j]!='\0')
    {    
        c =ch[j]-32;
        if(x>126){x=0;y++;}
        IIC_SetPos(x,y);
        for(i=0;i<6;i++)     
          Write_IIC_Data(F6x8[c][i]); 
        x+=6;
        j++;
    }
}

/***************功能描述：显示数字    显示的坐标（x,y），x为列，y为页(字符行)范围0～7****************/
void OLED_number(unsigned char x, unsigned char y,unsigned long n)
{
    unsigned char num[10],c=0,i=0,s=0 ;

	if(n==0) {num[s]=0;s++;}
	else
		while(n>0)
		{
			num[s]=n%10;
			n=n/10;
			s++;
		}
    while (s>0)
    {    
			c =num[s-1]+16;
			if(x>126){x=0;y++;}
			IIC_SetPos(x,y);
			for(i=0;i<6;i++)     
				Write_IIC_Data(F6x8[c][i]);
			x+=6;
			s--;
    }
}

/***************功能描述：显示系统时间    显示的坐标（x,y），x为列，y为页(字符行)范围0～7****************/
void OLED_time(unsigned char x, unsigned char y)
{
  unsigned char c=0,i=0,num[]={0,0,':'-32,0,0,':'-32,0,0},j=0;
	uint32_t n;
	n=runtime;
	num[7]=n%10+16;n=n/10;
	num[6]=n%6+16;n=n/6;
	num[4]=n%10+16;n=n/10;
	num[3]=n%6+16;n=n/6;	
	num[1]=n%10+16;n=n/10;
	num[0]=n%10+16;n=n/10;	
    for(j=0;j<8;j++)
    {    
        c =num[j];
        if(x>126){x=0;y++;}
        IIC_SetPos(x,y);
        for(i=0;i<6;i++)     
            Write_IIC_Data(F6x8[c][i]); 
        x+=6;
    }
		
}
///*******************功能描述：显示8*16一组标准ASCII字符串     显示的坐标（x,y），y为页范围0～7****************/
void OLED_P8x16Str(unsigned char x, unsigned char y,const char  ch[])
{
    unsigned char c=0,i=0,j=0;
    while (ch[j]!='\0')
    {    
			c =ch[j]-32;
			if(x>120){x=0;y++;}
			IIC_SetPos(x , y);
			for(i=0;i<8;i++)     
					Write_IIC_Data(F8X16[c*16+i]);
			IIC_SetPos(x,y + 1);
			for(i=0;i<8;i++)     
					Write_IIC_Data(F8X16[c*16+i+8]); 
			x+=8;
			j++;
    }
}
///*****************功能描述：显示16*16点阵  显示的坐标（x,y），y为页范围0～7****************************/
void OLED_P16x16Ch(unsigned char x, unsigned char y, unsigned char N)
{
    unsigned char wm=0;
    unsigned int adder=32*N;  //      
    IIC_SetPos(x , y);
    for(wm = 0;wm < 16;wm++)  //             
    {
        Write_IIC_Data(F16x16[adder]);    
        adder += 1;
    }      
	  IIC_SetPos(x,y + 1);
    for(wm = 0;wm < 16;wm++) //         
    {
        Write_IIC_Data(F16x16[adder]);
        adder += 1;
    }
}
///***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_BMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,const unsigned char BMP[])
{     
    unsigned int j=0;
    unsigned char x,y;
    if(y1%8==0) y=y1/8;      
    else y=y1/8+1;
    for(y=y0;y<y1;y++)
    {
        IIC_SetPos(x0,y); 
        for(x=x0;x<x1;x++)
        {      
            Write_IIC_Data(BMP[j++]);            
        }
    }
} 

///*****************功能描述：显示16*16点阵  显示的坐标（x,y），y为页范围0～7****************************/
void OLED_Ch(unsigned char x, unsigned char y, unsigned char N,const unsigned char CH[])
{
    unsigned char wm=0;
    unsigned int adder=32*N;  //      
    IIC_SetPos(x , y);
    for(wm = 0;wm < 16;wm++)  //             
    {
        Write_IIC_Data(CH[adder]);    
        adder += 1;
    }      
	  IIC_SetPos(x,y + 1);
    for(wm = 0;wm < 16;wm++) //         
    {
        Write_IIC_Data(CH[adder]);
        adder += 1;
    }
}



void OLED_int(unsigned char x, unsigned char y,unsigned int n)
{
  unsigned char num[5]="     ",c=0,i=0,s=0,j ;
	
	if(n==0) {num[s]=0;s++;}
	else
		while(n>0)
		{
			num[s]=n%10;
			n=n/10;
			s++;
		}
		j=5-s;
    while (s>0)
    {    
			c =num[s-1]+16;
			IIC_SetPos(x,y);

			for(i=0;i<6;i++)     
				Write_IIC_Data(F6x8[c][i]);
			x+=6;
			s--;
    }
    while (j>0)
    {    
			IIC_SetPos(x,y);

			for(i=0;i<6;i++)     
				Write_IIC_Data(F6x8[0][i]);

			x+=6;
			j--;
    }		
}














