#include "font_update.h"
  
#include "flash.h"  
#include "tftlcd.h"  
#include "string.h"



								  
//字库区域占用的总扇区数大小(3个字库+unigbk表+字库信息=3238700字节,约占791个W25QXX扇区)
#define FONTSECSIZE	 	791
//字库存放起始地址 
#define FONTINFOADDR 	1024*1024*12 					//从12M地址以后开始存放字库
														//前面12M被fatfs占用了.
														//12M以后紧跟3个字库+UNIGBK.BIN,总大小3.09M,被字库占用了,不能动!
														//15.10M以后,用户可以自由使用.建议用最后的100K字节比较好.
														//前面4M被fatfs占用了.
														
														
//用来保存字库基本信息，地址，大小等
_font_info ftinfo;

//字库存放在磁盘中的路径
u8*const GBK_PATH[5]=
{
"/FONT/UNIGBK.BIN",	//UNIGBK.BIN的存放位置
"/FONT/GBK12.FON",	//GBK12的存放位置
"/FONT/GBK16.FON",	//GBK16的存放位置
"/FONT/GBK24.FON",	//GBK24的存放位置
"/FONT/GBK32.FON",	//GBK32的存放位置
}; 
//更新时的提示信息
u8*const UPDATE_REMIND_TBL[5]=
{
"Updating UNIGBK.BIN",	//提示正在更新UNIGBK.bin
"Updating GBK12.FON ",	//提示正在更新GBK12
"Updating GBK16.FON ",	//提示正在更新GBK16
"Updating GBK24.FON ",	//提示正在更新GBK24
"Updating GBK32.FON ",	//提示正在更新GBK32
}; 




//初始化字体
//返回值:0,字库完好.
//		 其他,字库丢失
u8 font_init(void)
{		
	u8 t=0;
//	EN25QXX_Init();  
	while(t<10)//连续读取10次,都是错误,说明确实是有问题,得更新字库了
	{
		t++;
		EN25QXX_Read((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));//读出ftinfo结构体数据
		if(ftinfo.fontok==0XAA)break;
		HAL_Delay(20);
	}
	if(ftinfo.fontok!=0XAA)return 1;
	return 0;		    
}
