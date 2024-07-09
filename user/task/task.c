#include "task.h"
#include "tftlcd.h"
#include "touch.h"
#include  "menu.h"
#include  "string.h"
extern uint8_t zidong_star;// 全部自动控制PID的开关
char str_task[20];

/*扫描屏幕的坐标*/
uint16_t x2,y2;
setdata setting;
extern  float soil_humidity, water_level, Humidity, Temperature;
uint8_t first;//跳入菜单第一次才显示汉字
/*当显示东西多了，会导致屏幕扫描触摸的时间间隔过长，如果轻点屏幕处于间隔时间，就无法扫描到，
可以在一次循环中加入多次扫描处理
*/
on_off flag;
on_off shuzhi;

/*实时显示的功能函数*/
void Now_Data(void)
{
	LCD_Clear(GREEN);
	duocixianshi(0, 0, 96, 64, 0x2DBD5A, 24, 512);	
	LCD_ShowFontHZ(0, 81, (uint8_t *)"温度", BLACK, WHITE);
	duocixianshi(128, 80, 70, 66, 0x2BD188, 20, 72);			
	LCD_ShowFontHZ(0, 161, (uint8_t *)"湿度", BLACK, WHITE);
	duocixianshi(128, 161, 64, 64, 0x2C73C8, 16, 512);			
	LCD_ShowFontHZ(0, 241, (uint8_t *)"土壤湿度", BLACK, WHITE);
	duocixianshi(250, 241, 64, 64, 0x2CE354, 16, 512);			
	LCD_ShowFontHZ(0, 321, (uint8_t *)"水位", BLACK, WHITE);
	duocixianshi(128, 321, 59, 64, 0x2D52E0, 15, 384);
	duocixianshi(480, 0, 320, 480, 0x2E73E2, 600, 512);
	
	while(1)
	{
		TP_Scan(0);
		uint8_t x2 = 5 - tp_dev.x[0] / 80;
		if(x2 >= 20)
			x2 = 19;	
		
		if(x2 == 0)
		{
			LCD_Clear(GREEN);
			refresh_show();
			HAL_Delay(200);			
			return;
		}
		/*显示实时数据*/
		sprintf(str_task, ":%.4lf度", Temperature);
		LCD_ShowFontString(193, 96, 300, 64, (uint8_t*)str_task, 32, 0, BLACK, WHITE);
		sprintf(str_task, ":%.4lf%%", Humidity);
		LCD_ShowFontString(192, 175, 200,64, (uint8_t*)str_task, 32, 0, BLACK, WHITE);			
		sprintf(str_task, ":%.1lf%%", soil_humidity);
		LCD_ShowFontString(314, 256, 200, 64, (uint8_t*)str_task, 32, 0, BLACK, WHITE);
		sprintf(str_task, ":%.2lfcm", water_level);
		LCD_ShowFontString(187, 336, 200, 64, (uint8_t*)str_task, 32, 0, BLACK, WHITE);
	}

}


