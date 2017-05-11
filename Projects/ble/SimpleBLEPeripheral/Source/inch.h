/*********************************************************************************************************
*                                 Inch.h  头文件函数
*
* 文件名	: Inch.h
* 版本号	: V1.00
* 作者	: 张强
*             
*********************************************************************************************************/

#ifndef __INCH_H
#define __INCH_H 

#include<ioCC2540.h>
#include "hal_types.h"
#include "hal_board.h"




/* ------------------------------------------------------------------------------------------------
 *                                      各种软件相关宏定义
 * ------------------------------------------------------------------------------------------------
 */

#define INCH_MAIN_TICK_TIME 50  // ms
#define NODE_NUM			10


/* ------------------------------------------------------------------------------------------------
 *                                      函数引用申明
 * ------------------------------------------------------------------------------------------------
 */

void InchMain(void);
void SN74HC595_OUT(uint16 a);
uint8 ParseRxData( uint8 *buf, uint16 len );
void UploadBLEData(uint8 node_idx);




/* ------------------------------------------------------------------------------------------------
 *                                     变量引用申明
 * ------------------------------------------------------------------------------------------------
 */

extern uint16 NodeLen[NODE_NUM];



/* ------------------------------------------------------------------------------------------------
 *                                      结构体定义(移植MCU时需调整)
 * ------------------------------------------------------------------------------------------------
 */

typedef signed   char   t_int_8;     //!< Signed 8 bit integer
typedef unsigned char   t_uint_8;    //!< Unsigned 8 bit integer

typedef signed   short  t_int_16;    //!< Signed 16 bit integer
typedef unsigned short  t_uint_16;   //!< Unsigned 16 bit integer

typedef signed   long   t_int_32;    //!< Signed 32 bit integer
typedef unsigned long   t_uint_32;   //!< Unsigned 32 bit integer 



/* ------------------------------------------------------------------------------------------------
 *                                      各种硬件相关宏定义（移植硬件平台时需调整）
 * ------------------------------------------------------------------------------------------------
 */

//-----------------------------IO口定义
//LED IO操作
#define LED_INCH P1_0    
#define LED_ON() LED_INCH = 1
#define LED_OFF() LED_INCH = 0

#define LED_INCH1 P0_1    
#define LED1_ON() LED_INCH1 = 1
#define LED1_OFF() LED_INCH1 = 0

#define LED_ALL_ON() LED_ON();LED1_ON()
#define LED_ALL_OFF() LED_OFF();LED1_OFF()



//KEY IO操作
#define KEY_INCH P0_0    
#define READ_KEY() KEY_INCH


//CD4051  IO 定义
#define MUX_A			P1_5
#define MUX_B			P1_6
#define MUX_C			P1_7
#define INH0	P1_4 
#define INH1	P2_0 


//选通Rx通道的操作
#define SEL_RX0() INH1 = 1 ; INH0 = 0 ; MUX_A = 0; MUX_B = 0; MUX_C = 0
#define SEL_RX1() INH1 = 1 ; INH0 = 0 ; MUX_A = 1; MUX_B = 0; MUX_C = 0
#define SEL_RX2() INH1 = 1 ; INH0 = 0 ; MUX_A = 0; MUX_B = 1; MUX_C = 0
#define SEL_RX3() INH1 = 1 ; INH0 = 0 ; MUX_A = 1; MUX_B = 1; MUX_C = 0
#define SEL_RX4() INH1 = 1 ; INH0 = 0 ; MUX_A = 0; MUX_B = 0; MUX_C = 1
#define SEL_RX5() INH1 = 1 ; INH0 = 0 ; MUX_A = 1; MUX_B = 0; MUX_C = 1
#define SEL_RX6() INH1 = 1 ; INH0 = 0 ; MUX_A = 0; MUX_B = 1; MUX_C = 1
#define SEL_RX7() INH1 = 1 ; INH0 = 0 ; MUX_A = 1; MUX_B = 1; MUX_C = 1
#define SEL_RX8() INH0 = 1 ; INH1 = 0 ; MUX_A = 0; MUX_B = 0; MUX_C = 0
#define SEL_RX9() INH0 = 1 ; INH1 = 0 ; MUX_A = 1; MUX_B = 0; MUX_C = 0

