
/*
3.5'屏实际上是8位ILI9486
4.0'屏是16位ILI9806 CTS836U93J50
*/

#include "tftlcd.h"
#include "stdlib.h"
#include "font.h"
#include "fsmc.h"   
#include "stdio.h"

//LCD的画笔颜色和背景色	   
uint16_t FRONT_COLOR=BLACK;	//画笔颜色
uint16_t BACK_COLOR=WHITE;  //背景色 

uint16_t	LCD_DIR=1;			//0：竖屏  1：横屏  默认竖屏
uint16_t	LCD_height=800;
uint16_t	LCD_width=480;
uint16_t LCD_ID=0x9806;



//写寄存器函数
//cmd:寄存器值
void LCD_WriteCmd(uint16_t cmd)
{
	TFTLCD->LCD_CMD=cmd;
}

//写数据
//data:要写入的值
void LCD_WriteData(uint16_t data)
{
	TFTLCD->LCD_DATA=data;
}

void LCD_WriteCmdData(uint16_t cmd,uint16_t data)
{
	LCD_WriteCmd(cmd);
	LCD_WriteData(data);
}


uint32_t LCD_RGBColor_Change(uint16_t color)
{
	uint8_t r,g,b=0;
	
	r=(color>>11)&0x1f;
	g=(color>>5)&0x3f;
	b=color&0x1f;
	
	return ((r<<13)|(g<<6)|(b<<1));
}
void LCD_WriteData_Color(uint16_t color)
{

	TFTLCD->LCD_DATA=color;


}

//读数据
//返回值:读到的值
uint16_t LCD_ReadData(void)
{
	uint16_t ram;	
	if(LCD_ID==0x9486){	
	ram=TFTLCD->LCD_DATA;
	ram=ram<<8;
	ram|=TFTLCD->LCD_DATA;	
	}		
	else if(LCD_ID==0x9806){
	ram=TFTLCD->LCD_DATA;
	}
	return ram;	
}


//设置LCD显示方向
//dir:0,竖屏；1,横屏
void LCD_Display_Dir(uint8_t dir)
{
	LCD_DIR=dir;         //横屏/竖屏
	if(dir==0)  //默认竖屏方向
	{		
	if(LCD_ID==0x9486){		
		LCD_WriteCmdData(0x36,0x08);
	}		
	else if(LCD_ID==0x9806){			
		LCD_WriteCmdData(0x36,0x00);
	}

	}
	else
	{	
	if(LCD_ID==0x9486){		
		LCD_WriteCmdData(0x36,0x28);
		LCD_WriteCmd(0XB6);
		LCD_WriteData(0x00);
		LCD_WriteData(0x02);
		LCD_WriteData(0x3B);
	}		
	else if(LCD_ID==0x9806){	
		LCD_WriteCmdData(0x36,0x64);
		LCD_height=480;
		LCD_width=800;		
	}

	}	
}


//设置窗口,并自动设置画点坐标到窗口左上角(sx,sy).
//sx,sy:窗口起始坐标(左上角)
//width,height:窗口宽度和高度,必须大于0!!
//窗体大小:width*height. 
void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{    

	LCD_WriteCmd(0x2A);
	LCD_WriteData(sx/256);   
	LCD_WriteData(sx%256); 	 
	LCD_WriteData(width/256); 
	LCD_WriteData(width%256);
	
	LCD_WriteCmd(0x2B);
	LCD_WriteData(sy/256);  
	LCD_WriteData(sy%256);
	LCD_WriteData(height/256); 
	LCD_WriteData(height%256); 	

	LCD_WriteCmd(0x2C);
}


