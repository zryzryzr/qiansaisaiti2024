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
//默认为touchtype=0的数据.
u8 CMD_RDX=0XD0;
u8 CMD_RDY=0X90;

//SPI写数据
//向触摸屏IC写入1byte数据    
//num:要写入的数据
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
		TCLK_1;		//上升沿有效	        
	}		 			    
} 		 
//SPI读数据 
//从触摸屏IC读取adc值
//CMD:指令
//返回值:读到的数据	   
u16 TP_Read_AD(u8 CMD)	  
{ 	 
	u8 count=0; 	  
	u16 Num=0; 
	TCLK_0;		//先拉低时钟 	 
	TDIN_0; 	//拉低数据线
	TCS_0; 		//选中触摸屏IC
	TP_Write_Byte(CMD);//发送命令字
	delayus(2);//ADS7846的转换时间最长为6us
	TCLK_0; 	     	    
	delayus(2);    	   
	TCLK_1;		//给1个时钟，清除BUSY
	delayus(2);    
	TCLK_0; 	     	    
	for(count=0;count<16;count++)//读出16位数据,只有高12位有效 
	{ 				  
		Num<<=1; 	 
		TCLK_0;	//下降沿有效  	    	   
		delayus(2);    
 		TCLK_1;
 		if(DOUT)Num++; 		 
	}  	
	Num>>=4;   	//只有高12位有效.
	TCS_1;		//释放片选	 
	return(Num);   
}

//读取一个坐标值(x或者y)
//连续读取READ_TIMES次数据,对这些数据升序排列,
//然后去掉最低和最高LOST_VAL个数,取平均值 
//xy:指令（CMD_RDX/CMD_RDY）
//返回值:读到的数据
#define READ_TIMES 40 	//读取次数
#define LOST_VAL 1	  	//丢弃值
u16 TP_Read_XOY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u32 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)buf[i]=TP_Read_AD(xy);		 		    
	for(i=0;i<READ_TIMES-1; i++)//排序
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//升序排列
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

//读取x,y坐标
//最小值不能少于100.
//x,y:读取到的坐标值
//返回值:0,失败;1,成功。
u8 TP_Read_XY(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=TP_Read_XOY(CMD_RDX);
	ytemp=TP_Read_XOY(CMD_RDY);	  												   
	//if(xtemp<100||ytemp<100)return 0;//读数失败
	*x=xtemp;
	*y=ytemp;
	return 1;//读数成功
}
//连续2次读取触摸屏IC,且这两次的偏差不能超过
//ERR_RANGE,满足条件,则认为读数正确,否则读数错误.	   
//该函数能大大提高准确度
//x,y:读取到的坐标值
//返回值:0,失败;1,成功。
#define ERR_RANGE 50 //误差范围 
u8 TP_Read_XY2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;    
    flag=TP_Read_XY(&x1,&y1);   
    if(flag==0)return(0);
    flag=TP_Read_XY(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//前后两次采样在+-50内
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
}   
//////////////////////////////////////////////////////////////////////////////////		  
//与LCD部分有关的函数  
//画一个触摸点
//用来校准用的
//x,y:坐标
//color:颜色
void TP_Drow_Touch_Point(u16 x,u16 y,u16 color)
{
	Touch_Test_Point(x, y,color);
}	  
//画一个大点(2*2的点)		   
//x,y:坐标
//color:颜色
void TP_Draw_Big_Point(u16 x,u16 y,u16 color)
{	    
	Touch_Big_Point(x,y,color); 	  	
}							  
//////////////////////////////////////////////////////////////////////////////////		  
//触摸按键扫描
//tp:0,屏幕坐标;1,物理坐标(校准等特殊场合用)
//返回值:当前触屏状态.
//0,触屏无触摸;1,触屏有触摸
u8 TP_Scan(u8 tp)
{			   

	return (CST716_Scan(0));

}  
 
//提示校准结果(各个参数)
void TP_Adj_Info_Show(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2,u16 x3,u16 y3,u16 fac)
{	  
}
		 
//触摸屏校准代码
//得到四个校准参数
void Touch_Adjust(void)
{								 

}
//触摸屏初始化  		    
//返回值:0,进行过校准
//       1,未进行校准
u8 TOUCH_init(void)
{
	CST716_Init();
	return 0;
								 
}

/**************************CST716*******************************/
//I2C读写命令	
#define CST_CMD_WR 				0X2A    	//写命令
#define CST_CMD_RD 				0X2B		//读命令
  
//部分寄存器定义 
#define CST_DEVIDE_MODE 		0x00   		//模式控制寄存器
#define CST_REG_NUM_FINGER      0x02		//触摸状态寄存器

#define CST_TP1_REG 			0X03	  	//第一个触摸点数据地址
#define CST_TP2_REG 			0X09		//第二个触摸点数据地址
 
#define	CST_ID_G_LIB_VERSION	0xA1		//版本		
#define CST_ID_G_MODE 			0xA4   		//FT5206中断模式控制寄存器
#define CST_ID_G_THGROUP		0x80   		//触摸有效值设置寄存器
#define CST_ID_G_PERIODACTIVE	0x88   		//激活状态周期设置寄存器

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