/*自动控制的功能函数*/
void Set_Data(void)
{
	LCD_Clear(GREEN);
	duocixianshi(0, 0, 96, 64, 0x2DBD5A, 24, 512);	
	LCD_ShowFontHZ(0, 161, (uint8_t *)"温度", BLACK, WHITE);
	duocixianshi(128, 160, 70, 66, 0x2BD188, 20, 72);			
	LCD_ShowFontHZ(0, 241, (uint8_t *)"湿度", BLACK, WHITE);
	duocixianshi(128, 241, 64, 64, 0x2C73C8, 16, 512);			
	LCD_ShowFontHZ(0, 321, (uint8_t *)"土壤湿度", BLACK, WHITE);
	duocixianshi(250, 321, 64, 64, 0x2CE354, 16, 512);			
	LCD_ShowFontHZ(0, 402, (uint8_t *)"水位", BLACK, WHITE);
	duocixianshi(128, 402, 59, 64, 0x2D52E0, 15, 384);
	LCD_ShowFontHZ(0, 81, (uint8_t *)"自动", BLACK, WHITE);
	
	duocixianshi(570, 161, 64, 64, 0x4CDFE2, 16, 512);			
	duocixianshi(570, 241, 64, 64, 0x4CDFE2, 16, 512);				
	duocixianshi(570, 321, 64, 64, 0x4CDFE2, 16, 512);				
	duocixianshi(570, 402, 64, 64, 0x4CDFE2, 16, 512);	
	
	duocixianshi(680, 161,  64, 64, 0x4CFFE2, 16, 512);	
	duocixianshi(680, 241, 64, 64, 0x4CFFE2, 16, 512);				
	duocixianshi(680, 321, 64, 64, 0x4CFFE2, 16, 512);				
	duocixianshi(680, 402, 64, 64, 0x4CFFE2, 16, 512);		
	
	while(1)
	{
		TP_Scan(0);
		uint16_t x = 5 - tp_dev.x[0] / 80;
		uint16_t y = tp_dev.y[0];

		if(x >= 20)
			x = 19;			
		if(x == 0)
		{
			LCD_Clear(GREEN);
			refresh_show();
			first = 0;
			HAL_Delay(200);
			return;
		}
		/*横屏下的触摸屏坐标仍按照竖屏，扫描加的区域*/
		if((y>570)&&(y<634))
		{
			
			switch(x)
			{
				case 1:	
					if(first == 1)//只有first为1的情况下，按下开关有效，避免扫描过快的影响
						zidong_star = !zidong_star;
				case 2:
					setting.Temperature+=0.1;
				break;
				case 3:
					setting.Humidity+=0.1;
				break;
				case 4:
					setting.soil_humidity+=0.1;
				break;
				case 5:
					setting.water_level+=0.02;
				break;								
			}
			if(x == 1)
					first=0;			
		}	
		/*扫描减的区域*/
		if((y>680)&&(y<744))
		{
			switch(x)
			{
				case 2:
					setting.Temperature-=0.1;
				break;
				case 3:
					setting.Humidity-=0.1;
				break;
				case 4:
					setting.soil_humidity-=0.1;
				break;
				case 5:
					setting.water_level-=0.02;
				break;								
			}
		}
		

		/*显示设置数据的区域*/
		sprintf(str_task, ":%.1lf度 ", setting.Temperature);
		LCD_ShowFontString(198, 175, 300, 64, (uint8_t*)str_task, 32, 0, BLACK, WHITE);
		sprintf(str_task, ":%.1lf%% ", setting.Humidity);
		LCD_ShowString(190,256,200,64,32,(uint8_t*)str_task, BLACK, WHITE);
		
		sprintf(str_task, ":%.1lf%% ", setting.soil_humidity);
		LCD_ShowString(310, 336,200,64,32,(uint8_t*)str_task, BLACK, WHITE);
	
		sprintf(str_task, ":%.1lfcm ", setting.water_level);
		LCD_ShowString(187, 416,200,64,32,(uint8_t*)str_task, BLACK, WHITE);		
		
		
		if(first == 0)//显示自动开关的图片
		{
			HAL_Delay(200);
			first = 1;
		if(zidong_star == 1)
			duocixianshi(570, 81, 64, 64, 0x4F87E2, 16, 512);
		else
			duocixianshi(570, 81, 64, 64, 0x4FA7E2, 16, 512);		
		}
	}
}

