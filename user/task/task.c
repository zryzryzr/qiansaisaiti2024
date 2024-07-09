#include "task.h"
#include "tftlcd.h"
#include "touch.h"
#include  "menu.h"
#include  "string.h"
extern uint8_t zidong_star;// ȫ���Զ�����PID�Ŀ���
char str_task[20];

/*ɨ����Ļ������*/
uint16_t x2,y2;
setdata setting;
extern  float soil_humidity, water_level, Humidity, Temperature;
uint8_t first;//����˵���һ�β���ʾ����
/*����ʾ�������ˣ��ᵼ����Ļɨ�败����ʱ������������������Ļ���ڼ��ʱ�䣬���޷�ɨ�赽��
������һ��ѭ���м�����ɨ�账��
*/
on_off flag;
on_off shuzhi;

/*ʵʱ��ʾ�Ĺ��ܺ���*/
void Now_Data(void)
{
	LCD_Clear(GREEN);
	duocixianshi(0, 0, 96, 64, 0x2DBD5A, 24, 512);	
	LCD_ShowFontHZ(0, 81, (uint8_t *)"�¶�", BLACK, WHITE);
	duocixianshi(128, 80, 70, 66, 0x2BD188, 20, 72);			
	LCD_ShowFontHZ(0, 161, (uint8_t *)"ʪ��", BLACK, WHITE);
	duocixianshi(128, 161, 64, 64, 0x2C73C8, 16, 512);			
	LCD_ShowFontHZ(0, 241, (uint8_t *)"����ʪ��", BLACK, WHITE);
	duocixianshi(250, 241, 64, 64, 0x2CE354, 16, 512);			
	LCD_ShowFontHZ(0, 321, (uint8_t *)"ˮλ", BLACK, WHITE);
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
		/*��ʾʵʱ����*/
		sprintf(str_task, ":%.4lf��", Temperature);
		LCD_ShowFontString(193, 96, 300, 64, (uint8_t*)str_task, 32, 0, BLACK, WHITE);
		sprintf(str_task, ":%.4lf%%", Humidity);
		LCD_ShowFontString(192, 175, 200,64, (uint8_t*)str_task, 32, 0, BLACK, WHITE);			
		sprintf(str_task, ":%.1lf%%", soil_humidity);
		LCD_ShowFontString(314, 256, 200, 64, (uint8_t*)str_task, 32, 0, BLACK, WHITE);
		sprintf(str_task, ":%.2lfcm", water_level);
		LCD_ShowFontString(187, 336, 200, 64, (uint8_t*)str_task, 32, 0, BLACK, WHITE);
	}

}


