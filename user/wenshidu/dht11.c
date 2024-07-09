#include "dht11.h"
uint8_t str_wenshidu[20];
#define DHT11_PORT GPIOA
#define DHT11_PIN GPIO_PIN_7

uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2, SUM;

uint8_t RH, TEMP;

float Temperature = 0;
float Humidity = 0;

uint8_t retry, Response, Presence = 0;

int count1,ll;
void delay(uint32_t time)
{
	/* change your code here for the delay in microseconds */
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	while ((__HAL_TIM_GET_COUNTER(&htim1))<time);
}


void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}


void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/*温湿度模块dht11介绍
这是一个自带上拉电阻的模块，我测试了即使输入模式不加 上拉也没事
1、首先单片机要给dht11发送一个开始信号，首先拉低电平至少18ms，然后单片机输出26-28us的高电平
（这个20-40us的信号我觉得也是开始信号的一部分，表示单片机发送结束，去掉程序就不对了，但是有的解释很模糊）
2、dht11接受成功后，会发送一个持续80us的低电平信号，接着一个持续80us的高电平信号，表示准备完毕
3、dht11开始发送40bit的信号，每个bit都是以低电平开始，一会后拉高电平，再拉低，信号0：高电平持续20-40us
信号1；高电平持续70us，当发送完最后一次数据后，dht11会拉低电平50us，并拉起单总线电压，等待下一次开始信号
4、这40bit是五组数据8bit 湿度整数数据+8bit 湿度小数数据+8bit 温度整数数据+8bit 温度小数数据+8bit 校验和
校验和为前四个数据相加（应该是大小相加，我试了uint8_t, uint16_t, uint32_t 接收数据都正确，校验也正确），
则说明传输正确
*/


/*共用的单片机发送开始信号的代码*/
void DHT11_Start (void)
{
	Set_Pin_Output (DHT11_PORT, DHT11_PIN);  // set the pin as output
	HAL_GPIO_WritePin (DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET);   // pull the pin low
	delay (20000);   // wait for 18ms
  HAL_GPIO_WritePin (DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);   // pull the pin high
	delay (30);   // wait for 20us
}




/*检查是否连接成功-代码2-一直检测，直到不满足跳出检测*/
//uint32_t aa,bb,cc,dd;
//uint8_t DHT11_Check_Response (void){
//  retry = 0;
//  Set_Pin_Input(DHT11_PORT, DHT11_PIN);    // set as input
//  while(GPIO_PIN_SET == HAL_GPIO_ReadPin(DHT11_PORT,DHT11_PIN) && retry < 100){
//    retry++;
//    delay(1);
//      
//    if(retry>80)
//      return 1;
//  }

//  while(GPIO_PIN_RESET == HAL_GPIO_ReadPin(DHT11_PORT,DHT11_PIN)&&retry<100)
//  {
//    retry++;
//    delay(1);
//    if(retry>80)
//      return 1;
//  }
//  return 0;
//}


/*检验是否连接成功-代码1-直接取值来判断*/
uint8_t DHT11_Check_Response (void)
{
	Response = 1;
  
	delay (40);         //又延时了40us，等待单总线被dht11拉低，因为单总线拉低80us，所以不会超范围
  Set_Pin_Input(DHT11_PORT, DHT11_PIN);
	if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))
	{
		delay (80);      //延时了80us，加上前面的40us，肯定大于dhtll拉低的80us，此时dhtll应该拉高了
                      //并且dhtll拉高了80us，此时正好在拉高40us后，不会超范围
		if ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN))) Response = 0;
		else Response = -1; // 255
    while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));   //等待dhtll80us过后拉低电平，开始发数据
	}
    return Response;
}           

/*读取驱动代码1*/
//可以读取一位一位的数据，而不是一个字节
uint8_t DHT11_Read_Bit(void)
{
	while(GPIO_PIN_SET==HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN));//防止上一次的高电平信号影响到这一次
	while(GPIO_PIN_RESET==HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN));//高电平信号开始
	delay(40);

	if(GPIO_PIN_SET==HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN))
		return 1;
	else
		return 0;
}

//从DHT11读取一个字节
//返回值：读取到的字节数据(8位)
uint8_t DHT11_Read_Byte(void)
{
	uint8_t dat=0;
	for(uint8_t i=0;i<8;i++)
	{
		dat <<= 1;
		dat |= DHT11_Read_Bit();
	}
	return dat;
}

//将数据直接读到对应的数组中
//uint8_t DHT11_Read_Data(uint8_t* humi,uint8_t* temp)
//{
//    uint8_t buf[5];
//	DHT11_Start();
//	if(DHT11_Check_Response() == 0)
//	{
//		for(uint8_t i=0;i<5;i++)
//			buf[i]=DHT11_Read_Byte();
//		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
//		{
//			*humi=buf[0];
//			*temp=buf[2];
//		}
//	}
//	else
//		return 1;
//	return 0;
//}




/*读取驱动代码2*/
//uint8_t DHT11_Read_Byte (void)
//{
//	uint8_t i=0,j;
//	for (j=0;j<8;j++)
//	{ 
//		while (GPIO_PIN_RESET==HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)); // 等待总线被拉高，开始一位数据的发送
//		delay (50);   // wait for 40 us，分辨拉高了多久时间，判断数据
//		if (GPIO_PIN_RESET==HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) )  // if the pin is low
//		{
//			i&= ~(1<<(7-j));   // 拉高小于40us，说明数据为0
//		}
//		else i|= (1<<(7-j));  // 拉高大于40us, 说明数据为1
//		while (GPIO_PIN_SET==HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN));// 如果是1的话，40us还不是一位数据完成，为了这次的高电平不被当成下一次的数据
//                                                                  //等待总线被拉低，一位结束
//	}
//	return i;
//}



/*显示代码*/
//void Display_Temp (float Temp)
//{
//	char str[20] = {0};
//	sprintf (str, (char *)"TEMP: %.2f ", Temp);
//	LCD_ShowString(10,90,tftlcd_data.width,tftlcd_data.height,24,(uint8_t *)str);
//}

//void Display_Rh (float Rh)
//{
//	char str[20] = {0};
//	sprintf (str, "RH:%.2f ", Rh);
//  LCD_ShowString(10,140,tftlcd_data.width,tftlcd_data.height,24,(uint8_t *)str);
//}

void DHT11_GetValue(void)
{
		DHT11_Start();
	  Presence = DHT11_Check_Response();
	  Rh_byte1 = DHT11_Read_Byte ();
	  Rh_byte2 = DHT11_Read_Byte ();
	  Temp_byte1 = DHT11_Read_Byte ();
	  Temp_byte2 = DHT11_Read_Byte ();
	  SUM = DHT11_Read_Byte();
	  TEMP = Temp_byte1;
	  RH = Rh_byte1;
    Temperature = (float) TEMP;
    Humidity = (float) RH;

//    if(Presence==0) sprintf ((char*)str2, (char *)"connect:yes! ");
//    else if(Presence==1)sprintf ((char*)str2, (char *)"connect:not!   ");
//    LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,24,(uint8_t *)str2);
//    if(Rh_byte1+Rh_byte2+Temp_byte1+Temp_byte2 == SUM)
//    { 
//      LCD_ShowString(10,50,tftlcd_data.width,tftlcd_data.height,24,(uint8_t *)"data:right");
//      Display_Temp(Temperature);
//      Display_Rh(Humidity); 
//    }
//    else
//      LCD_ShowString(10,50,tftlcd_data.width,tftlcd_data.height,24,(uint8_t *)"data:error");

//    HAL_Delay(500);

}
