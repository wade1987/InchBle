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
	SDA_OUT();     //sda�����
	WRITE_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(2);
 	WRITE_SDA=0;
	delay_us(2);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void LDC_IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	WRITE_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(2);
	IIC_SCL=1; 
	WRITE_SDA=1;//����I2C���߽����ź�
	delay_us(2);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
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
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(unsigned char txd)
{                        
    unsigned char t;   
	  SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {                   
		if((txd<<t)&0x80)
			WRITE_SDA=1;
		else
			WRITE_SDA=0; 
		delay_us(2);   //��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
		
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
unsigned char IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}




/*******************************************************************************
* Function Name  :  I2c_StartCondition
* Description    :  ģ��IIC ����
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
* Description    :  ģ��IIC ֹͣ
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
* Description    :  IIC�����ֽ�
* Input          :  u8 ucSend_Byte Ҫ���͵�����
* Output         :  None
* Return         :  ���ͳɹ�����ʧ��
*******************************************************************************/
unsigned char I2c_WriteByte (unsigned char ucSend_Byte)
{
    unsigned char ucMark;
    unsigned char ucError = 0;

    // ѭ������
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

        // ��ʱˢ��ʱ��
        delay_us(6 >> 2); //
        IIC_SCL=1; 
        delay_us(6 >> 1); // 
        IIC_SCL=0; 
        delay_us(6 >> 2); // 
    } 

    // �ͷ�����
    WRITE_SDA=1;

    // �����ھŸ�ʱ��
    IIC_SCL=1;
    delay_us(6 >> 2);      // Delay_us(TEMP_HUMI_CLK_DELAY);

	
	SDA_IN();
    // ���Ӧ��
    if(READ_SDA == 1)
    {
        ucError = ACK_ERROR;
    }   
    IIC_SCL=0; 
    
    delay_us(6 << 1);     // (TEMP_HUMI_CLK_DELAY << 2);

	SDA_OUT();

    // ���ش����� 
    return ucError; 
    
}// End of u8 I2c_WriteByte (u8 ucSend_Byte)

/*******************************************************************************
* Function Name  :  I2c_ReadByte
* Description    :  IIC�����ֽ�
* Input          :  u8 ucACK �Ƿ���ҪӦ��
* Output         :  None
* Return         :  ��ȡ����ֵ
*******************************************************************************/
unsigned char I2c_ReadByte (unsigned char ucACK)
{
    unsigned char ucMask, ucRead_Data = 0;
	
	SDA_IN();

    // �ͷ�����
    WRITE_SDA=1;

    // ��ȡ����
    for (ucMask = 0x80; ucMask > 0; ucMask >>= 1)  
    { 
        // ����ʱ�Ӷ�ȡ����
        IIC_SCL=1; 
        delay_us(6 >> 1);  // TEMP_HUMI_CLK_DELAY
        if(READ_SDA == 1)
        {
            ucRead_Data = (ucRead_Data | ucMask);
        }   

        // ����ʱ��׼������
        IIC_SCL=0; 
        delay_us(6 >> 2);  // TEMP_HUMI_CLK_DELAY
    }

	SDA_OUT();

    // ����Ӧ��λ
    if(ucACK)
    {
    	WRITE_SDA=1;//TEMP_HUMI_SDA = ucACK;
    }
	else
	{
		WRITE_SDA=0;
	}
      
    delay_us(6 >> 2); // TEMP_HUMI_CLK_DELAY 

    // Ӧ��λʱ��
    IIC_SCL=1; 
    delay_us(6 >> 1); // TEMP_HUMI_CLK_DELAY 
    IIC_SCL=0; 

    // �ͷ�����
    WRITE_SDA=1;
    delay_us(6 << 1); // TEMP_HUMI_CLK_DELAY << 1

    // ������ֵ
    return ucRead_Data; 
    
}// End of u8 I2c_ReadByte (u8 ucACK)