//读取个某点的颜色值	 
//x,y:坐标
//返回值:此点的颜色
uint16_t LCD_ReadPoint(uint16_t x,uint16_t y)
{
 	uint16_t r=0,g=0,b=0,color;
	
	if(x>=LCD_width||y>=LCD_height)return 0;	//超过了范围,直接返回		     
	LCD_Set_Window(x, y, x, y);
	
	if(LCD_ID==0x9486){
		LCD_WriteCmd(0x2e);
		r=LCD_ReadData();								//dummy Read     		 				    
		r=LCD_ReadData();								//实际坐标颜色	
		b=r>>8;
		g=r<<8;
		color=g+b;
	}		
	else if(LCD_ID==0x9806){
		LCD_WriteCmd(0x2e);
		r=LCD_ReadData();								//dummy Read     		 				    
		r=LCD_ReadData();								//实际坐标颜色	
		b=LCD_ReadData();
		g=LCD_ReadData(); 
		color=(((r>>11)<<11)|((g>>10)<<5)|(b>>11));
	}
 	return color;				
}
	 

void TFTLCD_Init(void)
{

	LCD_WriteCmd(0xd3);	
	LCD_ID=TFTLCD->LCD_DATA;	 
	LCD_ID=TFTLCD->LCD_DATA;  
	LCD_ID=TFTLCD->LCD_DATA;  
	LCD_ID<<=8;
	LCD_ID|=TFTLCD->LCD_DATA;  

if(LCD_ID==0x9486){
	LCD_height=480;
	LCD_width=320;	
	LCD_WriteCmd(0xE0); //P-Gamma
	LCD_WriteData(0x00);
	LCD_WriteData(0x13);
	LCD_WriteData(0x18);
	LCD_WriteData(0x04);
	LCD_WriteData(0x0F);
	LCD_WriteData(0x06);
	LCD_WriteData(0x3A);
	LCD_WriteData(0x56);
	LCD_WriteData(0x4D);
	LCD_WriteData(0x03);
	LCD_WriteData(0x0A);
	LCD_WriteData(0x06);
	LCD_WriteData(0x30);
	LCD_WriteData(0x3E);
	LCD_WriteData(0x0F);
	
	LCD_WriteCmd(0XE1); //N-Gamma
	LCD_WriteData(0x00);
	LCD_WriteData(0x13);
	LCD_WriteData(0x18);
	LCD_WriteData(0x01);
	LCD_WriteData(0x11);
	LCD_WriteData(0x06);
	LCD_WriteData(0x38);
	LCD_WriteData(0x34);
	LCD_WriteData(0x4D);
	LCD_WriteData(0x06);
	LCD_WriteData(0x0D);
	LCD_WriteData(0x0B);
	LCD_WriteData(0x31);
	LCD_WriteData(0x37);
	LCD_WriteData(0x0F); 
	
	LCD_WriteCmd(0XC0);   //Power Control 1
	LCD_WriteData(0x18); //Vreg1out
	LCD_WriteData(0x17); //Verg2out
	
	LCD_WriteCmd(0xC1);   //Power Control 2
	LCD_WriteData(0x41); //VGH,VGL
	
	LCD_WriteCmd(0xC5);   //Power Control 3
	LCD_WriteData(0x00);
	LCD_WriteData(0x1A); //Vcom
	LCD_WriteData(0x80);
	
	LCD_WriteCmd(0x36);   //Memory Access
	LCD_WriteData(0x08);   //48
	
	LCD_WriteCmd(0x3A);   // Interface Pixel Format
	LCD_WriteData(0x55); //16bit
	
	LCD_WriteCmd(0XB0);   // Interface Mode Control
	LCD_WriteData(0x00);
	
	LCD_WriteCmd(0xB1);   //Frame rate
	LCD_WriteData(0xA0); //60Hz
	
	LCD_WriteCmd(0xB4);   //Display Inversion Control
	LCD_WriteData(0x02); //2-dot
	
	LCD_WriteCmd(0XB6);   //RGB/MCU Interface Control
	LCD_WriteData(0x00);
	LCD_WriteData(0x22);//0 GS SS SM ISC[3:0];其中GS SS控制显示方向，同时修改R36
	LCD_WriteData(0x3B);
	
	LCD_WriteCmd(0XE9);    // Set Image Function
	LCD_WriteData(0x00);  //disable 24 bit data input
	
	LCD_WriteCmd(0xF7);    // Adjust Control
	LCD_WriteData(0xA9);
	LCD_WriteData(0x51);
	LCD_WriteData(0x2C);
	LCD_WriteData(0x82);  // D7 stream, loose
	
}
else if(LCD_ID==0x9806){
	LCD_height=800;
	LCD_width=480;		
	LCD_WriteCmd(0xFF);
	LCD_WriteData(0xFF);
	LCD_WriteData(0x98);
	LCD_WriteData(0x06);

	LCD_WriteCmd(0xBA);
	LCD_WriteData(0x60);

	LCD_WriteCmd(0xBC);
	LCD_WriteData(0x03);
	LCD_WriteData(0x0E);
	LCD_WriteData(0x03);
	LCD_WriteData(0x63);
	LCD_WriteData(0x01);
	LCD_WriteData(0x01);
	LCD_WriteData(0x1B);
	LCD_WriteData(0x12);
	LCD_WriteData(0x6F);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x01);
	LCD_WriteData(0x01);
	LCD_WriteData(0x03);
	LCD_WriteData(0x02);
	LCD_WriteData(0xFF);
	LCD_WriteData(0xF2);
	LCD_WriteData(0x01);
	LCD_WriteData(0x00);
	LCD_WriteData(0xC0);

	LCD_WriteCmd(0xBD);
	LCD_WriteData(0x02);
	LCD_WriteData(0x13);
	LCD_WriteData(0x45);
	LCD_WriteData(0x67);
	LCD_WriteData(0x45);
	LCD_WriteData(0x67);
	LCD_WriteData(0x01);
	LCD_WriteData(0x23);

	LCD_WriteCmd(0xBE);
	LCD_WriteData(0x01);
	LCD_WriteData(0x22);
	LCD_WriteData(0x22);
	LCD_WriteData(0xDC);
	LCD_WriteData(0xBA);
	LCD_WriteData(0x67);
	LCD_WriteData(0x22);
	LCD_WriteData(0x22);
	LCD_WriteData(0x22);

	LCD_WriteCmd(0xC7);
	LCD_WriteData(0x66);

	LCD_WriteCmd(0xED);
	LCD_WriteData(0x7F);
	LCD_WriteData(0x0F);
	LCD_WriteData(0x00);

	LCD_WriteCmd(0xC0);
	LCD_WriteData(0x03);
	LCD_WriteData(0x0B);
	LCD_WriteData(0x00);
	
	LCD_WriteCmd(0XF5);
	LCD_WriteData(0x20);
	LCD_WriteData(0x43);
	LCD_WriteData(0x00);

	LCD_WriteCmd(0xEE);
	LCD_WriteData(0x0A);
	LCD_WriteData(0x1B);
	LCD_WriteData(0x5F);
	LCD_WriteData(0x40);
	LCD_WriteData(0x28);
	LCD_WriteData(0x38);
	LCD_WriteData(0x02);
	LCD_WriteData(0x2B);
	LCD_WriteData(0x50);
	LCD_WriteData(0x00);
	LCD_WriteData(0x80);

	LCD_WriteCmd(0xFC);
	LCD_WriteData(0x08);

	LCD_WriteCmd(0xDF);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x20);

	LCD_WriteCmd(0xF3);
	LCD_WriteData(0x74);

	LCD_WriteCmd(0xB4);
	LCD_WriteData(0x02);
	LCD_WriteData(0x02);
	LCD_WriteData(0x02);

	LCD_WriteCmd(0xF7);
	LCD_WriteData(0x82);
	
	LCD_WriteCmd(0xB1);
	LCD_WriteData(0x00);
	LCD_WriteData(0x13);
	LCD_WriteData(0x13);

	LCD_WriteCmd(0xF2);
	LCD_WriteData(0x41);
	LCD_WriteData(0x04);
	LCD_WriteData(0x41);
	LCD_WriteData(0x28);

	LCD_WriteCmd(0xC1);
	LCD_WriteData(0x17);
	LCD_WriteData(0x78);
	LCD_WriteData(0x7B);
	LCD_WriteData(0x20);

	LCD_WriteCmd(0xE0);
	LCD_WriteData(0x00);
	LCD_WriteData(0x02);
	LCD_WriteData(0x0C);
	LCD_WriteData(0x0F);
	LCD_WriteData(0x11);
	LCD_WriteData(0x1C);
	LCD_WriteData(0xC8);
	LCD_WriteData(0x07);
	LCD_WriteData(0x03);
	LCD_WriteData(0x08);
	LCD_WriteData(0x03);
	LCD_WriteData(0x0D);
	LCD_WriteData(0x0C);
	LCD_WriteData(0x31);
	LCD_WriteData(0x2C);
	LCD_WriteData(0x00);

	LCD_WriteCmd(0xE1);
	LCD_WriteData(0x00);
	LCD_WriteData(0x02);
	LCD_WriteData(0x08);
	LCD_WriteData(0x0E);
	LCD_WriteData(0x12);
	LCD_WriteData(0x17);
	LCD_WriteData(0x7C);
	LCD_WriteData(0x0A);
	LCD_WriteData(0x03);
	LCD_WriteData(0x09);
	LCD_WriteData(0x06);
	LCD_WriteData(0x0C);
	LCD_WriteData(0x0C);
	LCD_WriteData(0x2E);
	LCD_WriteData(0x2A);
	LCD_WriteData(0x00);

	LCD_WriteCmd(0x35);
	LCD_WriteData(0x00);

	LCD_WriteCmd(0x36); 
	LCD_WriteData(0xC0);

	LCD_WriteCmd(0x3A); 
	LCD_WriteData(0x55);

}
	LCD_WriteCmd(0x11);