/*�Զ����ƵĹ��ܺ���*/
void Set_Data(void)
{
	LCD_Clear(GREEN);
	duocixianshi(0, 0, 96, 64, 0x2DBD5A, 24, 512);	
	LCD_ShowFontHZ(0, 161, (uint8_t *)"�¶�", BLACK, WHITE);
	duocixianshi(128, 160, 70, 66, 0x2BD188, 20, 72);			
	LCD_ShowFontHZ(0, 241, (uint8_t *)"ʪ��", BLACK, WHITE);
	duocixianshi(128, 241, 64, 64, 0x2C73C8, 16, 512);			
	LCD_ShowFontHZ(0, 321, (uint8_t *)"����ʪ��", BLACK, WHITE);
	duocixianshi(250, 321, 64, 64, 0x2CE354, 16, 512);			
	LCD_ShowFontHZ(0, 402, (uint8_t *)"ˮλ", BLACK, WHITE);
	duocixianshi(128, 402, 59, 64, 0x2D52E0, 15, 384);
	LCD_ShowFontHZ(0, 81, (uint8_t *)"�Զ�", BLACK, WHITE);
	
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
		/*�����µĴ����������԰���������ɨ��ӵ�����*/
		if((y>570)&&(y<634))
		{
			
			switch(x)
			{
				case 1:	
					if(first == 1)//ֻ��firstΪ1������£����¿�����Ч������ɨ������Ӱ��
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
		/*ɨ���������*/
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
		

		/*��ʾ�������ݵ�����*/
		sprintf(str_task, ":%.1lf�� ", setting.Temperature);
		LCD_ShowFontString(198, 175, 300, 64, (uint8_t*)str_task, 32, 0, BLACK, WHITE);
		sprintf(str_task, ":%.1lf%% ", setting.Humidity);
		LCD_ShowString(190,256,200,64,32,(uint8_t*)str_task, BLACK, WHITE);
		
		sprintf(str_task, ":%.1lf%% ", setting.soil_humidity);
		LCD_ShowString(310, 336,200,64,32,(uint8_t*)str_task, BLACK, WHITE);
	
		sprintf(str_task, ":%.1lfcm ", setting.water_level);
		LCD_ShowString(187, 416,200,64,32,(uint8_t*)str_task, BLACK, WHITE);		
		
		
		if(first == 0)//��ʾ�Զ����ص�ͼƬ
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

/*�ֶ����ƵĹ��ܺ���*/
void KaiGuan(void)
{

	LCD_Clear(GREEN);
	/*ֻ����ʾһ�εĲ���*/
	duocixianshi(0, 0, 96, 64, 0x2DBD5A, 24, 512);			 
	LCD_ShowFontHZ(0, 81, (uint8_t *)"��ˮ", BLACK, WHITE);
	duocixianshi(128, 81, 64, 64, 0x4EE7E2, 16, 512);
	LCD_ShowFontHZ(0, 402, (uint8_t *)"��ʪ��", BLACK, WHITE);
	duocixianshi(192, 402, 64, 64, 0x4F07E2, 16, 512);
	LCD_ShowFontHZ(0, 241, (uint8_t *)"��", BLACK, WHITE);
	duocixianshi(64, 241, 64, 64, 0x4F27E2, 16, 512);
	LCD_ShowFontHZ(0, 321, (uint8_t *)"����", BLACK, WHITE);
	duocixianshi(128, 321, 64, 64, 0x4F47E2, 16, 512);
	LCD_ShowFontHZ(0, 161, (uint8_t *)"�ι�", BLACK, WHITE);
	duocixianshi(128, 161, 64, 64, 0x4F67E2, 16, 512);
	
	duocixianshi(520, 81, 64, 64, 0x4CDFE2, 16, 512);			
	duocixianshi(520, 161, 64, 64, 0x4CDFE2, 16, 512);				
	duocixianshi(520, 241, 64, 64, 0x4CDFE2, 16, 512);				
	duocixianshi(520, 321, 64, 64, 0x4CDFE2, 16, 512);	
/*����pwm���Ƽ�ʪ�����õ��ĵĴ���*/	
//	duocixianshi(520, 402, 64, 64, 0x4CDFE2, 16, 512);		
	
	duocixianshi(610, 81,  64, 64, 0x4CFFE2, 16, 512);	
	duocixianshi(610, 161, 64, 64, 0x4CFFE2, 16, 512);				
	duocixianshi(610, 241, 64, 64, 0x4CFFE2, 16, 512);				
	duocixianshi(610, 321, 64, 64, 0x4CFFE2, 16, 512);
/*����pwm���Ƽ�ʪ�����õ��ĵĴ���*/	
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
�����µĴ����������԰�������
ɨ��ӵ�����		
		
*/
		if((y>520)&&(y<584))
		{
			switch(x)
			{
				case 1:
					if(shuzhi.choushui < 100)
						shuzhi.choushui += 1;
				break;

/*����pwm���Ƽ�ʪ�����õ��ĵĴ���*/					
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
	
/*ɨ���������*/	
		if((y>610)&&(y<674))
		{
			switch(x)
			{
				case 1:
					if(shuzhi.choushui > 0)
						shuzhi.choushui -= 1;

				break;
		
/*����pwm���Ƽ�ʪ�����õ��ĵĴ���*/
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
			
/*�����µĴ����������԰�������, ɨ�迪���Ƿ���*/
		if((y>700)&&(y<764))
		{		
			/*����ָ�����򣬻Ὣfirst���㣬������õ�*/	
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
��ʾ�����ٶ�
*/		
		sprintf(str_task, ":%d%%  ", shuzhi.choushui);
		LCD_ShowString(192, 96, 200,64,32,(uint8_t*)str_task, BLACK, WHITE);

/*����pwm���Ƽ�ʪ�����õ��ĵĴ���*/	
//		sprintf(str_task, ":%d%%  ", shuzhi.jiashi);
//		LCD_ShowString(254,425,200,64,32,(uint8_t*)str_task, BLACK, WHITE);
		
		sprintf(str_task, ":%d%%  ", shuzhi.deng);
		LCD_ShowString(128, 256,200,64,32,(uint8_t*)str_task, BLACK, WHITE);
	
		sprintf(str_task, ":%d%%  ", shuzhi.fengshan);
		LCD_ShowString(195, 336,200,64,32,(uint8_t*)str_task, BLACK, WHITE);
		
		sprintf(str_task, ":%d%%  ", shuzhi.jiaoshui);
		LCD_ShowString(195, 175,200,64,32,(uint8_t*)str_task, BLACK, WHITE);			
		
		
/*
������Ǹ��ݱ�־λ������ʾ����first��һ����ʾһ�Σ���ֹһֱ��ʾ��Ӱ��ɨ��
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
		
			HAL_Delay(200);//��ֹ��һ��ɨ�赽�ܶ��		
		}
		
	}
}
