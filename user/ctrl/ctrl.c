#include "ctrl.h"

extern on_off flag, shuzhi;
extern struct MenuInit MainMenu[20];
extern struct MenuInit* MenuPoint;
extern uint32_t rxbuffer[adc_max];
extern uint8_t first;
uint8_t adc_time, pwm_time, pid_time, gpio_time;
double adc_add[adc_num]; 
double adc_average[adc_num];
float soil_humidity, water_level;
Scan yu, yu_tem;
uint8_t yu_jiemian_flag;
uint8_t datashi[6];
char proBuffer[6]={101, 0 , 0 , 0 , 0, 102};
uint8_t rxBuffer[4];

void adc_change()
{
  if(adc_time > 30)
  {			

		adc_time = 0;
    for(int k = 0; k < adc_max;)
      for(int  j = 0; j < adc_num;)
        adc_add[j++] += rxbuffer[k++];
    for(int j = 0; j < adc_num; j++)
    {
      adc_average[j] = adc_add[j] / 32; 
      adc_add[j] = 0;
    }
		soil_humidity = (((4095-adc_average[1])* 100/3092 ) + ((4095-adc_average[2])* 100/3092 )) / 2;
		water_level = (adc_average[0]-1000)/200;
		
		datashi[1] = soil_humidity;
		datashi[2] = water_level;
		datashi[3] = Humidity;
		datashi[4] = Temperature;
		HAL_UART_Transmit_IT(&huart1, datashi, 4);
  }
}


void pwm_change()
{
	if(pwm_time > 25)
	{
		pwm_time = 0;
//根据pid计算出来的各个pwm数值来进行pwm控制
	__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1, shuzhi.choushui);
	__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2, shuzhi.jiaoshui);
	__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3, shuzhi.deng);
	__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4, shuzhi.fengshan);
	__HAL_TIM_SetCompare(&htim9,TIM_CHANNEL_1, shuzhi.fengshan_jiare);
//根据加湿的标志位来调节
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, flag.jiashi);
//	__HAL_TIM_SetCompare(&htim9,TIM_CHANNEL_2, shuzhi.jiashi);	//加湿还不知道怎么pwm控制
	}
}

void pid_change()
{
	if(pid_time > 45)
	{
		pid_time = 0;
		if(zidong_star == 1)//开启自动开关时才开始pid调节
		{

//进行不同模块的pid计算并赋值给对应的pwm设定值
			wendu.SetPoint = setting.Temperature;
			wendu_jiang.SetPoint = setting.Temperature;
			shidu.SetPoint = setting.Humidity;
			turangshidu.SetPoint = setting.soil_humidity;
			shuiwei.SetPoint = setting.water_level;
			shuzhi.fengshan_jiare = PID_wendu(&wendu, Temperature);
			shuzhi.fengshan = PID_wendu_tongfeng(&wendu_jiang, Temperature);
			shuzhi.jiaoshui = PID_turangshidu(&turangshidu, soil_humidity);
			shuzhi.choushui = PID_shuiwei(&shuiwei, water_level);

/*采用pwm控制加湿速率用到的的代码*/			
//		 shuzhi.jiashi = PID_shidu(&shidu, Humidity);

/*采用继电器控制加湿，根据实际数值和设定数值来确定加湿的标志位*/			
      if(Humidity < setting.Humidity)
					shuzhi.jiashi = 100;
			else shuzhi.jiashi = 0;
/*采用pwm控制加湿速率用到的的代码*/
			if(shuzhi.jiashi==0&&flag.jiashi==1)
			{	flag.jiashi=0;  first = 0;}
			else if(shuzhi.jiashi>0&&flag.jiashi==0)
			{	flag.jiashi=1;	first = 0;}			
		}
	}
}

