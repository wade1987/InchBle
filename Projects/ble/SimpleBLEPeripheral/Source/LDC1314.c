#include "LDC1314.h"
/*******************************************************************************
*�ļ���     : Device_LDC1314.c	
*����       : LDC1314����
*ԭ��       : ����I2C����ͨ��            
********************************************************************************
*�汾   V1.0    
*����   ADZ          
*����   2016/09/08         
*˵��   ���Ե�ʱ�򣬿��Խ�����ļĴ���ע�ͣ�������ֵ���¼������
*******************************************************************************/




/*******************************************************************************
* ������	: WriteRegLDC 
* ����	    : д�Ĵ�������
* �������  : add��value
* ���ز���  : ��
*******************************************************************************/
void WriteRegLDC(unsigned char add,unsigned int value,unsigned char chip_addr)
{
    I2c_StartCondition(); 
    I2c_WriteByte((chip_addr << 1) + 0);	 //ADDR=0ʱ����ַ0X2A<<1+0=0X54  
    I2c_WriteByte(add);      //д��ַ
    I2c_WriteByte(value>>8); //д��8λ
    I2c_WriteByte(value&0xff);//д��8λ
    I2c_StopCondition();               //����һ��ֹͣ���� 
    delay_ms(1);	 
}
/*******************************************************************************
* ������	: ReadRegLDC 
* ����	    : ���Ĵ�������
* �������  : add��ַ
* ���ز���  : ��
*******************************************************************************/
unsigned int ReadRegLDC(unsigned char add,unsigned char chip_addr)
{
    unsigned int status;
    unsigned int a,b;
	
    I2c_StartCondition(); 
    I2c_WriteByte((chip_addr << 1) + 0);	   //д����ADDR=0
    I2c_WriteByte(add);     //��ַ
    I2c_StartCondition();            //���¿�ʼ
    I2c_WriteByte((chip_addr << 1) + 1);	   //���Ͷ�����ADDR=0
    a=I2c_ReadByte(0);     //����λ
    b=I2c_ReadByte(1);		 //����λ
    status=(a<<8)+b;
    I2c_StopCondition();
    return (status);
}
/*******************************************************************************
* ������	: InitSingleLDC1314 
* ����	    : ��ʼ����ͨ��
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
void InitSingleLDC1314(unsigned char chip_addr)
{
    WriteRegLDC(0x08,0x04D6,chip_addr);//����ת��ʱ��

    WriteRegLDC(0x10,0x000A,chip_addr);

    WriteRegLDC(0x14,0x2002,chip_addr); //��Ƶϴϵ��
    //    LDC_Write_Reg(0x0C,0x0F00,chip_addr); //����
    WriteRegLDC(0x19,0x0000,chip_addr); //ERROE_CONFIG
    WriteRegLDC(0x1B,0x020D,chip_addr); //MUX_CONFIG��ͨ��/��ͨ������0000 0010 0000 1100
    WriteRegLDC(0x1E,0x9000,chip_addr); //������������
    WriteRegLDC(0x1A,0x1400,chip_addr);
}
/*******************************************************************************
* ������	: InitMultiLDC1314 
* ����	    : ��ʼ����ͨ��
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
void InitMultiLDC1314(unsigned char chip_addr)
{
	
    WriteRegLDC(LDC1314_CMD_REF_COUNT_CH0,0xee48,chip_addr);//ת��ʱ��
    WriteRegLDC(LDC1314_CMD_REF_COUNT_CH1,0xee48,chip_addr);
	WriteRegLDC(LDC1314_CMD_REF_COUNT_CH2,0xee48,chip_addr);
	WriteRegLDC(LDC1314_CMD_REF_COUNT_CH3,0xee48,chip_addr);

    WriteRegLDC(LDC1314_CMD_SETTLE_COUNT_CH0,0x0400,chip_addr);//����ʱ��
    WriteRegLDC(LDC1314_CMD_SETTLE_COUNT_CH1,0x0400,chip_addr);
	WriteRegLDC(LDC1314_CMD_SETTLE_COUNT_CH2,0x0400,chip_addr);
	WriteRegLDC(LDC1314_CMD_SETTLE_COUNT_CH3,0x0400,chip_addr);

    WriteRegLDC(LDC1314_CMD_CLOCK_DIVIDERS_CH0,0x1001,chip_addr);//��Ƶ
    WriteRegLDC(LDC1314_CMD_CLOCK_DIVIDERS_CH1,0x1001,chip_addr);
	WriteRegLDC(LDC1314_CMD_CLOCK_DIVIDERS_CH2,0x1001,chip_addr);
	WriteRegLDC(LDC1314_CMD_CLOCK_DIVIDERS_CH3,0x1001,chip_addr); 

    WriteRegLDC(LDC1314_CMD_ERROR_CONFIG,0x0000,chip_addr);
    WriteRegLDC(LDC1314_CMD_MUX_CONFIG,0xC20C,chip_addr);//���ö�ͨ��
    //WriteRegLDC(LDC1314_CMD_MUX_CONFIG,0x820D,chip_addr);//���ö�ͨ��

    WriteRegLDC(LDC1314_CMD_DRIVE_CURRENT_CH0,0x8c40,chip_addr); //��������
    WriteRegLDC(LDC1314_CMD_DRIVE_CURRENT_CH1,0x8c40,chip_addr);
	WriteRegLDC(LDC1314_CMD_DRIVE_CURRENT_CH2,0x8c40,chip_addr);
	WriteRegLDC(LDC1314_CMD_DRIVE_CURRENT_CH3,0x8c40,chip_addr);

    WriteRegLDC(LDC1314_CMD_OFFSET_CH0,0x0000,chip_addr); //��0ֵ
    WriteRegLDC(LDC1314_CMD_OFFSET_CH1,0x0000,chip_addr); 
	WriteRegLDC(LDC1314_CMD_OFFSET_CH2,0x0000,chip_addr); 		
	WriteRegLDC(LDC1314_CMD_OFFSET_CH3,0x0000,chip_addr); 

	WriteRegLDC(LDC1314_CMD_RESET_DEVICE,0x0000,chip_addr); //gain=1	0 bit shiftb
    WriteRegLDC(LDC1314_CMD_CONFIG,0x1e01,chip_addr);
}

