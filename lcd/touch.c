#include "touch.h"
#include "stdlib.h"
#include "math.h"

float xfac=0.1750;
float yfac=0.2580;
float xoff=21.0000;
float yoff=-17.0000;

void delayus(uint8_t x)
{
	volatile	uint8_t i;//
	for(i=0;i<x+10;i++);
}

_m_tp_dev tp_dev=
{
	TOUCH_init,
	TP_Scan,
	Touch_Adjust,
	0,
	0, 
	0,
	0,
	0,
	0,	  	 		
	0,
	0,	  	 		
};					
//Ĭ��Ϊtouchtype=0������.
u8 CMD_RDX=0XD0;
u8 CMD_RDY=0X90;

//SPIд����
//������ICд��1byte����    
//num:Ҫд�������
void TP_Write_Byte(u8 num)    
{  
	u8 count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80)TDIN_1;  
		else TDIN_0;   
		num<<=1;    
		TCLK_0; 
		delayus(2);
		TCLK_1;		//��������Ч	        
	}		 			    
} 		 
//SPI������ 
//�Ӵ�����IC��ȡadcֵ
//CMD:ָ��
//����ֵ:����������	   
u16 TP_Read_AD(u8 CMD)	  
{ 	 
	u8 count=0; 	  
	u16 Num=0; 
	TCLK_0;		//������ʱ�� 	 
	TDIN_0; 	//����������
	TCS_0; 		//ѡ�д�����IC
	TP_Write_Byte(CMD);//����������
	delayus(2);//ADS7846��ת��ʱ���Ϊ6us
	TCLK_0; 	     	    
	delayus(2);    	   
	TCLK_1;		//��1��ʱ�ӣ����BUSY
	delayus(2);    
	TCLK_0; 	     	    
	for(count=0;count<16;count++)//����16λ����,ֻ�и�12λ��Ч 
	{ 				  
		Num<<=1; 	 
		TCLK_0;	//�½�����Ч  	    	   
		delayus(2);    
 		TCLK_1;
 		if(DOUT)Num++; 		 
	}  	
	Num>>=4;   	//ֻ�и�12λ��Ч.
	TCS_1;		//�ͷ�Ƭѡ	 
	return(Num);   
}

//��ȡһ������ֵ(x����y)
//������ȡREAD_TIMES������,����Щ������������,
//Ȼ��ȥ����ͺ����LOST_VAL����,ȡƽ��ֵ 
//xy:ָ�CMD_RDX/CMD_RDY��
//����ֵ:����������
#define READ_TIMES 40 	//��ȡ����
#define LOST_VAL 1	  	//����ֵ
u16 TP_Read_XOY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u32 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)buf[i]=TP_Read_AD(xy);		 		    
	for(i=0;i<READ_TIMES-1; i++)//����
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//��������
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 

//��ȡx,y����
//��Сֵ��������100.
//x,y:��ȡ��������ֵ
//����ֵ:0,ʧ��;1,�ɹ���
u8 TP_Read_XY(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=TP_Read_XOY(CMD_RDX);
	ytemp=TP_Read_XOY(CMD_RDY);	  												   
	//if(xtemp<100||ytemp<100)return 0;//����ʧ��
	*x=xtemp;
	*y=ytemp;
	return 1;//�����ɹ�
}
//����2�ζ�ȡ������IC,�������ε�ƫ��ܳ���
//ERR_RANGE,��������,����Ϊ������ȷ,�����������.	   
//�ú����ܴ�����׼ȷ��
//x,y:��ȡ��������ֵ
//����ֵ:0,ʧ��;1,�ɹ���
#define ERR_RANGE 50 //��Χ 
u8 TP_Read_XY2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;    
    flag=TP_Read_XY(&x1,&y1);   
    if(flag==0)return(0);
    flag=TP_Read_XY(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//ǰ�����β�����+-50��
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
}   
//////////////////////////////////////////////////////////////////////////////////		  
//��LCD�����йصĺ���  
//��һ��������
//����У׼�õ�
//x,y:����
//color:��ɫ
void TP_Drow_Touch_Point(u16 x,u16 y,u16 color)
{
	Touch_Test_Point(x, y,color);
}	  
//��һ�����(2*2�ĵ�)		   
//x,y:����
//color:��ɫ
void TP_Draw_Big_Point(u16 x,u16 y,u16 color)
{	    
	Touch_Big_Point(x,y,color); 	  	
}							  
//////////////////////////////////////////////////////////////////////////////////		  
//��������ɨ��
//tp:0,��Ļ����;1,��������(У׼�����ⳡ����)
//����ֵ:��ǰ����״̬.
//0,�����޴���;1,�����д���
u8 TP_Scan(u8 tp)
{			   

	return (CST716_Scan(0));

}  
 