int jh,kh=0;
int gh[10];
void GPIO_Scan()
{
	if(gpio_time > 28)
	{
		yu_tem.pa0=yu.pa0;yu_tem.pa1=yu.pa1;yu_tem.pa2=yu.pa2;
		yu_tem.pa4=yu.pa4;yu_tem.pa5=yu.pa5;yu_tem.pb6=yu.pb6;
		yu.pa0 = HAL_GPIO_ReadPin(yu_pa0_GPIO_Port, yu_pa0_Pin);
		yu.pa1 = HAL_GPIO_ReadPin(yu_pa1_GPIO_Port, yu_pa1_Pin);
		yu.pa2 = HAL_GPIO_ReadPin(yu_pa2_GPIO_Port, yu_pa2_Pin);	
		yu.pa4 = HAL_GPIO_ReadPin(yu_pa4_GPIO_Port, yu_pa4_Pin);	
		yu.pa5 = HAL_GPIO_ReadPin(yu_pa5_GPIO_Port, yu_pa5_Pin);	
		yu.pb6 = HAL_GPIO_ReadPin(yu_pb6_GPIO_Port, yu_pb6_Pin);	
		
		if(yu_tem.pa0!=yu.pa0||yu_tem.pa1!=yu.pa1||yu_tem.pa2!=yu.pa2||yu_tem.pa4!=yu.pa4||yu_tem.pa5!=yu.pa5||yu_tem.pb6!=yu.pb6)
		{	
			if(yu.pa0 == 1)
			{

				shuzhi.deng = 100;
				jh++;				
			}
			else 
			{shuzhi.deng = 0;jh++;}

			if(yu.pa1 == 1)
			{
				shuzhi.jiaoshui = 50;
			}	
			else shuzhi.jiaoshui = 0;

			if(yu.pa2 == 1)
			{
				shuzhi.jiashi=100;
			}		
			else 
			{shuzhi.jiashi=0;}

			if(yu.pa4 == 1)
			{
				shuzhi.fengshan = 100;
			}	
			else shuzhi.fengshan= 0;

			if(yu.pa5 == 1)
			{
				shuzhi.choushui = 50;
			}	
			else shuzhi.choushui = 0;

			if(yu.pb6 == 1)
			{
				shuzhi.fengshan_jiare = 100;
			}
			else shuzhi.fengshan_jiare = 0;
	}
		
		
//		yu.pa6 = HAL_GPIO_ReadPin(yu_pa6_GPIO_Port, yu_pa6_Pin);
//		yu.pb5 = HAL_GPIO_ReadPin(yu_pb5_GPIO_Port, yu_pb5_Pin);
//		yu.pa3 = HAL_GPIO_ReadPin(yu_pa3_GPIO_Port, yu_pa3_Pin);	
//		if(yu.pb5 == 0 || yu.pa3 == 0 || yu.pa6 == 0) 
//			yu_jiemian_flag = 0;
		
//		if(yu.pa6 == 1 && yu_jiemian_flag == 0)
//		{
//			MenuPoint = MainMenu;
//			yu_jiemian_flag = 1;			
//			Now_Data();
//		}

//		if(yu.pb5 == 1 && yu_jiemian_flag == 0)
//		{
//		
//			MenuPoint = MainMenu;
//			yu_jiemian_flag = 1;			
//			KaiGuan();
//		}

//		if(yu.pa3 == 1 && yu_jiemian_flag == 0)
//		{
//	
//			MenuPoint = MainMenu;
//			yu_jiemian_flag = 1;			
//			Set_Data();
//		}
		
	}		
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  if(htim->Instance == TIM3)
	{
/*时间片每隔一段时间执行对应的函数*/	
			adc_time++;
			pwm_time++;			
			pid_time++;
			gpio_time++;
			adc_change();
			pwm_change();
			pid_change();	
			GPIO_Scan();
	}
/*
温湿度检测模块，不连上温湿度模块,显示屏无法显示，程序会卡死在读取	
while(GPIO_PIN_SET==HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN));这里
//*/
	if(htim->Instance == TIM2)
	{
			DHT11_GetValue();
	}

}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    
    if(huart->Instance == USART1)
		{
        for(uint16_t i = 0; i < RX_CMD_LEN; i++)
				{
            
            proBuffer[i] = rxBuffer[i];
        }
    }
    flag.choushui = proBuffer[1];
		flag.deng = proBuffer[2];
		flag.fengshan = proBuffer[3];
		flag.jiashi = proBuffer[0];  \
}