//	HAL_Delay(120);

	LCD_WriteCmd(0x29);

	LCD_Display_Dir(LCD_DIR);		//0：竖屏  1：横屏  默认竖屏
	LCD_Clear(BLACK);
}

//清屏函数
//color:要清屏的填充色
void LCD_Clear(uint16_t color)
{
	uint16_t i, j ;

	LCD_Set_Window(0, 0, LCD_width-1, LCD_height-1);	 //作用区域
  	for(i=0; i<LCD_width; i++)
	{
		for (j=0; j<LCD_height; j++)
		{
			LCD_WriteData_Color(color);
		}
	} 
}

void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{	  	    			
	LCD_WriteCmd(0x2A);
	LCD_WriteData(Xpos>>8);   
	LCD_WriteData(Xpos); 	 
	LCD_WriteData(Xpos>>8); 
	LCD_WriteData(Xpos);
	
	LCD_WriteCmd(0x2B);
	LCD_WriteData(Ypos>>8);  
	LCD_WriteData(Ypos);
	LCD_WriteData(Ypos>>8); 
	LCD_WriteData(Ypos); 	

	LCD_WriteCmd(0x2C);	
} 

void LCD_setPixel(uint16_t x,uint16_t y,uint16_t color)
{
	LCD_SetCursor(x,y);//设置光标位置 
	LCD_WriteData_Color(color); 
}

