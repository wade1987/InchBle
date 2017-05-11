/*********************************************************************************************************
*                                 Inch.c  主函数
*
* 文件名	: Inch.c
* 版本号	: V1.00
* 作者	: 张强
*             
*********************************************************************************************************/

#include "inch.h"
#include "npi.h"
//#include "oled.h"
//#include "myiic.h"
//#include "LDC1314.h"


#include "bcomdef.h"
#include "OSAL.h"
#include "OSAL_PwrMgr.h"

#include "OnBoard.h"


#include "gatt.h"

#include "hci.h"

#include "gapgattserver.h"
#include "gattservapp.h"

#include "simpleGATTprofile.h"




/* ------------------------------------------------------------------------------------------------
 *                                      本文件所用全局变量
 * ------------------------------------------------------------------------------------------------
 */

//上电计时器
static uint32 SytmPwrOnTim = 0; 
static uint8 tmr1;


static uint8 NodeIdx = NODE_0;
static uint8 ParseStep = 0;
static uint16 data_h = 0;
static uint16 data_l = 0;

static uint8 KeyDetectStep=0;
static uint8 KeyDetectTmr=0;

static uint8 StartDetectLengthFlag=0;



uint16 NodeLen[NODE_NUM];




/*
*********************************************************************************************************
*                                           ParseRxData()
*
* 描述: 解析node 发来的数据
*
* 参数: 无
*
* 附注: 返回当前Node 的序号
*********************************************************************************************************
*/

uint8 ParseRxData( uint8 *buf, uint16 len )
{
	uint8 i;
	static uint8 nodeidx;

	//找到0xaa 0x55 开头的数据
	for(i = 0 ; i < len ; i++)
	{
		switch(ParseStep)
		{
			case 0://find 0xaa
				if(buf[i] == 0xaa)
				{
					nodeidx = NodeIdx;
					ParseStep++;
				}
				break;
			case 1://find 0x55
				if(buf[i] == 0x55)
					ParseStep++;
				else
					ParseStep = 0; //fail
				break;
			case 2://find DATA_H (buf[i] >= 5)是指长度不会大于1024mm
				if((buf[i] == 0xaa) || (buf[i] == 0x55) || (buf[i] >= 5))
				{
					ParseStep = 0; //fail
				}
				else
				{
					data_h = buf[i];
					ParseStep++;
				}
				break;
			case 3://find DATA_L and calc length value
				data_l = buf[i];
				if(nodeidx ==  NodeIdx)
				{
					NodeLen[nodeidx] = (data_h << 8) | data_l;
				}
				else
				{
					ParseStep = 0;
					return 0xff;
				}
				ParseStep = 0;
				return nodeidx;
			default:
				ParseStep = 0;
				break;
		}
	}
	return 0xff;
}

/*
*********************************************************************************************************
*                                           LedRunningProcess()
*
* 描述: LED指示衣服是否在检测长度
*
* 参数: 无
*
* 附注: 闪烁就是开始检测长途
*********************************************************************************************************
*/

void LedRunningProcess(void)
{
	static uint8 flagg = 0;

	if(StartDetectLengthFlag)
	{
		if(flagg)
		{
			LED_ALL_ON();
			flagg = 0;
		}
		else
		{
			LED_ALL_OFF();
			flagg = 1;
		}
	}
	else
	{
		;	
	}
}

/*
*********************************************************************************************************
*                                           DebugOut()
*
* 描述: 输出每个node 的长度
*
* 参数: 无
*
* 附注: 无
*********************************************************************************************************
*/