/*手动控制的功能函数*/
void KaiGuan(void)
{

	LCD_Clear(GREEN);
	/*只需显示一次的部分*/
	duocixianshi(0, 0, 96, 64, 0x2DBD5A, 24, 512);			 
	LCD_ShowFontHZ(0, 81, (uint8_t *)"抽水", BLACK, WHITE);
	duocixianshi(128, 81, 64, 64, 0x4EE7E2, 16, 512);
	LCD_ShowFontHZ(0, 402, (uint8_t *)"加湿器", BLACK, WHITE);
	duocixianshi(192, 402, 64, 64, 0x4F07E2, 16, 512);
	LCD_ShowFontHZ(0, 241, (uint8_t *)"灯", BLACK, WHITE);
	duocixianshi(64, 241, 64, 64, 0x4F27E2, 16, 512);
	LCD_ShowFontHZ(0, 321, (uint8_t *)"风扇", BLACK, WHITE);
	duocixianshi(128, 321, 64, 64, 0x4F47E2, 16, 512);
	LCD_ShowFontHZ(0, 161, (uint8_t *)"滴灌", BLACK, WHITE);
	duocixianshi(128, 161, 64, 64, 0x4F67E2, 16, 512);
	
	duocixianshi(520, 81, 64, 64, 0x4CDFE2, 16, 512);			
	duocixianshi(520, 161, 64, 64, 0x4CDFE2, 16, 512);				
	duocixianshi(520, 241, 64, 64, 0x4CDFE2, 16, 512);				
	duocixianshi(520, 321, 64, 64, 0x4CDFE2, 16, 512);	
/*采用pwm控制加湿速率用到的的代码*/	
//	duocixianshi(520, 402, 64, 64, 0x4CDFE2, 16, 512);		
	
	duocixianshi(610, 81,  64, 64, 0x4CFFE2, 16, 512);	
	duocixianshi(610, 161, 64, 64, 0x4CFFE2, 16, 512);				
	duocixianshi(610, 241, 64, 64, 0x4CFFE2, 16, 512);				
	duocixianshi(610, 321, 64, 64, 0x4CFFE2, 16, 512);
/*采用pwm控制加湿速率用到的的代码*/	
//	duocixianshi(610, 402, 64, 64, 0x4CFFE2, 16, 512);		
	
	while(1)
	{
		TP_Scan(0);
		uint16_t x = 5 - tp_dev.x[0] / 80;
		uint16_t y = tp_dev.y[0];
		if(x >= 20)
			x = 19;			
		if(x == 0)
		{
			LCD_Clear(GREEN);
			refresh_show();
			first = 0;
			HAL_Delay(200);
			return;
		}
		
/*
横屏下的触摸屏坐标仍按照竖屏
扫描加的区域		
		
*/
		if((y>520)&&(y<584))
		{
			switch(x)
			{
				case 1:
					if(shuzhi.choushui < 100)
						shuzhi.choushui += 1;
				break;

/*采用pwm控制加湿速率用到的的代码*/					
//				case 5:
//					if(shuzhi.jiashi < 100)
//						shuzhi.jiashi += 1;
//				break;

				case 3:
					if(shuzhi.deng < 100)
						shuzhi.deng += 1;
				break;
				case 4:
					if(shuzhi.fengshan < 100)
						shuzhi.fengshan += 1;
				break;	
				case 2:
					if(shuzhi.jiaoshui < 100)
						shuzhi.jiaoshui += 1;
				break;				
			}
		}
	
/*扫描减的区域*/	
		if((y>610)&&(y<674))
		{
			switch(x)
			{
				case 1:
					if(shuzhi.choushui > 0)
						shuzhi.choushui -= 1;

				break;
		
/*采用pwm控制加湿速率用到的的代码*/
//				case 5:
//					if(shuzhi.jiashi > 0)
//						shuzhi.jiashi -= 1;

//				break;

				case 3:
					if(shuzhi.deng > 0)
						shuzhi.deng -= 1;
					
				break;
				case 4:
					if(shuzhi.fengshan > 0)
						shuzhi.fengshan -= 1;
				
				break;	
				case 2:
					if(shuzhi.jiaoshui > 0)
						shuzhi.jiaoshui -= 1;
					
				break;				
			}
		}
		
			if(shuzhi.choushui==0 && flag.choushui==1)
			{flag.choushui=0; first = 0;}
			else  if(shuzhi.choushui>0 && flag.choushui==0)
			{flag.choushui=1;	first = 0;}	
			


			if(shuzhi.deng==0&&flag.deng==1)
			{	flag.deng=0;    first = 0;}
			else if(shuzhi.deng>0&&flag.deng==0)
			{	flag.deng=1;		first = 0;}
			if(shuzhi.fengshan==0&&flag.fengshan==1)
			{	flag.fengshan=0;	first = 0;}
			else if(shuzhi.fengshan>0&&flag.fengshan==0)
			{	flag.fengshan=1;	first = 0;}
			if(shuzhi.jiaoshui==0&&flag.jiaoshui==1)
			{	flag.jiaoshui=0;  first = 0;}
			else if(shuzhi.jiaoshui>0&&flag.jiaoshui==0)
			{	flag.jiaoshui=1;  first = 0;}
			
/*横屏下的触摸屏坐标仍按照竖屏, 扫描开关是否按下*/
		if((y>700)&&(y<764))
		{		
			/*按下指定区域，会将first归零，下面会用到*/	
			if(x>0&&x<6)
					first=0;
			switch(x)
			{
				case 1:			
					flag.choushui = !flag.choushui;
				break;
				case 5:
					flag.jiashi = !flag.jiashi;
					if(flag.jiashi == 1)
						shuzhi.jiashi = 100;
					else 
						shuzhi.jiashi = 0;
				break;
				case 3:
					flag.deng = !flag.deng;			
				break;
				case 4:	
					flag.fengshan = !flag.fengshan;			
				break;	
				case 2:		
					flag.jiaoshui = !flag.jiaoshui;								
				break;				
			}
		}

/*
显示各个速度
*/		
		sprintf(str_task, ":%d%%  ", shuzhi.choushui);
		LCD_ShowString(192, 96, 200,64,32,(uint8_t*)str_task, BLACK, WHITE);

/*采用pwm控制加湿速率用到的的代码*/	
//		sprintf(str_task, ":%d%%  ", shuzhi.jiashi);
//		LCD_ShowString(254,425,200,64,32,(uint8_t*)str_task, BLACK, WHITE);
		
		sprintf(str_task, ":%d%%  ", shuzhi.deng);
		LCD_ShowString(128, 256,200,64,32,(uint8_t*)str_task, BLACK, WHITE);
	
		sprintf(str_task, ":%d%%  ", shuzhi.fengshan);
		LCD_ShowString(195, 336,200,64,32,(uint8_t*)str_task, BLACK, WHITE);
		
		sprintf(str_task, ":%d%%  ", shuzhi.jiaoshui);
		LCD_ShowString(195, 175,200,64,32,(uint8_t*)str_task, BLACK, WHITE);			
		
		
/*
下面的是根据标志位，来显示开关first按一次显示一次，防止一直显示，影响扫描
*/
		if(first == 0)
		{
			first = 1;	
			if(flag.choushui == 1)
				duocixianshi(700, 81, 64, 64, 0x4F87E2, 16, 512);
			else
			{duocixianshi(700, 81, 64, 64, 0x4FA7E2, 16, 512);shuzhi.choushui=0;}
			
			if(flag.jiaoshui == 1)
				duocixianshi(700, 161, 64, 64, 0x4F87E2, 16, 512);
			else
			{duocixianshi(700, 161, 64, 64, 0x4FA7E2, 16, 512);shuzhi.jiaoshui=0;}
			
			if(flag.deng == 1)
				duocixianshi(700, 241, 64, 64, 0x4F87E2, 16, 512);
			else
			{duocixianshi(700, 241, 64, 64, 0x4FA7E2, 16, 512);shuzhi.deng=0;}
			if(flag.fengshan == 1)
				duocixianshi(700, 321, 64, 64, 0x4F87E2, 16, 512);
			else
			{duocixianshi(700, 321, 64, 64, 0x4FA7E2, 16, 512);shuzhi.fengshan=0;}
			if(flag.jiashi == 1)
				duocixianshi(700, 402, 64, 64, 0x4F87E2, 16, 512);
			else
			{duocixianshi(700, 402, 64, 64, 0x4FA7E2, 16, 512);shuzhi.jiashi=0;}
		
			HAL_Delay(200);//防止按一次扫描到很多次		
		}
		
	}
}