void LCD_setPixelB(uint16_t Xpos, uint16_t Ypos,uint16_t color,uint16_t R)
{
	LCD_WriteCmd(0x2A);
	LCD_WriteData(Xpos>>8);   
	LCD_WriteData(Xpos); 	 
	LCD_WriteData((Xpos+R)>>8); 
	LCD_WriteData(Xpos+R);
	
	LCD_WriteCmd(0x2B);
	LCD_WriteData(Ypos>>8);  
	LCD_WriteData(Ypos);
	LCD_WriteData((Ypos+R)>>8); 
	LCD_WriteData(Ypos+R); 	

	LCD_WriteCmd(0x2C);
	LCD_WriteData_Color(color);
}

void Touch_Test_Point(uint16_t x,uint16_t y,uint16_t color)
{
	uint8_t i;
	for(i=0;i<25;i++)
	LCD_setPixel(x-12+i,y,color);
	for(i=0;i<25;i++)
	LCD_setPixel(x,y-12+i,color);	
	
	
	for(i=0;i<13;i++)
	LCD_setPixel(x-6+i,y-6,color);
	for(i=0;i<13;i++)
	LCD_setPixel(x-6+i,y+6,color);		
	
	for(i=0;i<13;i++)
	LCD_setPixel(x-6,y-6+i,color);
	for(i=0;i<13;i++)
	LCD_setPixel(x+6,y-6+i,color);			

}
void Touch_Big_Point(uint16_t x,uint16_t y,uint16_t color)
{	    
		u16 i,j;
	for(i=y;i<y+4;i++)
		for(j=x;j<x+4;j++)
			LCD_setPixel(j,i,color);
	 	  	
}

