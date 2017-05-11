/*********************************************************************************************************
*                                 Inch.c  ������
*
* �ļ���	: Inch.c
* �汾��	: V1.00
* ����	: ��ǿ
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
 *                                      ���ļ�����ȫ�ֱ���
 * ------------------------------------------------------------------------------------------------
 */

//�ϵ��ʱ��
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
* ����: ����node ����������
*
* ����: ��
*
* ��ע: ���ص�ǰNode �����
*********************************************************************************************************
*/

uint8 ParseRxData( uint8 *buf, uint16 len )
{
	uint8 i;
	static uint8 nodeidx;

	//�ҵ�0xaa 0x55 ��ͷ������
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
			case 2://find DATA_H (buf[i] >= 5)��ָ���Ȳ������1024mm
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
* ����: LEDָʾ�·��Ƿ��ڼ�ⳤ��
*
* ����: ��
*
* ��ע: ��˸���ǿ�ʼ��ⳤ;
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
* ����: ���ÿ��node �ĳ���
*
* ����: ��
*
* ��ע: ��
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
* ����: ���Կ����
*
* ����: ��
*
* ��ע: ÿ100ms ����һ��node �����ݸ����ڵ�������
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
* ����: Ϊ����SN74HC595 �������byte
*
* ����: ��
*
* ��ע: ��
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
	//�������
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
* ����: �򿪼��
*
* ����: ��
*
* ��ע: ��
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
* ����: �رռ��
*
* ����: ��
*
* ��ע: ��
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
* ����: �������
*
* ����: ��
*
* ��ע: KeyDetectTmr 50ms ��1
*********************************************************************************************************
*/

void KeyDetectProcess(void)
{
	switch(KeyDetectStep)
	{
		case 0:
			//����а�������
			if(READ_KEY() == 0)
			{
				KeyDetectStep++;
				KeyDetectTmr = 0;
			}
			break;
		case 1:
			//����
			if(READ_KEY() == 1)
			{
				KeyDetectStep--;
			}
			//ȷ�ϰ���ȷʵ������
			else if(KeyDetectTmr >= 1)
			{
				//��ʼ��ֹͣ��ⳤ��
				if(StartDetectLengthFlag > 0)
					DisableNodeDec();
				else
					EnableNodeDec();

				KeyDetectStep++;
			}
			break;
		case 2:
			//�����������
			if(READ_KEY() == 1)
			{
				KeyDetectStep++;
				KeyDetectTmr = 0;
			}
			break;
		case 3:
			//����
			if(READ_KEY() == 0)
			{
				KeyDetectStep--;
			}
			//ȷ�ϰ���ȷʵ������
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
* ����: �л���rxͨ�ŵ�node �Լ�node �ĵ�Դ�˿�(74hc595)
*
* ����: ��
*
* ��ע: �ú���ÿ50msִ��һ�Σ�����Ҳ��50ms�л�һ��
*********************************************************************************************************
*/

void NodeSwitchProcess(void)
{
	uint8 numBytes = 0;
	uint8 buf[128];

	if(StartDetectLengthFlag)
	{
		//ÿ���л�rx ͨ��ʱ���һ��rx buffer �ͽ��չ���״̬
		ParseStep = 0;
		numBytes = NPI_RxBufLen();//�������ڻ������ж����ֽ� 
	    if(numBytes)
    	{
	      NPI_ReadTransport(buf,numBytes);//�Ӵ��ڻ���������numBytes�ֽ�����
	    }
		ParseStep = 0;
		
		//�л�NODE ��rx �͵�Դ
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
* ����: ʱ��������
*
* ����: ��
*
* ��ע: ÿINCH_MAIN_TICK_TIME(ms) //50ms  ����һ��
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
* ����: ���Գߴ�
*
* ����: ��
*
* ��ע: ��performPeriodicTask �е���period = INCH_MAIN_TICK_TIME(ms) //50ms
*********************************************************************************************************
*/

void InchMain(void)
{
	InchMainTmrTick();

	//���ϵ��ִ�еĳ���
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
	else //������ִ�д�
	{
		LedRunningProcess();
		DebugOutProcess();
		NodeSwitchProcess();
		KeyDetectProcess();

		
	}  
	
}