void DebugOut(void)
{
	static uint8 i=0;
	
	switch(i)
	{
		case 0:NPI_PrintValue("node0:",NodeLen[0],10);NPI_PrintString("mm ");NPI_PrintString("\n");i++;break;
		case 1:NPI_PrintValue("node1:",NodeLen[1],10);NPI_PrintString("mm ");NPI_PrintString("\n");i++;break;
		case 2:NPI_PrintValue("node2:",NodeLen[2],10);NPI_PrintString("mm ");NPI_PrintString("\n");i++;break;
		case 3:NPI_PrintValue("node3:",NodeLen[3],10);NPI_PrintString("mm ");NPI_PrintString("\n");i++;break;
		case 4:NPI_PrintValue("node4:",NodeLen[4],10);NPI_PrintString("mm ");NPI_PrintString("\n");i++;break;
		case 5:NPI_PrintValue("node5:",NodeLen[5],10);NPI_PrintString("mm ");NPI_PrintString("\n");i++;break;
		case 6:NPI_PrintValue("node6:",NodeLen[6],10);NPI_PrintString("mm ");NPI_PrintString("\n");i++;break;
		case 7:NPI_PrintValue("node7:",NodeLen[7],10);NPI_PrintString("mm ");NPI_PrintString("\n");i++;break;
		case 8:NPI_PrintValue("node8:",NodeLen[8],10);NPI_PrintString("mm ");NPI_PrintString("\n");i++;break;
		case 9:NPI_PrintValue("node9:",NodeLen[9],10);NPI_PrintString("mm ");NPI_PrintString("\n");i=0;break;
		default:i = 0;break;
	}
}

/*
*********************************************************************************************************
*                                           DebugOutProcess()
*
* 描述: 调试口输出
*
* 参数: 无
*
* 附注: 每100ms 发送一个node 的数据给串口调试数据
*********************************************************************************************************
*/

void DebugOutProcess(void)
{
	if(StartDetectLengthFlag)
	{
		if(tmr1 >= 2) // 50*2=100ms
		{
			tmr1 = 0;
			DebugOut();
		}
	}

}

/*
*********************************************************************************************************
*                                           SN74HC595_OUT()
*
* 描述: 为两个SN74HC595 输出两个byte
*
* 参数: 无
*
* 附注: 无
*********************************************************************************************************
*/

void SN74HC595_OUT(uint16 a) 
{  
	uint8 i; 

	for(i=0;i<16;i++) 
	{ 
		asm("nop");
		SRCLK = 0;    
		asm("nop"); 
		SER = (a & 0x8000) > 0 ? 1 : 0; 
		a = a << 1; 
		asm("nop");
		SRCLK = 1;
		asm("nop");
	} 
	//锁存输出
	asm("nop"); 
	RCLK = 0;         
	asm("nop");
	RCLK = 1; 
	asm("nop");
} 


/*
*********************************************************************************************************
*                                           EnableNodeDec()
*
* 描述: 打开检测
*
* 参数: 无
*
* 附注: 无
*********************************************************************************************************
*/

void EnableNodeDec(void) 
{
	StartDetectLengthFlag = 1;
	SwiftToRx0();
	NodeIdx = NODE_0;
	SimpleProfile_SetParameter( SIMPLEPROFILE_START, sizeof(uint8), &StartDetectLengthFlag);
}

/*
*********************************************************************************************************
*                                          DisableNodeDec()
*
* 描述: 关闭检测
*
* 参数: 无
*
* 附注: 无
*********************************************************************************************************
*/

void DisableNodeDec(void) 
{
	StartDetectLengthFlag = 0;
	SN74HC595_OUT(PWROFF_ALL_NODE);
	SimpleProfile_SetParameter( SIMPLEPROFILE_START, sizeof(uint8), &StartDetectLengthFlag);
}

/*
*********************************************************************************************************
*                                           KeyDetectProcess()
*
* 描述: 按键检测
*
* 参数: 无
*
* 附注: KeyDetectTmr 50ms 加1
*********************************************************************************************************
*/