//��ʾУ׼���(��������)
void TP_Adj_Info_Show(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2,u16 x3,u16 y3,u16 fac)
{	  
}
		 
//������У׼����
//�õ��ĸ�У׼����
void Touch_Adjust(void)
{								 

}
//��������ʼ��  		    
//����ֵ:0,���й�У׼
//       1,δ����У׼
u8 TOUCH_init(void)
{
	CST716_Init();
	return 0;
								 
}

/**************************CST716*******************************/
//I2C��д����	
#define CST_CMD_WR 				0X2A    	//д����
#define CST_CMD_RD 				0X2B		//������
  
//���ּĴ������� 
#define CST_DEVIDE_MODE 		0x00   		//ģʽ���ƼĴ���
#define CST_REG_NUM_FINGER      0x02		//����״̬�Ĵ���

#define CST_TP1_REG 			0X03	  	//��һ�����������ݵ�ַ
#define CST_TP2_REG 			0X09		//�ڶ������������ݵ�ַ
 
#define	CST_ID_G_LIB_VERSION	0xA1		//�汾		
#define CST_ID_G_MODE 			0xA4   		//FT5206�ж�ģʽ���ƼĴ���
#define CST_ID_G_THGROUP		0x80   		//������Чֵ���üĴ���
#define CST_ID_G_PERIODACTIVE	0x88   		//����״̬�������üĴ���

#define	SCL_0	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0, GPIO_PIN_RESET)
#define SCL_1	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0, GPIO_PIN_SET)
#define	SDA_0	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_11, GPIO_PIN_RESET)
#define	SDA_1	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_11, GPIO_PIN_SET)
#define	SDA_in		HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_11)
#define	INT_in		HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)
#define RST_0		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13, GPIO_PIN_RESET)
#define	RST_1		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13, GPIO_PIN_SET)