/*#define PWRUP_NODE_0_3 0x000f //0b 0000 0000 0000 1111
#define PWRUP_NODE_1_4 0x001e //0b 0000 0000 0001 1110
#define PWRUP_NODE_2_5 0x003c //0b 0000 0000 0011 1100
#define PWRUP_NODE_3_6 0x0078 //0b 0000 0000 0111 1000
#define PWRUP_NODE_4_7 0x00f0 //0b 0000 0000 1111 0000
#define PWRUP_NODE_5_8 0x01e0 //0b 0000 0001 1110 0000
#define PWRUP_NODE_6_9 0x03c0 //0b 0000 0011 1100 0000
#define PWRUP_NODE_7_0 0x0381 //0b 0000 0011 1000 0001
#define PWRUP_NODE_8_1 0x0303 //0b 0000 0011 0000 0011
#define PWRUP_NODE_9_2 0x0207 //0b 0000 0010 0000 0111
#define PWROFF_ALL_NODE 0x0000 //0b 0000 0000 0000 0000*/

//全部取反
#define PWRUP_NODE_0_3 0xfff0 //0b 0000 0000 0000 1111
#define PWRUP_NODE_1_4 0xffe1 //0b 0000 0000 0001 1110
#define PWRUP_NODE_2_5 0xffc3 //0b 0000 0000 0011 1100
#define PWRUP_NODE_3_6 0xff87 //0b 0000 0000 0111 1000
#define PWRUP_NODE_4_7 0xff0f //0b 0000 0000 1111 0000
#define PWRUP_NODE_5_8 0xfe1f //0b 0000 0001 1110 0000
#define PWRUP_NODE_6_9 0xfc3f //0b 0000 0011 1100 0000
#define PWRUP_NODE_7_0 0xfc7e //0b 0000 0011 1000 0001
#define PWRUP_NODE_8_1 0xfcfc //0b 0000 0011 0000 0011
#define PWRUP_NODE_9_2 0xfdf8 //0b 0000 0010 0000 0111
#define PWROFF_ALL_NODE 0xffff //0b 0000 0000 0000 0000



//切换到每个node 对应的rx时，
//其实这个node 之前已经大概工作了150ms。
//每个node 接收50ms，所以每个node 一共上电200ms就断电了。
//注意: 要从0~9逐个切换
#define SwiftToRx0() SEL_RX0();SN74HC595_OUT(PWRUP_NODE_0_3)
#define SwiftToRx1() SEL_RX1();SN74HC595_OUT(PWRUP_NODE_1_4)
#define SwiftToRx2() SEL_RX2();SN74HC595_OUT(PWRUP_NODE_2_5)
#define SwiftToRx3() SEL_RX3();SN74HC595_OUT(PWRUP_NODE_3_6)
#define SwiftToRx4() SEL_RX4();SN74HC595_OUT(PWRUP_NODE_4_7)
#define SwiftToRx5() SEL_RX5();SN74HC595_OUT(PWRUP_NODE_5_8)
#define SwiftToRx6() SEL_RX6();SN74HC595_OUT(PWRUP_NODE_6_9)
#define SwiftToRx7() SEL_RX7();SN74HC595_OUT(PWRUP_NODE_7_0)
#define SwiftToRx8() SEL_RX8();SN74HC595_OUT(PWRUP_NODE_8_1)
#define SwiftToRx9() SEL_RX9();SN74HC595_OUT(PWRUP_NODE_9_2)



//74hc595 IO 定义
#define SER P0_5
#define RCLK P0_4
#define SRCLK P0_6


//IIC IO操作函数	 
#define SDA_IN()  {P1DIR &= 0xf7;P2INP = 0;P1INP &= 0xf7;}
#define SDA_OUT() {P1DIR |= 0x08;}

#define IIC_SCL    P1_2   //SCL
#define WRITE_SDA    P1_3 //写入SDA	 
#define READ_SDA   P1_3    //读取SDA 



#define NODE_0	0
#define NODE_1	1
#define NODE_2	2
#define NODE_3	3
#define NODE_4	4
#define NODE_5	5
#define NODE_6	6
#define NODE_7	7
#define NODE_8	8
#define NODE_9	9





//-----------------------------宏函数定义


//P0_2,3用作uart 外设
//P0_0,2 输入,1,3,4,5,6,7输出
//P1_0,1,2,3,5,6,7输出
//P2_0输出
#define IO_Init(  )\
    do\
    {\
        P0SEL = 0x0c;\
        P1SEL = 0;\
        P2SEL = 0;\
        P0DIR = 0xfa;\
        P1DIR = 0xff;\
        P2DIR = 0x1F;\
        P0 = 0x0;\
        P1 = 0x0;\
        P2 = 0x0;\
    }   while( 1==0 )
    




#endif

