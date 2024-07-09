#include "menu.h"
#include "tftlcd.h"
#include "touch.h"
#include "task.h"
#include "sdio_sdcard.h"

unsigned char gImage_qq_logo1[512];
int kkk;
/*
该函数是每次读取一个扇区，读取512个数据显示出来
参数1：显示图片的起始地址，即存入时的开始低地址
参数2：图片一共存了多少个扇区，即数组中数据个数/512并取整，不满512也进1
参数3：最后一组的数据数
*/
void duocixianshi(int x, int y, int weight, int height, uint32_t addr, int number, int last)
{
		for(int i = 0; i < number/*此处改为总的扇区数*/; i++)
		{
			SD_ReadDisk(gImage_qq_logo1, addr/*这个地址要与写入sd卡的初始地址一致*/+i*0x200, 1);		
			if(i==0)
				LCD_ShowPicture_Segmented(x,y,weight,height,(u8 *)gImage_qq_logo1,512);
			else if(i == number-1/*总扇区数减1*/)
				LCD_ShowPicture_Segmented2(weight,height,(u8 *)gImage_qq_logo1,last/*这个改为最后一个扇区里的数*/);
			else
				LCD_ShowPicture_Segmented2(weight,height,(u8 *)gImage_qq_logo1,512);
		}
}

char Str[20];

struct MenuInit InitialMenu[20] = 
{
{(uint8_t *)"______智能浇灌系统", 0, 0, 0},
{(uint8_t *)"   ______ ", 0, 0, 0}
};

struct MenuInit MainMenu[20] = 
{
{(uint8_t *)"", 0, 0, InitialMenu},
{(uint8_t *)"______--实时显示", Now_Data, 0, InitialMenu},
{(uint8_t *)"______--自动控制", Set_Data, 0, InitialMenu},
{(uint8_t *)"______--开关控制", KaiGuan, 0, InitialMenu}
};

struct MenuInit* MenuPoint = InitialMenu;


int  menu_num(void)
{
	int num1=10;
	while(--num1){
			if(MenuPoint[num1].DisplaySrting)
			{
				return num1+1;
			}
}
	return 0;
}


void refresh_show(void)
{	

		
		for(uint16_t m = 0; m < menu_num(); m++)
		{
				LCD_ShowFontHZ(0, m * 80 + 1, MenuPoint[m].DisplaySrting, WHITE, GRAYBLUE);

		}

		if(MenuPoint == InitialMenu)//进入菜单后的图片和文字显示
		{
				RST_0;
				HAL_Delay(20);//带有RST是为了优化刚开始进入的菜单中二维码显示不全的现象
				duocixianshi(430, 237, 240, 243, 0x4D1FE2, 228, 96);//二维码
				RST_1;
				HAL_Delay(50);
				duocixianshi(310, 70, 143, 127, 0x2915D4, 71, 482);//开始
				duocixianshi(100, 237, 240, 243, 0xAF0D0, 225, 512);//用户
				LCD_ShowFontString(160, 200, 100, 40, (uint8_t*)"用户1", 32, 0, BLACK, GRAYBLUE);
				LCD_ShowFontString(480, 200, 140, 40, (uint8_t*)"故障报修", 32, 0, BLACK, GRAYBLUE);
		}
		if(MenuPoint == MainMenu)
		{
				duocixianshi(0, 0, 96, 64, 0x2DBD5A, 24, 512);
				/*主菜单每个选项两边的图片*/
				duocixianshi(175,77, 80, 75, 0x15D06A, 26, 8);//实时
				duocixianshi(510,77, 80, 75, 0x15D06A, 26, 8);
				duocixianshi(185,160, 70, 64, 0x169116, 20, 72);//自动
				duocixianshi(510,160, 70, 64, 0x169116, 20, 72);		
	
				duocixianshi(186,240, 70, 64, 0x2B52C6, 19, 452);//手动
				duocixianshi(510,240, 70, 64, 0x2B52C6, 19, 452);
			
				duocixianshi(10,353, 780, 127, 0x173356, 387, 488);//下面背景
		}			
}


void menu_init(void)
{
	InitialMenu[1].ChildrenMenu = MainMenu;
	InitialMenu[2].ChildrenMenu = MainMenu;//为了扩大触摸开始的区域，再将第二行当成是主菜单的父菜单
  refresh_show();
}

uint16_t x1=0, y=0;

void menu(void)
{
	kkk++;
	TP_Scan(0);
	x1 = 5 - tp_dev.x[0] / 80;		//横屏显示下触摸显示的坐标不变导致横坐标不是从上到下，所以5减去识别的结果
	y = tp_dev.y[0] / 80;

	if(x1 >= 20)	//防止x1太大导致下面数组越界
		x1 = 19;
	if(x1 == 0 && MenuPoint != InitialMenu) //最上边为返回键，返回父菜单
	{
		LCD_Clear(GREEN);
		MenuPoint = MenuPoint[x1].ParentMenu;
		refresh_show();
		HAL_Delay(200);
	} 

	else if(MenuPoint[x1].ChildrenMenu != 0 && x1 < 20)//当前触摸行有子菜单的，进入子菜单
	{
		LCD_Clear(GREEN);
		MenuPoint = MenuPoint[x1].ChildrenMenu;
		refresh_show();  
		HAL_Delay(200);
	}

	else if(MenuPoint[x1].function != 0)  // 有功能函数的进入功能函数
	{
		MenuPoint[x1].function();
	}

}