void CT_SDA_OUT(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /*Configure GPIO pin : PF11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);	
}

void CT_SDA_IN(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /*Configure GPIO pin : PF11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
}

void CT_IIC_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /*Configure GPIO pin : PB0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  /*Configure GPIO pin : PF11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
}

//����I2C�ٶȵ���ʱ
void CT_Delay(void)
{
	delayus(5);
}

//����IIC��ʼ�ź�
void CT_IIC_Start(void)
{
	CT_SDA_OUT();     //sda�����
	SDA_1;	  	  
	SCL_1;
	delayus(30);
 	SDA_0;//START:when CLK is high,DATA change form high to low 
	CT_Delay();
	SCL_0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void CT_IIC_Stop(void)
{
	CT_SDA_OUT();//sda�����
	SCL_1;
	delayus(30);
	SDA_0;//STOP:when CLK is high DATA change form low to high
	CT_Delay();
	SDA_1;//����I2C���߽����ź�  
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 CT_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	CT_SDA_IN();      //SDA����Ϊ����  
	SDA_1;	   
	SCL_1;
	CT_Delay();
	while(SDA_in)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			CT_IIC_Stop();
			return 1;
		} 
		CT_Delay();
	}
	SCL_0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void CT_IIC_Ack(void)
{
	SCL_0;
	CT_SDA_OUT();
	CT_Delay();
	SDA_0;
	CT_Delay();
	SCL_1;
	CT_Delay();
	SCL_0;
}
//������ACKӦ��		    
void CT_IIC_NAck(void)
{
	SCL_0;
	CT_SDA_OUT();
	CT_Delay();
	SDA_1;
	CT_Delay();
	SCL_1;
	CT_Delay();
	SCL_0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void CT_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	CT_SDA_OUT(); 	    
    SCL_0;//����ʱ�ӿ�ʼ���ݴ���
	CT_Delay();
	for(t=0;t<8;t++)
    {              

			if(txd&0x80) SDA_1;
			else	SDA_0;
        txd<<=1; 	      
		SCL_1; 
		CT_Delay();
		SCL_0;	
		CT_Delay();
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 CT_IIC_Read_Byte(unsigned char ack)
{
	u8 i,receive=0;
 	CT_SDA_IN();//SDA����Ϊ����
	delayus(30);
	for(i=0;i<8;i++ )
	{ 
		SCL_0; 	    	   
		CT_Delay();
		SCL_1;	 
		receive<<=1;
		if(SDA_in)receive++;   
	}	  				 
	if (!ack)CT_IIC_NAck();//����nACK
	else CT_IIC_Ack(); //����ACK   
 	return receive;
}



//д��һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:д���ݳ���
//����ֵ:0,�ɹ�;1,ʧ��.
u8 CST716_WR_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 ret=0;
	CT_IIC_Start();	 
	CT_IIC_Send_Byte(CST_CMD_WR);	//����д���� 	 
	CT_IIC_Wait_Ack(); 	 										  		   
	CT_IIC_Send_Byte(reg&0XFF);   	//���͵�8λ��ַ
	CT_IIC_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
    	CT_IIC_Send_Byte(buf[i]);  	//������
		ret=CT_IIC_Wait_Ack();
		if(ret)break;  
	}
    CT_IIC_Stop();					//����һ��ֹͣ����	    
	return ret; 
}
//����һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:�����ݳ���			  
void CST716_RD_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(CST_CMD_WR);   	//����д���� 	 
	CT_IIC_Wait_Ack(); 	 										  		   
 	CT_IIC_Send_Byte(reg&0XFF);   	//���͵�8λ��ַ
	CT_IIC_Wait_Ack();  
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(CST_CMD_RD);   	//���Ͷ�����		   
	CT_IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
    	buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); //������	  
	} 
    CT_IIC_Stop();//����һ��ֹͣ����     
} 

u8 CIP[5]; //������Ŵ���IC
//��ʼ��������
//����ֵ:0,��ʼ���ɹ�;1,��ʼ��ʧ�� 
u8 CST716_Init(void)
{
	u8 temp[5];  		

	CT_IIC_Init();      	//��ʼ����������I2C����  
	RST_0;				//��λ
	HAL_Delay(20);
 	RST_1;				//�ͷŸ�λ		    
	HAL_Delay(50);  
	
	CST716_RD_Reg(0xA7,&temp[0],1);
//	printf("temp[0]=%d\r\n",temp[0]);
	
	temp[0]=0;
	CST716_WR_Reg(CST_DEVIDE_MODE,temp,1);	//������������ģʽ 
	CST716_WR_Reg(CST_ID_G_MODE,temp,1);		//��ѯģʽ 
	temp[0]=22;								//������Чֵ��22��ԽСԽ����	
	CST716_WR_Reg(CST_ID_G_THGROUP,temp,1);	//���ô�����Чֵ
	temp[0]=12;								//�������ڣ�����С��12�����14
	CST716_WR_Reg(CST_ID_G_PERIODACTIVE,temp,1); 

    return 0;
}

const u16 CST716_TPX_TBL[5]={CST_TP1_REG,CST_TP2_REG};
//u8 g_gt_tnum=1;      //Ĭ��֧�ֵĴ���������(2�㴥��)

//ɨ�败����(���ò�ѯ��ʽ)
//mode:0,����ɨ��.
//����ֵ:��ǰ����״̬.
//0,�����޴���;1,�����д���
u8 CST716_Scan(u8 mode)
{
	u8 buf[4];
	CST716_RD_Reg(CST_REG_NUM_FINGER,&mode,1);//��ȡ�������״̬  
	if((mode&0X1F))
	{
		HAL_Delay(3);    
		CST716_RD_Reg(CST_TP1_REG,buf,4);	
		tp_dev.x[0]=(((u16)(buf[0]&0X0F)<<8)+buf[1]);
		tp_dev.y[0]=((u16)(buf[2]&0X0F)<<8)+buf[3];			
		if(tp_dev.x[0]<480||tp_dev.y[0]<800)  //�Ϸ�����
		{
			return 1;
		}
	}
	tp_dev.x[0]=0xffff;
	tp_dev.y[0]=0xffff;
	return 0;
}