//画点
//x,y:坐标
//FRONT_COLOR:此点的颜色
void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	LCD_Set_Window(x, y, x, y);  //设置点的位置
	LCD_WriteData_Color(FRONT_COLOR);	
}

//快速画点
//x,y:坐标
//color:颜色
void LCD_DrawFRONT_COLOR(uint16_t x,uint16_t y,uint16_t color)
{	   
	LCD_Set_Window(x, y, x, y);
	LCD_WriteData_Color(color);	
} 

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 


void LCD_DrawLine_Color(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawFRONT_COLOR(uRow,uCol,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 

 

 

//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24/32
//mode:叠加方式(1)还是非叠加方式(0),同一位置的第二个显示不会覆盖第一个显示
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode, uint16_t huabi_color1, uint16_t beijing_color1)
{  							  
    uint8_t temp,t1,t;
	uint16_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{   
//		if(size==12)temp=ascii_1206[num][t]; 	 	//调用1206字体
//			if(size==16)temp=ascii_1608[num][t];	//调用1608字体
//		else if(size==24)temp=ascii_2412[num][t];	//调用2412字体
			if(size==32)temp=ascii_3216[num][t];	//调用2412字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_DrawFRONT_COLOR(x,y,huabi_color1);
			else if(mode==0)LCD_DrawFRONT_COLOR(x,y,beijing_color1);
			temp<<=1;
			y++;
			if(y>=LCD_height)return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=LCD_width)return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
} 


//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p, uint16_t huabi_color, uint16_t beijing_color)
{         
	uint8_t x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,0, huabi_color, beijing_color);
        x+=size/2;
        p++;
    }  
}

/*字符显示使用教程, 用普中官方的字模提取软件，取模方式选择c51格式, 参数设置中有两个选项
第一个可以选择字体和大小，第二个里面选择恒向取模，在文字输入区输入汉字，按下ctrl+回车，
在对应点阵生成区生成点阵数据, 复制之后粘贴到font.h中的struct  Cn32CharTypeDef const CnChar32x29[]这个里面
前面还要加上对应汉字，加上双引号
for (wordNum=0; wordNum<20; wordNum++)这一行如果加的汉字多余20个字就要把20改的大一点
for(i=0; i<116; i++)这一行的116是生成的字有多少数据，
if((x-x0)==32)、if((y-y0)==29)、x += 32;中的32,29是指的宽度和高度，根据生成的点阵改
*/

void LCD_ShowFontHZ(uint16_t x, uint16_t y, uint8_t *cn, uint16_t huabi_color, uint16_t beijing_color)
{
	uint16_t i, j, wordNum;
	uint16_t color;
	uint16_t x0=x; 
	uint16_t y0=y; 
	while (*cn != '\0')
	{
		for (wordNum=0; wordNum<50; wordNum++)
		{	//wordNum扫描字库的字数
			if ((CnChar32x29[wordNum].Index[0]==*cn)&&(CnChar32x29[wordNum].Index[1]==*(cn+1)))
			{
				for(i=0; i<512; i++) 
				{	//MSK的位数
					color=CnChar32x29[wordNum].Msk[i];
					for(j=0;j<8;j++) 
					{
						if((color&0x80)==0x80)
						{
							LCD_DrawFRONT_COLOR(x,y,huabi_color);
						} 						
						else
						{
							LCD_DrawFRONT_COLOR(x,y,beijing_color);
						} 
						color<<=1;
						x++;
						if((x-x0)==64)
						{
							x=x0;
							y++;
							if((y-y0)==64)
							{
								y=y0;
							}
						}
					}//for(j=0;j<8;j++)结束
				}	
			}
			
		} //for (wordNum=0; wordNum<20; wordNum++)结束 	
		cn += 2;
		x += 64;
		x0=x;
	}
}	
 