void KeyDetectProcess(void)
{
	switch(KeyDetectStep)
	{
		case 0:
			//如果有按键按下
			if(READ_KEY() == 0)
			{
				KeyDetectStep++;
				KeyDetectTmr = 0;
			}
			break;
		case 1:
			//防抖
			if(READ_KEY() == 1)
			{
				KeyDetectStep--;
			}
			//确认按键确实按下了
			else if(KeyDetectTmr >= 1)
			{
				//开始或停止检测长度
				if(StartDetectLengthFlag > 0)
					DisableNodeDec();
				else
					EnableNodeDec();

				KeyDetectStep++;
			}
			break;
		case 2:
			//如果按键弹起
			if(READ_KEY() == 1)
			{
				KeyDetectStep++;
				KeyDetectTmr = 0;
			}
			break;
		case 3:
			//防抖
			if(READ_KEY() == 0)
			{
				KeyDetectStep--;
			}
			//确认按键确实弹起了
			else if(KeyDetectTmr >= 1)
			{
				KeyDetectStep = 0;
			}
			break;
		default:
			KeyDetectStep = 0;
			break;
	}
}

/*
*********************************************************************************************************
*                                           NodeSwitchProcess()
*
* 描述: 切换和rx通信的node 以及node 的电源端口(74hc595)
*
* 参数: 无
*
* 附注: 该函数每50ms执行一次，所以也就50ms切换一次
*********************************************************************************************************
*/

void NodeSwitchProcess(void)
{
	uint8 numBytes = 0;
	uint8 buf[128];

	if(StartDetectLengthFlag)
	{
		//每次切换rx 通道时清空一下rx buffer 和接收工作状态
		ParseStep = 0;
		numBytes = NPI_RxBufLen();//读出串口缓冲区有多少字节 
	    if(numBytes)
    	{
	      NPI_ReadTransport(buf,numBytes);//从串口缓冲区读出numBytes字节数据
	    }
		ParseStep = 0;
		
		//切换NODE 的rx 和电源
		if(NodeIdx == NODE_0)
		{
			SwiftToRx1();
			NodeIdx = NODE_1;
		}
		else if(NodeIdx == NODE_1)
		{
			SwiftToRx2();
			NodeIdx = NODE_2;
		}
		else if(NodeIdx == NODE_2)
		{
			SwiftToRx3();
			NodeIdx = NODE_3;
		}
		else if(NodeIdx == NODE_3)
		{
			SwiftToRx4();
			NodeIdx = NODE_4;
		}
		else if(NodeIdx == NODE_4)
		{
			SwiftToRx5();
			NodeIdx = NODE_5;
		}
		else if(NodeIdx == NODE_5)
		{
			SwiftToRx6();
			NodeIdx = NODE_6;
		}
		else if(NodeIdx == NODE_6)
		{
			SwiftToRx7();
			NodeIdx = NODE_7;
		}
		else if(NodeIdx == NODE_7)
		{
			SwiftToRx8();
			NodeIdx = NODE_8;
		}
		else if(NodeIdx == NODE_8)
		{
			SwiftToRx9();
			NodeIdx = NODE_9;
		}
		else if(NodeIdx == NODE_9)
		{
			SwiftToRx0();
			NodeIdx = NODE_0;
		}
		else
		{
			SwiftToRx0();
			NodeIdx = NODE_0;
		}
	}

}

