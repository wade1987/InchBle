#include "LDC1314.h"
/*******************************************************************************
*文件名     : Device_LDC1314.c	
*作用       : LDC1314驱动
*原理       : 采用I2C总线通信            
********************************************************************************
*版本   V1.0    
*作者   ADZ          
*日期   2016/09/08         
*说明   测试的时候，可以将调零的寄存器注释，看读的值重新计算调零
*******************************************************************************/




/*******************************************************************************
* 函数名	: WriteRegLDC 
* 描述	    : 写寄存器数据
* 输入参数  : add，value
* 返回参数  : 无
*******************************************************************************/
void WriteRegLDC(unsigned char add,unsigned int value,unsigned char chip_addr)
{
    I2c_StartCondition(); 
    I2c_WriteByte((chip_addr << 1) + 0);	 //ADDR=0时，地址0X2A<<1+0=0X54  
    I2c_WriteByte(add);      //写地址
    I2c_WriteByte(value>>8); //写高8位
    I2c_WriteByte(value&0xff);//写低8位
    I2c_StopCondition();               //产生一个停止条件 
    delay_ms(1);	 
}
/*******************************************************************************
* 函数名	: ReadRegLDC 
* 描述	    : 读寄存器数据
* 输入参数  : add地址
* 返回参数  : 无
*******************************************************************************/
unsigned int ReadRegLDC(unsigned char add,unsigned char chip_addr)
{
    unsigned int status;
    unsigned int a,b;
	
    I2c_StartCondition(); 
    I2c_WriteByte((chip_addr << 1) + 0);	   //写命令ADDR=0
    I2c_WriteByte(add);     //地址
    I2c_StartCondition();            //重新开始
    I2c_WriteByte((chip_addr << 1) + 1);	   //发送读命令ADDR=0
    a=I2c_ReadByte(0);     //读高位
    b=I2c_ReadByte(1);		 //读低位
    status=(a<<8)+b;
    I2c_StopCondition();
    return (status);
}
/*******************************************************************************
* 函数名	: InitSingleLDC1314 
* 描述	    : 初始化单通道
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/
void InitSingleLDC1314(unsigned char chip_addr)
{
    WriteRegLDC(0x08,0x04D6,chip_addr);//设置转换时间

    WriteRegLDC(0x10,0x000A,chip_addr);

    WriteRegLDC(0x14,0x2002,chip_addr); //分频洗系数
    //    LDC_Write_Reg(0x0C,0x0F00,chip_addr); //调零
    WriteRegLDC(0x19,0x0000,chip_addr); //ERROE_CONFIG
    WriteRegLDC(0x1B,0x020D,chip_addr); //MUX_CONFIG多通道/单通道配置0000 0010 0000 1100
    WriteRegLDC(0x1E,0x9000,chip_addr); //配置驱动电流
    WriteRegLDC(0x1A,0x1400,chip_addr);
}
/*******************************************************************************
* 函数名	: InitMultiLDC1314 
* 描述	    : 初始化多通道
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/
void InitMultiLDC1314(unsigned char chip_addr)
{
	
    WriteRegLDC(LDC1314_CMD_REF_COUNT_CH0,0xee48,chip_addr);//转换时间
    WriteRegLDC(LDC1314_CMD_REF_COUNT_CH1,0xee48,chip_addr);
	WriteRegLDC(LDC1314_CMD_REF_COUNT_CH2,0xee48,chip_addr);
	WriteRegLDC(LDC1314_CMD_REF_COUNT_CH3,0xee48,chip_addr);

    WriteRegLDC(LDC1314_CMD_SETTLE_COUNT_CH0,0x0400,chip_addr);//作用时间
    WriteRegLDC(LDC1314_CMD_SETTLE_COUNT_CH1,0x0400,chip_addr);
	WriteRegLDC(LDC1314_CMD_SETTLE_COUNT_CH2,0x0400,chip_addr);
	WriteRegLDC(LDC1314_CMD_SETTLE_COUNT_CH3,0x0400,chip_addr);

    WriteRegLDC(LDC1314_CMD_CLOCK_DIVIDERS_CH0,0x1001,chip_addr);//分频
    WriteRegLDC(LDC1314_CMD_CLOCK_DIVIDERS_CH1,0x1001,chip_addr);
	WriteRegLDC(LDC1314_CMD_CLOCK_DIVIDERS_CH2,0x1001,chip_addr);
	WriteRegLDC(LDC1314_CMD_CLOCK_DIVIDERS_CH3,0x1001,chip_addr); 

    WriteRegLDC(LDC1314_CMD_ERROR_CONFIG,0x0000,chip_addr);
    WriteRegLDC(LDC1314_CMD_MUX_CONFIG,0xC20C,chip_addr);//配置多通道
    //WriteRegLDC(LDC1314_CMD_MUX_CONFIG,0x820D,chip_addr);//配置多通道

    WriteRegLDC(LDC1314_CMD_DRIVE_CURRENT_CH0,0x8c40,chip_addr); //驱动电流
    WriteRegLDC(LDC1314_CMD_DRIVE_CURRENT_CH1,0x8c40,chip_addr);
	WriteRegLDC(LDC1314_CMD_DRIVE_CURRENT_CH2,0x8c40,chip_addr);
	WriteRegLDC(LDC1314_CMD_DRIVE_CURRENT_CH3,0x8c40,chip_addr);

    WriteRegLDC(LDC1314_CMD_OFFSET_CH0,0x0000,chip_addr); //调0值
    WriteRegLDC(LDC1314_CMD_OFFSET_CH1,0x0000,chip_addr); 
	WriteRegLDC(LDC1314_CMD_OFFSET_CH2,0x0000,chip_addr); 		
	WriteRegLDC(LDC1314_CMD_OFFSET_CH3,0x0000,chip_addr); 

	WriteRegLDC(LDC1314_CMD_RESET_DEVICE,0x0000,chip_addr); //gain=1	0 bit shiftb
    WriteRegLDC(LDC1314_CMD_CONFIG,0x1e01,chip_addr);
}