//控制I2C速度的延时
void CT_Delay(void)
{
	delayus(5);
}

//产生IIC起始信号
void CT_IIC_Start(void)
{
	CT_SDA_OUT();     //sda线输出
	SDA_1;	  	  
	SCL_1;
	delayus(30);
 	SDA_0;//START:when CLK is high,DATA change form high to low 
	CT_Delay();
	SCL_0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void CT_IIC_Stop(void)
{
	CT_SDA_OUT();//sda线输出
	SCL_1;
	delayus(30);
	SDA_0;//STOP:when CLK is high DATA change form low to high
	CT_Delay();
	SDA_1;//发送I2C总线结束信号  
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 CT_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	CT_SDA_IN();      //SDA设置为输入  
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
	SCL_0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
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
//不产生ACK应答		    
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
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void CT_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	CT_SDA_OUT(); 	    
    SCL_0;//拉低时钟开始数据传输
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
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 CT_IIC_Read_Byte(unsigned char ack)
{
	u8 i,receive=0;
 	CT_SDA_IN();//SDA设置为输入
	delayus(30);
	for(i=0;i<8;i++ )
	{ 
		SCL_0; 	    	   
		CT_Delay();
		SCL_1;	 
		receive<<=1;
		if(SDA_in)receive++;   
	}	  				 
	if (!ack)CT_IIC_NAck();//发送nACK
	else CT_IIC_Ack(); //发送ACK   
 	return receive;
}



//写入一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:写数据长度
//返回值:0,成功;1,失败.
u8 CST716_WR_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 ret=0;
	CT_IIC_Start();	 
	CT_IIC_Send_Byte(CST_CMD_WR);	//发送写命令 	 
	CT_IIC_Wait_Ack(); 	 										  		   
	CT_IIC_Send_Byte(reg&0XFF);   	//发送低8位地址
	CT_IIC_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
    	CT_IIC_Send_Byte(buf[i]);  	//发数据
		ret=CT_IIC_Wait_Ack();
		if(ret)break;  
	}
    CT_IIC_Stop();					//产生一个停止条件	    
	return ret; 
}
//读出一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:读数据长度			  
void CST716_RD_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(CST_CMD_WR);   	//发送写命令 	 
	CT_IIC_Wait_Ack(); 	 										  		   
 	CT_IIC_Send_Byte(reg&0XFF);   	//发送低8位地址
	CT_IIC_Wait_Ack();  
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(CST_CMD_RD);   	//发送读命令		   
	CT_IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
    	buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); //发数据	  
	} 
    CT_IIC_Stop();//产生一个停止条件     
} 

u8 CIP[5]; //用来存放触摸IC
//初始化触摸屏
//返回值:0,初始化成功;1,初始化失败 
u8 CST716_Init(void)
{
	u8 temp[5];  		

	CT_IIC_Init();      	//初始化电容屏的I2C总线  
	RST_0;				//复位
	HAL_Delay(20);
 	RST_1;				//释放复位		    
	HAL_Delay(50);  
	
	CST716_RD_Reg(0xA7,&temp[0],1);
//	printf("temp[0]=%d\r\n",temp[0]);
	
	temp[0]=0;
	CST716_WR_Reg(CST_DEVIDE_MODE,temp,1);	//进入正常操作模式 
	CST716_WR_Reg(CST_ID_G_MODE,temp,1);		//查询模式 
	temp[0]=22;								//触摸有效值，22，越小越灵敏	
	CST716_WR_Reg(CST_ID_G_THGROUP,temp,1);	//设置触摸有效值
	temp[0]=12;								//激活周期，不能小于12，最大14
	CST716_WR_Reg(CST_ID_G_PERIODACTIVE,temp,1); 

    return 0;
}

const u16 CST716_TPX_TBL[5]={CST_TP1_REG,CST_TP2_REG};
//u8 g_gt_tnum=1;      //默认支持的触摸屏点数(2点触摸)

//扫描触摸屏(采用查询方式)
//mode:0,正常扫描.
//返回值:当前触屏状态.
//0,触屏无触摸;1,触屏有触摸
u8 CST716_Scan(u8 mode)
{
	u8 buf[4];
	CST716_RD_Reg(CST_REG_NUM_FINGER,&mode,1);//读取触摸点的状态  
	if((mode&0X1F))
	{
		HAL_Delay(3);    
		CST716_RD_Reg(CST_TP1_REG,buf,4);	
		tp_dev.x[0]=(((u16)(buf[0]&0X0F)<<8)+buf[1]);
		tp_dev.y[0]=((u16)(buf[2]&0X0F)<<8)+buf[3];			
		if(tp_dev.x[0]<480||tp_dev.y[0]<800)  //合法数据
		{
			return 1;
		}
	}
	tp_dev.x[0]=0xffff;
	tp_dev.y[0]=0xffff;
	return 0;
}