//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Fill(uint16_t xState,uint16_t yState,uint16_t xEnd,uint16_t yEnd,uint16_t color)
{          
	uint16_t temp;

    if((xState > xEnd) || (yState > yEnd))
    {
        return;
    }   
	LCD_Set_Window(xState, yState, xEnd, yEnd); 
    xState = xEnd - xState + 1;
	yState = yEnd - yState + 1;

	while(xState--)
	{
	 	temp = yState;
		while (temp--)
	 	{			
			LCD_WriteData_Color(color);	
		}
	}	
} 

//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color)
{  
	uint16_t height,width;
	uint16_t i,j;
	width=ex-sx+1; 			//得到填充的宽度
	height=ey-sy+1;			//高度
	
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			LCD_Set_Window(sx+j, sy+i,ex, ey);
			LCD_WriteData_Color(color[i*width+j]);
		}
	}		  
}


void LCD_ShowPicture(u16 x, u16 y, u16 wide, u16 high,u8 *pic)
{

	u16 temp = 0;
	long tmp=0,num=0;
	LCD_Set_Window(x, y, x+wide-1, y+high-1);
	num = wide * high*2 ;
	//方法1：提高显示速度，太快可能导致图像显示不全
	do
	{  
		temp = pic[tmp + 1];
		temp = temp << 8;
		temp = temp | pic[tmp];
		LCD_WriteData_Color(temp);//逐点显示
		tmp += 2;
	}
	while(tmp < num);
	
	//方法2：可有效消除LCD显示速度过快导致显示不全问题
//	for(i=0;i<high;i++)
//	{
//		for(j=0;j<wide;j++)
//		{
//			temp = pic[tmp + 1];
//			temp = temp << 8;
//			temp = temp | pic[tmp];
//			LCD_DrawFRONT_COLOR(x+j,y+i,temp);
//			tmp += 2;
//		}
//	}	
}


/*该函数可控制显示多少数据，segment_size*/
void LCD_ShowPicture_Segmented(u16 x, u16 y, u16 wide, u16 high, u8 *pic, u32 segment_size)
{
    u16 temp = 0;
    u32 tmp = 0, num = 0;
    num = wide * high * 2;	
    u32 remaining_data = num;
    u32 data_to_write;

    LCD_Set_Window(x, y, x + wide - 1, y + high - 1);

        // Determine the number of data bytes to write in this segment
        data_to_write = (num > segment_size) ? segment_size : remaining_data;

        // Loop through the segment and write the data
//        for (tmp = 0; tmp < data_to_write; tmp += 2)
//        {
//            temp = pic[tmp + 1];
//            temp = temp << 8;
//            temp = temp | pic[tmp];
//            LCD_WriteData_Color(temp);
//        }
				do
				{  
					temp = pic[tmp + 1];
					temp = temp << 8;
					temp = temp | pic[tmp];
					LCD_WriteData_Color(temp);//逐点显示
					tmp += 2;
				}
				while(tmp < data_to_write);
}
/*配合上一个的函数使用，没有设置窗口，光标不会移动*/
void LCD_ShowPicture_Segmented2(u16 wide, u16 high, u8 *pic, u32 segment_size)
{
    u16 temp = 0;
    u32 tmp = 0, num = 0;
    num = wide * high * 2;	
    u32 remaining_data = num;
    u32 data_to_write;
	
        // Determine the number of data bytes to write in this segment
        data_to_write = (num > segment_size) ? segment_size : remaining_data;

        // Loop through the segment and write the data
//        for (tmp = 0; tmp < data_to_write; tmp += 2)
//        {
//            temp = pic[tmp + 1];
//            temp = temp << 8;
//            temp = temp | pic[tmp];
//            LCD_WriteData_Color(temp);
//        }
				do
				{  
					temp = pic[tmp + 1];
					temp = temp << 8;
					temp = temp | pic[tmp];
					LCD_WriteData_Color(temp);//逐点显示
					tmp += 2;
				}
				while(tmp < data_to_write);
}
