#ifndef __MYIIC_H
#define __MYIIC_H
#include "inch.h"


#define     CHECKSUM_ERROR              0x80                            // 校验错误
#define     TIME_OUT_ERROR              0x40                            // 超时错误
#define     ACK_ERROR                   0x20                            // 无应答
#define     NO_ERROR                    0x00                            // 无错误



//IIC所有操作函数
//void IIC_Init(void);                //初始化IIC的IO口				 
void LDC_IIC_Start(void);				        //发送IIC开始信号
void LDC_IIC_Stop(void);	  			      //发送IIC停止信号
void IIC_Send_Byte(unsigned char txd);			    //IIC发送一个字节
unsigned char IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
unsigned char IIC_Wait_Ack(void); 				      //IIC等待ACK信号
void IIC_Ack(void);					        //IIC发送ACK信号
void IIC_NAck(void);				        //IIC不发送ACK信号

void IIC_Write_One_Byte(unsigned char daddr,unsigned char addr,unsigned char data);
unsigned char IIC_Read_One_Byte(unsigned char daddr,unsigned char addr);	  
void delay_us(int value);
void delay_ms(int value);

void I2c_StartCondition(void);
void I2c_StopCondition(void);
unsigned char I2c_WriteByte (unsigned char ucSend_Byte);
unsigned char I2c_ReadByte (unsigned char ucACK);





#endif
















