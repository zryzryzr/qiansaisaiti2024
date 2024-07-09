#include "menu.h"
#include "tftlcd.h"
#include "touch.h"
#include "task.h"
#include "sdio_sdcard.h"

unsigned char gImage_qq_logo1[512];
int kkk;
/*
�ú�����ÿ�ζ�ȡһ����������ȡ512��������ʾ����
����1����ʾͼƬ����ʼ��ַ��������ʱ�Ŀ�ʼ�͵�ַ
����2��ͼƬһ�����˶��ٸ������������������ݸ���/512��ȡ��������512Ҳ��1
����3�����һ���������
*/
void duocixianshi(int x, int y, int weight, int height, uint32_t addr, int number, int last)
{
		for(int i = 0; i < number/*�˴���Ϊ�ܵ�������*/; i++)
		{
			SD_ReadDisk(gImage_qq_logo1, addr/*�����ַҪ��д��sd���ĳ�ʼ��ַһ��*/+i*0x200, 1);		
			if(i==0)
				LCD_ShowPicture_Segmented(x,y,weight,height,(u8 *)gImage_qq_logo1,512);
			else if(i == number-1/*����������1*/)
				LCD_ShowPicture_Segmented2(weight,height,(u8 *)gImage_qq_logo1,last/*�����Ϊ���һ�����������*/);
			else
				LCD_ShowPicture_Segmented2(weight,height,(u8 *)gImage_qq_logo1,512);
		}
}

char Str[20];

struct MenuInit InitialMenu[20] = 
{
{(uint8_t *)"______���ܽ���ϵͳ", 0, 0, 0},
{(uint8_t *)"   ______ ", 0, 0, 0}
};

struct MenuInit MainMenu[20] = 
{
{(uint8_t *)"", 0, 0, InitialMenu},
{(uint8_t *)"______--ʵʱ��ʾ", Now_Data, 0, InitialMenu},
{(uint8_t *)"______--�Զ�����", Set_Data, 0, InitialMenu},
{(uint8_t *)"______--���ؿ���", KaiGuan, 0, InitialMenu}
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

		if(MenuPoint == InitialMenu)//����˵����ͼƬ��������ʾ
		{
				RST_0;
				HAL_Delay(20);//����RST��Ϊ���Ż��տ�ʼ����Ĳ˵��ж�ά����ʾ��ȫ������
				duocixianshi(430, 237, 240, 243, 0x4D1FE2, 228, 96);//��ά��
				RST_1;
				HAL_Delay(50);
				duocixianshi(310, 70, 143, 127, 0x2915D4, 71, 482);//��ʼ
				duocixianshi(100, 237, 240, 243, 0xAF0D0, 225, 512);//�û�
				LCD_ShowFontString(160, 200, 100, 40, (uint8_t*)"�û�1", 32, 0, BLACK, GRAYBLUE);
				LCD_ShowFontString(480, 200, 140, 40, (uint8_t*)"���ϱ���", 32, 0, BLACK, GRAYBLUE);
		}
		if(MenuPoint == MainMenu)
		{
				duocixianshi(0, 0, 96, 64, 0x2DBD5A, 24, 512);
				/*���˵�ÿ��ѡ�����ߵ�ͼƬ*/
				duocixianshi(175,77, 80, 75, 0x15D06A, 26, 8);//ʵʱ
				duocixianshi(510,77, 80, 75, 0x15D06A, 26, 8);
				duocixianshi(185,160, 70, 64, 0x169116, 20, 72);//�Զ�
				duocixianshi(510,160, 70, 64, 0x169116, 20, 72);		
	
				duocixianshi(186,240, 70, 64, 0x2B52C6, 19, 452);//�ֶ�
				duocixianshi(510,240, 70, 64, 0x2B52C6, 19, 452);
			
				duocixianshi(10,353, 780, 127, 0x173356, 387, 488);//���汳��
		}			
}


void menu_init(void)
{
	InitialMenu[1].ChildrenMenu = MainMenu;
	InitialMenu[2].ChildrenMenu = MainMenu;//Ϊ����������ʼ�������ٽ��ڶ��е��������˵��ĸ��˵�
  refresh_show();
}

uint16_t x1=0, y=0;

void menu(void)
{
	kkk++;
	TP_Scan(0);
	x1 = 5 - tp_dev.x[0] / 80;		//������ʾ�´�����ʾ�����겻�䵼�º����겻�Ǵ��ϵ��£�����5��ȥʶ��Ľ��
	y = tp_dev.y[0] / 80;

	if(x1 >= 20)	//��ֹx1̫������������Խ��
		x1 = 19;
	if(x1 == 0 && MenuPoint != InitialMenu) //���ϱ�Ϊ���ؼ������ظ��˵�
	{
		LCD_Clear(GREEN);
		MenuPoint = MenuPoint[x1].ParentMenu;
		refresh_show();
		HAL_Delay(200);
	} 

	else if(MenuPoint[x1].ChildrenMenu != 0 && x1 < 20)//��ǰ���������Ӳ˵��ģ������Ӳ˵�
	{
		LCD_Clear(GREEN);
		MenuPoint = MenuPoint[x1].ChildrenMenu;
		refresh_show();  
		HAL_Delay(200);
	}

	else if(MenuPoint[x1].function != 0)  // �й��ܺ����Ľ��빦�ܺ���
	{
		MenuPoint[x1].function();
	}

}

