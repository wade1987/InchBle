#include "myiic.h"
 


void delay_us(int value)
{
	int i, j;
	for(i = 0 ; i < 6 ; i++)
	{
		for(j = 0 ; j < value ; j++)
			;
	}
}

void delay_ms(int value)
{
	  int i,j;
  for(i=value;i>0;i--)
    for(j=587;j>0;j--);
}


void LDC_IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	WRITE_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(2);
 	WRITE_SDA=0;
	delay_us(2);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void LDC_IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	WRITE_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(2);
	IIC_SCL=1; 
	WRITE_SDA=1;//发送I2C总线结束信号
	delay_us(2);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
unsigned char IIC_Wait_Ack(void)
{
	unsigned char ucErrTime=0;
	
	SDA_OUT();     
	WRITE_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);
	SDA_IN(); 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			LDC_IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	WRITE_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	WRITE_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(unsigned char txd)
{                        
    unsigned char t;   
	  SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {                   
		if((txd<<t)&0x80)
			WRITE_SDA=1;
		else
			WRITE_SDA=0; 
		delay_us(2);   //这三个延时都是必须的
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
		
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
unsigned char IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(2); 
		IIC_SCL=0;
		delay_us(2); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}




/*******************************************************************************
* Function Name  :  I2c_StartCondition
* Description    :  模拟IIC 启动
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void I2c_StartCondition(void)
{
    WRITE_SDA=1;
    IIC_SCL=1; 
    delay_us(6); 
    WRITE_SDA=0;
    delay_us(6);  
    IIC_SCL=0; 
    delay_us(6);
    
}// End of void I2c_StartCondition(void)

/*******************************************************************************
* Function Name  :  I2c_StopCondition
* Description    :  模拟IIC 停止
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void I2c_StopCondition(void)
{
	WRITE_SDA=0;
    IIC_SCL=0; 
    delay_us(6); 
    IIC_SCL=1;
    delay_us(6);  
    WRITE_SDA=1; 
    delay_us(6);
}// End of void I2c_StopCondition(void)

/*******************************************************************************
* Function Name  :  I2c_WriteByte
* Description    :  IIC发送字节
* Input          :  u8 ucSend_Byte 要发送的数据
* Output         :  None
* Return         :  发送成功或者失败
*******************************************************************************/
unsigned char I2c_WriteByte (unsigned char ucSend_Byte)
{
    unsigned char ucMark;
    unsigned char ucError = 0;

    // 循环发送
    for(ucMark = 0x80; ucMark > 0; ucMark >>= 1)  
    { 
        if((ucMark & ucSend_Byte) == 0) 
        {
            WRITE_SDA=0;
        }   
        else 
        {
            WRITE_SDA=1;
        }

        // 延时刷新时钟
        delay_us(6 >> 2); //
        IIC_SCL=1; 
        delay_us(6 >> 1); // 
        IIC_SCL=0; 
        delay_us(6 >> 2); // 
    } 

    // 释放总线
    WRITE_SDA=1;

    // 产生第九个时钟
    IIC_SCL=1;
    delay_us(6 >> 2);      // Delay_us(TEMP_HUMI_CLK_DELAY);

	
	SDA_IN();
    // 检查应答
    if(READ_SDA == 1)
    {
        ucError = ACK_ERROR;
    }   
    IIC_SCL=0; 
    
    delay_us(6 << 1);     // (TEMP_HUMI_CLK_DELAY << 2);

	SDA_OUT();

    // 返回错误码 
    return ucError; 
    
}// End of u8 I2c_WriteByte (u8 ucSend_Byte)

/*******************************************************************************
* Function Name  :  I2c_ReadByte
* Description    :  IIC接收字节
* Input          :  u8 ucACK 是否需要应答
* Output         :  None
* Return         :  读取的数值
*******************************************************************************/
unsigned char I2c_ReadByte (unsigned char ucACK)
{
    unsigned char ucMask, ucRead_Data = 0;
	
	SDA_IN();

    // 释放总线
    WRITE_SDA=1;

    // 读取数据
    for (ucMask = 0x80; ucMask > 0; ucMask >>= 1)  
    { 
        // 拉高时钟读取数据
        IIC_SCL=1; 
        delay_us(6 >> 1);  // TEMP_HUMI_CLK_DELAY
        if(READ_SDA == 1)
        {
            ucRead_Data = (ucRead_Data | ucMask);
        }   

        // 拉低时钟准备数据
        IIC_SCL=0; 
        delay_us(6 >> 2);  // TEMP_HUMI_CLK_DELAY
    }

	SDA_OUT();

    // 发送应答位
    if(ucACK)
    {
    	WRITE_SDA=1;//TEMP_HUMI_SDA = ucACK;
    }
	else
	{
		WRITE_SDA=0;
	}
      
    delay_us(6 >> 2); // TEMP_HUMI_CLK_DELAY 

    // 应答位时钟
    IIC_SCL=1; 
    delay_us(6 >> 1); // TEMP_HUMI_CLK_DELAY 
    IIC_SCL=0; 

    // 释放总线
    WRITE_SDA=1;
    delay_us(6 << 1); // TEMP_HUMI_CLK_DELAY << 1

    // 返回数值
    return ucRead_Data; 
    
}// End of u8 I2c_ReadByte (u8 ucACK)























