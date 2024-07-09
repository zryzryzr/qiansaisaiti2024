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

/*��ʪ��ģ��dht11����
����һ���Դ����������ģ�飬�Ҳ����˼�ʹ����ģʽ���� ����Ҳû��
1�����ȵ�Ƭ��Ҫ��dht11����һ����ʼ�źţ��������͵�ƽ����18ms��Ȼ��Ƭ�����26-28us�ĸߵ�ƽ
�����20-40us���ź��Ҿ���Ҳ�ǿ�ʼ�źŵ�һ���֣���ʾ��Ƭ�����ͽ�����ȥ������Ͳ����ˣ������еĽ��ͺ�ģ����
2��dht11���ܳɹ��󣬻ᷢ��һ������80us�ĵ͵�ƽ�źţ�����һ������80us�ĸߵ�ƽ�źţ���ʾ׼�����
3��dht11��ʼ����40bit���źţ�ÿ��bit�����Ե͵�ƽ��ʼ��һ������ߵ�ƽ�������ͣ��ź�0���ߵ�ƽ����20-40us
�ź�1���ߵ�ƽ����70us�������������һ�����ݺ�dht11�����͵�ƽ50us�����������ߵ�ѹ���ȴ���һ�ο�ʼ�ź�
4����40bit����������8bit ʪ����������+8bit ʪ��С������+8bit �¶���������+8bit �¶�С������+8bit У���
У���Ϊǰ�ĸ�������ӣ�Ӧ���Ǵ�С��ӣ�������uint8_t, uint16_t, uint32_t �������ݶ���ȷ��У��Ҳ��ȷ����
��˵��������ȷ
*/


/*���õĵ�Ƭ�����Ϳ�ʼ�źŵĴ���*/
void DHT11_Start (void)
{
	Set_Pin_Output (DHT11_PORT, DHT11_PIN);  // set the pin as output
	HAL_GPIO_WritePin (DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET);   // pull the pin low
	delay (20000);   // wait for 18ms
  HAL_GPIO_WritePin (DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);   // pull the pin high
	delay (30);   // wait for 20us
}




/*����Ƿ����ӳɹ�-����2-һֱ��⣬ֱ���������������*/
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


/*�����Ƿ����ӳɹ�-����1-ֱ��ȡֵ���ж�*/
uint8_t DHT11_Check_Response (void)
{
	Response = 1;
  
	delay (40);         //����ʱ��40us���ȴ������߱�dht11���ͣ���Ϊ����������80us�����Բ��ᳬ��Χ
  Set_Pin_Input(DHT11_PORT, DHT11_PIN);
	if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))
	{
		delay (80);      //��ʱ��80us������ǰ���40us���϶�����dhtll���͵�80us����ʱdhtllӦ��������
                      //����dhtll������80us����ʱ����������40us�󣬲��ᳬ��Χ
		if ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN))) Response = 0;
		else Response = -1; // 255
    while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));   //�ȴ�dhtll80us�������͵�ƽ����ʼ������
	}
    return Response;
}           

/*��ȡ��������1*/
//���Զ�ȡһλһλ�����ݣ�������һ���ֽ�
uint8_t DHT11_Read_Bit(void)
{
	while(GPIO_PIN_SET==HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN));//��ֹ��һ�εĸߵ�ƽ�ź�Ӱ�쵽��һ��
	while(GPIO_PIN_RESET==HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN));//�ߵ�ƽ�źſ�ʼ
	delay(40);

	if(GPIO_PIN_SET==HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN))
		return 1;
	else
		return 0;
}

//��DHT11��ȡһ���ֽ�
//����ֵ����ȡ�����ֽ�����(8λ)
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

//������ֱ�Ӷ�����Ӧ��������
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




/*��ȡ��������2*/
//uint8_t DHT11_Read_Byte (void)
//{
//	uint8_t i=0,j;
//	for (j=0;j<8;j++)
//	{ 
//		while (GPIO_PIN_RESET==HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)); // �ȴ����߱����ߣ���ʼһλ���ݵķ���
//		delay (50);   // wait for 40 us���ֱ������˶��ʱ�䣬�ж�����
//		if (GPIO_PIN_RESET==HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) )  // if the pin is low
//		{
//			i&= ~(1<<(7-j));   // ����С��40us��˵������Ϊ0
//		}
//		else i|= (1<<(7-j));  // ���ߴ���40us, ˵������Ϊ1
//		while (GPIO_PIN_SET==HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN));// �����1�Ļ���40us������һλ������ɣ�Ϊ����εĸߵ�ƽ����������һ�ε�����
//                                                                  //�ȴ����߱����ͣ�һλ����
//	}
//	return i;
//}



/*��ʾ����*/
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