void UploadBLEData(uint8 node_idx)
{

	uint8 tmp[2];
	 switch(node_idx)
 	{
		case 0:
			tmp[0] = (uint8)(NodeLen[node_idx] >> 8);
			tmp[1] = (uint8)NodeLen[node_idx];
			SimpleProfile_SetParameter( SIMPLEPROFILE_NODE0, SIMPLEPROFILE_NODE_LEN, tmp);
			break;
		case 1:
			tmp[0] = (uint8)(NodeLen[node_idx] >> 8);
			tmp[1] = (uint8)NodeLen[node_idx];
			SimpleProfile_SetParameter( SIMPLEPROFILE_NODE1, SIMPLEPROFILE_NODE_LEN, tmp);
			break;
		case 2:
			tmp[0] = (uint8)(NodeLen[node_idx] >> 8);
			tmp[1] = (uint8)NodeLen[node_idx];
			SimpleProfile_SetParameter( SIMPLEPROFILE_NODE2, SIMPLEPROFILE_NODE_LEN, tmp);
			break;
		case 3:
			tmp[0] = (uint8)(NodeLen[node_idx] >> 8);
			tmp[1] = (uint8)NodeLen[node_idx];
			SimpleProfile_SetParameter( SIMPLEPROFILE_NODE3, SIMPLEPROFILE_NODE_LEN, tmp);
			break;
		case 4:
			tmp[0] = (uint8)(NodeLen[node_idx] >> 8);
			tmp[1] = (uint8)NodeLen[node_idx];
			SimpleProfile_SetParameter( SIMPLEPROFILE_NODE4, SIMPLEPROFILE_NODE_LEN, tmp);
			break;
		case 5:
			tmp[0] = (uint8)(NodeLen[node_idx] >> 8);
			tmp[1] = (uint8)NodeLen[node_idx];
			SimpleProfile_SetParameter( SIMPLEPROFILE_NODE5, SIMPLEPROFILE_NODE_LEN, tmp);
			break;
		case 6:
			tmp[0] = (uint8)(NodeLen[node_idx] >> 8);
			tmp[1] = (uint8)NodeLen[node_idx];
			SimpleProfile_SetParameter( SIMPLEPROFILE_NODE6, SIMPLEPROFILE_NODE_LEN, tmp);
			break;
		case 7:
			tmp[0] = (uint8)(NodeLen[node_idx] >> 8);
			tmp[1] = (uint8)NodeLen[node_idx];
			SimpleProfile_SetParameter( SIMPLEPROFILE_NODE7, SIMPLEPROFILE_NODE_LEN, tmp);
			break;
		case 8:
			tmp[0] = (uint8)(NodeLen[node_idx] >> 8);
			tmp[1] = (uint8)NodeLen[node_idx];
			SimpleProfile_SetParameter( SIMPLEPROFILE_NODE8, SIMPLEPROFILE_NODE_LEN, tmp);
			break;
		case 9:
			tmp[0] = (uint8)(NodeLen[node_idx] >> 8);
			tmp[1] = (uint8)NodeLen[node_idx];
			SimpleProfile_SetParameter( SIMPLEPROFILE_NODE9, SIMPLEPROFILE_NODE_LEN, tmp);
			break;
		default:
			;
			break;

 	}

}

/*
*********************************************************************************************************
*                                           InchMainTmrTick()
*
* 描述: 时基本函数
*
* 参数: 无
*
* 附注: 每INCH_MAIN_TICK_TIME(ms) //50ms  调用一次
*********************************************************************************************************
*/

void InchMainTmrTick(void) 
{
	KeyDetectTmr++;
	tmr1++;
	
}

/*
*********************************************************************************************************
*                                           InchMain()
*
* 描述: 测试尺寸
*
* 参数: 无
*
* 附注: 在performPeriodicTask 中调用period = INCH_MAIN_TICK_TIME(ms) //50ms
*********************************************************************************************************
*/

void InchMain(void)
{
	InchMainTmrTick();

	//刚上电后执行的程序
	if(SytmPwrOnTim == 0)
	{
		IO_Init();
		LED_ALL_OFF();
		SytmPwrOnTim++;
		DisableNodeDec();
	}
	else if(SytmPwrOnTim < 6)
	{
		SytmPwrOnTim++;
	}
	else if(SytmPwrOnTim == 6)
	{
		SytmPwrOnTim++;
	}
	else //主程序执行处
	{
		LedRunningProcess();
		DebugOutProcess();
		NodeSwitchProcess();
		KeyDetectProcess();

		
	}  
	
}




