#ifndef __MYIIC_H
#define __MYIIC_H
#include "inch.h"


#define     CHECKSUM_ERROR              0x80                            // У�����
#define     TIME_OUT_ERROR              0x40                            // ��ʱ����
#define     ACK_ERROR                   0x20                            // ��Ӧ��
#define     NO_ERROR                    0x00                            // �޴���



//IIC���в�������
//void IIC_Init(void);                //��ʼ��IIC��IO��				 
void LDC_IIC_Start(void);				        //����IIC��ʼ�ź�
void LDC_IIC_Stop(void);	  			      //����IICֹͣ�ź�
void IIC_Send_Byte(unsigned char txd);			    //IIC����һ���ֽ�
unsigned char IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
unsigned char IIC_Wait_Ack(void); 				      //IIC�ȴ�ACK�ź�
void IIC_Ack(void);					        //IIC����ACK�ź�
void IIC_NAck(void);				        //IIC������ACK�ź�

void IIC_Write_One_Byte(unsigned char daddr,unsigned char addr,unsigned char data);
unsigned char IIC_Read_One_Byte(unsigned char daddr,unsigned char addr);	  
void delay_us(int value);
void delay_ms(int value);

void I2c_StartCondition(void);
void I2c_StopCondition(void);
unsigned char I2c_WriteByte (unsigned char ucSend_Byte);
unsigned char I2c_ReadByte (unsigned char ucACK);





#endif
















