/**************************************************************************************************
  Filename:       simpleGATTprofile.c
  Revised:        $Date: 2013-05-06 13:33:47 -0700 (Mon, 06 May 2013) $
  Revision:       $Revision: 34153 $

  Description:    This file contains the Simple GATT profile sample GATT service 
                  profile for use with the BLE sample application.

  Copyright 2010 - 2013 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, 
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, 
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com. 
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "bcomdef.h"
#include "OSAL.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "gapbondmgr.h"

#include "simpleGATTprofile.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

#define SERVAPP_NUM_ATTR_SUPPORTED        38

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
// Simple GATT Profile Service UUID: 0xFFCC
CONST uint8 simpleProfileServUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(SIMPLEPROFILE_SERV_UUID), HI_UINT16(SIMPLEPROFILE_SERV_UUID)
};

// Characteristic NODE0 UUID: 0xFFE0
CONST uint8 simpleProfilenode0UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(SIMPLEPROFILE_NODE0_UUID), HI_UINT16(SIMPLEPROFILE_NODE0_UUID)
};

// Characteristic NODE1 UUID: 0xFFE1
CONST uint8 simpleProfilenode1UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(SIMPLEPROFILE_NODE1_UUID), HI_UINT16(SIMPLEPROFILE_NODE1_UUID)
};

// Characteristic NODE2 UUID: 0xFFE2
CONST uint8 simpleProfilenode2UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(SIMPLEPROFILE_NODE2_UUID), HI_UINT16(SIMPLEPROFILE_NODE2_UUID)
};

// Characteristic NODE3 UUID: 0xFFE3
CONST uint8 simpleProfilenode3UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(SIMPLEPROFILE_NODE3_UUID), HI_UINT16(SIMPLEPROFILE_NODE3_UUID)
};

// Characteristic NODE4 UUID: 0xFFE4
CONST uint8 simpleProfilenode4UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(SIMPLEPROFILE_NODE4_UUID), HI_UINT16(SIMPLEPROFILE_NODE4_UUID)
};

// Characteristic NODE5 UUID: 0xFFE5
CONST uint8 simpleProfilenode5UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(SIMPLEPROFILE_NODE5_UUID), HI_UINT16(SIMPLEPROFILE_NODE5_UUID)
};

// Characteristic NODE6 UUID: 0xFFE6
CONST uint8 simpleProfilenode6UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(SIMPLEPROFILE_NODE6_UUID), HI_UINT16(SIMPLEPROFILE_NODE6_UUID)
};

// Characteristic NODE7 UUID: 0xFFE7
CONST uint8 simpleProfilenode7UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(SIMPLEPROFILE_NODE7_UUID), HI_UINT16(SIMPLEPROFILE_NODE7_UUID)
};

// Characteristic NODE8 UUID: 0xFFE8
CONST uint8 simpleProfilenode8UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(SIMPLEPROFILE_NODE8_UUID), HI_UINT16(SIMPLEPROFILE_NODE8_UUID)
};

// Characteristic NODE9 UUID: 0xFFE9
CONST uint8 simpleProfilenode9UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(SIMPLEPROFILE_NODE9_UUID), HI_UINT16(SIMPLEPROFILE_NODE9_UUID)
};

// Characteristic START UUID: 0xFFEA
CONST uint8 simpleProfilestartUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(SIMPLEPROFILE_START_UUID), HI_UINT16(SIMPLEPROFILE_START_UUID)
};

// Characteristic BATTY UUID: 0xFFEB
CONST uint8 simpleProfilebattyUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(SIMPLEPROFILE_BATTY_UUID), HI_UINT16(SIMPLEPROFILE_BATTY_UUID)
};



/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

static simpleProfileCBs_t *simpleProfile_AppCBs = NULL;

/*********************************************************************
 * Profile Attributes - variables
 */

// Simple Profile Service attribute
static CONST gattAttrType_t simpleProfileService = { ATT_BT_UUID_SIZE, simpleProfileServUUID };

// Simple Profile Characteristic NODE0 Properties
static uint8 simpleProfileNode0Props = GATT_PROP_READ;

// Characteristic NODE0 Value
static uint8 simpleProfileNode0[SIMPLEPROFILE_NODE_LEN] = { 0, 0 };

// Simple Profile Characteristic NODE0 User Description
static uint8 simpleProfileNode0UserDesp[6] = "Node0\0";


// Simple Profile Characteristic NODE1 Properties
static uint8 simpleProfileNode1Props = GATT_PROP_READ;

// Characteristic NODE1 Value
static uint8 simpleProfileNode1[SIMPLEPROFILE_NODE_LEN] = { 0, 0 };

// Simple Profile Characteristic NODE1 User Description
static uint8 simpleProfileNode1UserDesp[6] = "Node1\0";


// Simple Profile Characteristic NODE2 Properties
static uint8 simpleProfileNode2Props = GATT_PROP_READ;

// Characteristic NODE2 Value
static uint8 simpleProfileNode2[SIMPLEPROFILE_NODE_LEN] = { 0, 0 };

// Simple Profile Characteristic NODE2 User Description
static uint8 simpleProfileNode2UserDesp[6] = "Node2\0";


// Simple Profile Characteristic NODE3 Properties
static uint8 simpleProfileNode3Props = GATT_PROP_READ;

// Characteristic NODE3 Value
static uint8 simpleProfileNode3[SIMPLEPROFILE_NODE_LEN] = { 0, 0 };

// Simple Profile Characteristic NODE3 User Description
static uint8 simpleProfileNode3UserDesp[6] = "Node3\0";


// Simple Profile Characteristic NODE4 Properties
static uint8 simpleProfileNode4Props = GATT_PROP_READ;

// Characteristic NODE4 Value
static uint8 simpleProfileNode4[SIMPLEPROFILE_NODE_LEN] = { 0, 0 };

// Simple Profile Characteristic NODE4 User Description
static uint8 simpleProfileNode4UserDesp[6] = "Node4\0";


// Simple Profile Characteristic NODE5 Properties
static uint8 simpleProfileNode5Props = GATT_PROP_READ;

// Characteristic NODE5 Value
static uint8 simpleProfileNode5[SIMPLEPROFILE_NODE_LEN] = { 0, 0 };

// Simple Profile Characteristic NODE5 User Description
static uint8 simpleProfileNode5UserDesp[6] = "Node5\0";


// Simple Profile Characteristic NODE6 Properties
static uint8 simpleProfileNode6Props = GATT_PROP_READ;

// Characteristic NODE6 Value
static uint8 simpleProfileNode6[SIMPLEPROFILE_NODE_LEN] = { 0, 0 };

// Simple Profile Characteristic NODE6 User Description
static uint8 simpleProfileNode6UserDesp[6] = "Node6\0";


// Simple Profile Characteristic NODE7 Properties
static uint8 simpleProfileNode7Props = GATT_PROP_READ;

// Characteristic NODE7 Value
static uint8 simpleProfileNode7[SIMPLEPROFILE_NODE_LEN] = { 0, 0 };

// Simple Profile Characteristic NODE7 User Description
static uint8 simpleProfileNode7UserDesp[6] = "Node7\0";


// Simple Profile Characteristic NODE8 Properties
static uint8 simpleProfileNode8Props = GATT_PROP_READ;

// Characteristic NODE8 Value
static uint8 simpleProfileNode8[SIMPLEPROFILE_NODE_LEN] = { 0, 0 };

// Simple Profile Characteristic NODE8 User Description
static uint8 simpleProfileNode8UserDesp[6] = "Node8\0";


// Simple Profile Characteristic NODE9 Properties
static uint8 simpleProfileNode9Props = GATT_PROP_READ;

// Characteristic NODE9 Value
static uint8 simpleProfileNode9[SIMPLEPROFILE_NODE_LEN] = { 0, 0 };

// Simple Profile Characteristic NODE9 User Description
static uint8 simpleProfileNode9UserDesp[6] = "Node9\0";


// Simple Profile Characteristic START Properties
static uint8 simpleProfileStartProps = GATT_PROP_NOTIFY;

// Characteristic START Value
static uint8 simpleProfileStart = 0;

// Simple Profile Characteristic START Configuration Each client has its own
// instantiation of the Client Characteristic Configuration. Reads of the
// Client Characteristic Configuration only shows the configuration for
// that client and writes only affect the configuration of that client.
static gattCharCfg_t simpleProfileStartConfig[GATT_MAX_NUM_CONN];
                                        
// Simple Profile Characteristic START User Description
static uint8 simpleProfileStartUserDesp[6] = "Start\0";


// Simple Profile Characteristic BATTY Properties
static uint8 simpleProfileBattyProps = GATT_PROP_READ;

// Characteristic BATTY Value
static uint8 simpleProfileBatty=0;

// Simple Profile Characteristic BATTY User Description
static uint8 simpleProfileBattyUserDesp[8] = "Battery\0";



/*
// Simple Profile Characteristic 1 Properties
static uint8 simpleProfileChar1Props = GATT_PROP_READ | GATT_PROP_WRITE;

// Characteristic 1 Value
static uint8 simpleProfileChar1 = 0;

// Simple Profile Characteristic 1 User Description
static uint8 simpleProfileChar1UserDesp[17] = "Characteristic 1\0";*/


/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t simpleProfileAttrTbl[SERVAPP_NUM_ATTR_SUPPORTED] = 
{
  // Simple Profile Service
  { 
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
    GATT_PERMIT_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8 *)&simpleProfileService            /* pValue */
  },

    // Characteristic Node0 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &simpleProfileNode0Props 
    },

      // Characteristic Value Node0
      { 
        { ATT_BT_UUID_SIZE, simpleProfilenode0UUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileNode0 
      },

      // Characteristic Node0 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileNode0UserDesp 
      }, 
      
    // Characteristic Node1 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &simpleProfileNode1Props 
    },

      // Characteristic Value Node1
      { 
        { ATT_BT_UUID_SIZE, simpleProfilenode1UUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileNode1
      },

      // Characteristic Node1 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileNode1UserDesp 
      },
      
    // Characteristic Node2 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &simpleProfileNode2Props 
    },

      // Characteristic Value Node2
      { 
        { ATT_BT_UUID_SIZE, simpleProfilenode2UUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileNode2
      },

      // Characteristic Node2 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileNode2UserDesp 
      },
      
    // Characteristic Node3 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &simpleProfileNode3Props 
    },

      // Characteristic Value Node3
      { 
        { ATT_BT_UUID_SIZE, simpleProfilenode3UUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileNode3
      },

      // Characteristic Node3 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileNode3UserDesp 
      },
      
    // Characteristic Node4 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &simpleProfileNode4Props 
    },

      // Characteristic Value Node4
      { 
        { ATT_BT_UUID_SIZE, simpleProfilenode4UUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileNode4
      },

      // Characteristic Node4 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileNode4UserDesp 
      },
      
    // Characteristic Node5 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &simpleProfileNode5Props 
    },

      // Characteristic Value Node5
      { 
        { ATT_BT_UUID_SIZE, simpleProfilenode5UUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileNode5
      },

      // Characteristic Node5 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileNode5UserDesp 
      },
      
    // Characteristic Node6 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &simpleProfileNode6Props 
    },

      // Characteristic Value Node6
      { 
        { ATT_BT_UUID_SIZE, simpleProfilenode6UUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileNode6
      },

      // Characteristic Node6 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileNode6UserDesp 
      },
      
    // Characteristic Node7 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &simpleProfileNode7Props 
    },

      // Characteristic Value Node7
      { 
        { ATT_BT_UUID_SIZE, simpleProfilenode7UUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileNode7
      },

      // Characteristic Node7 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileNode7UserDesp 
      },
      
	// Characteristic Node8 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &simpleProfileNode8Props 
    },

      // Characteristic Value Node8
      { 
        { ATT_BT_UUID_SIZE, simpleProfilenode8UUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileNode8
      },

      // Characteristic Node8 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileNode8UserDesp 
      },
      
    // Characteristic Node9 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &simpleProfileNode9Props 
    },

      // Characteristic Value Node9
      { 
        { ATT_BT_UUID_SIZE, simpleProfilenode9UUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileNode9
      },

      // Characteristic Node9 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileNode9UserDesp 
      },
      
    // Characteristic Start Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &simpleProfileStartProps 
    },

      // Characteristic Value Start
      { 
        { ATT_BT_UUID_SIZE, simpleProfilestartUUID },
        0, 
        0, 
        &simpleProfileStart
      },

      // Characteristic Start configuration
      { 
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE, 
        0, 
        (uint8 *)simpleProfileStartConfig 
      },
      
      // Characteristic Start User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileStartUserDesp 
      },
      
    // Characteristic Batty Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &simpleProfileBattyProps 
    },

      // Characteristic Value Batty
      { 
        { ATT_BT_UUID_SIZE, simpleProfilebattyUUID },
        GATT_PERMIT_READ, 
        0, 
        &simpleProfileBatty
      },

      // Characteristic Batty User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileBattyUserDesp 
      }, 
      
    /*// Characteristic 5 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &simpleProfileChar5Props 
    },

      // Characteristic Value 5
      { 
        { ATT_BT_UUID_SIZE, simpleProfilechar5UUID },
        GATT_PERMIT_AUTHEN_READ, 
        0, 
        simpleProfileChar5 
      },

      // Characteristic 5 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileChar5UserDesp 
      },*/


};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8 simpleProfile_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
                            uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen );
static bStatus_t simpleProfile_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                 uint8 *pValue, uint8 len, uint16 offset );

static void simpleProfile_HandleConnStatusCB( uint16 connHandle, uint8 changeType );


/*********************************************************************
 * PROFILE CALLBACKS
 */
// Simple Profile Service Callbacks
CONST gattServiceCBs_t simpleProfileCBs =
{
  simpleProfile_ReadAttrCB,  // Read callback function pointer
  simpleProfile_WriteAttrCB, // Write callback function pointer
  NULL                       // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      SimpleProfile_AddService
 *
 * @brief   Initializes the Simple Profile service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   services - services to add. This is a bit map and can
 *                     contain more than one service.
 *
 * @return  Success or Failure
 */
bStatus_t SimpleProfile_AddService( uint32 services )
{
  uint8 status = SUCCESS;

  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, simpleProfileStartConfig );

  // Register with Link DB to receive link status change callback
  VOID linkDB_Register( simpleProfile_HandleConnStatusCB );  
  
  if ( services & SIMPLEPROFILE_SERVICE )
  {
    // Register GATT attribute list and CBs with GATT Server App
    status = GATTServApp_RegisterService( simpleProfileAttrTbl, 
                                          GATT_NUM_ATTRS( simpleProfileAttrTbl ),
                                          &simpleProfileCBs );
  }

  return ( status );
}


/*********************************************************************
 * @fn      SimpleProfile_RegisterAppCBs
 *
 * @brief   Registers the application callback function. Only call 
 *          this function once.
 *
 * @param   callbacks - pointer to application callbacks.
 *
 * @return  SUCCESS or bleAlreadyInRequestedMode
 */
bStatus_t SimpleProfile_RegisterAppCBs( simpleProfileCBs_t *appCallbacks )
{
  if ( appCallbacks )
  {
    simpleProfile_AppCBs = appCallbacks;
    
    return ( SUCCESS );
  }
  else
  {
    return ( bleAlreadyInRequestedMode );
  }
}
  

/*********************************************************************
 * @fn      SimpleProfile_SetParameter
 *
 * @brief   Set a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len - length of data to right
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t SimpleProfile_SetParameter( uint8 param, uint8 len, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {

	case SIMPLEPROFILE_NODE0:
      if ( len == SIMPLEPROFILE_NODE_LEN ) 
      {
        VOID osal_memcpy( simpleProfileNode0, value, SIMPLEPROFILE_NODE_LEN );
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;
	
	case SIMPLEPROFILE_NODE1:
		  if ( len == SIMPLEPROFILE_NODE_LEN ) 
		  {
			VOID osal_memcpy( simpleProfileNode1, value, SIMPLEPROFILE_NODE_LEN );
		  }
		  else
		  {
			ret = bleInvalidRange;
		  }
		  break;
	
	case SIMPLEPROFILE_NODE2:
		  if ( len == SIMPLEPROFILE_NODE_LEN ) 
		  {
			VOID osal_memcpy( simpleProfileNode2, value, SIMPLEPROFILE_NODE_LEN );
		  }
		  else
		  {
			ret = bleInvalidRange;
		  }
		  break;
	
	case SIMPLEPROFILE_NODE3:
		  if ( len == SIMPLEPROFILE_NODE_LEN ) 
		  {
			VOID osal_memcpy( simpleProfileNode3, value, SIMPLEPROFILE_NODE_LEN );
		  }
		  else
		  {
			ret = bleInvalidRange;
		  }
		  break;
	
	case SIMPLEPROFILE_NODE4:
		  if ( len == SIMPLEPROFILE_NODE_LEN ) 
		  {
			VOID osal_memcpy( simpleProfileNode4, value, SIMPLEPROFILE_NODE_LEN );
		  }
		  else
		  {
			ret = bleInvalidRange;
		  }
		  break;
	
	case SIMPLEPROFILE_NODE5:
		  if ( len == SIMPLEPROFILE_NODE_LEN ) 
		  {
			VOID osal_memcpy( simpleProfileNode5, value, SIMPLEPROFILE_NODE_LEN );
		  }
		  else
		  {
			ret = bleInvalidRange;
		  }
		  break;
	
	case SIMPLEPROFILE_NODE6:
		  if ( len == SIMPLEPROFILE_NODE_LEN ) 
		  {
			VOID osal_memcpy( simpleProfileNode6, value, SIMPLEPROFILE_NODE_LEN );
		  }
		  else
		  {
			ret = bleInvalidRange;
		  }
		  break;
	
	case SIMPLEPROFILE_NODE7:
		  if ( len == SIMPLEPROFILE_NODE_LEN ) 
		  {
			VOID osal_memcpy( simpleProfileNode7, value, SIMPLEPROFILE_NODE_LEN );
		  }
		  else
		  {
			ret = bleInvalidRange;
		  }
		  break;
	
	case SIMPLEPROFILE_NODE8:
		  if ( len == SIMPLEPROFILE_NODE_LEN ) 
		  {
			VOID osal_memcpy( simpleProfileNode8, value, SIMPLEPROFILE_NODE_LEN );
		  }
		  else
		  {
			ret = bleInvalidRange;
		  }
		  break;
	
	case SIMPLEPROFILE_NODE9:
		  if ( len == SIMPLEPROFILE_NODE_LEN ) 
		  {
			VOID osal_memcpy( simpleProfileNode9, value, SIMPLEPROFILE_NODE_LEN );
		  }
		  else
		  {
			ret = bleInvalidRange;
		  }
		  break;

	case SIMPLEPROFILE_START:
      if ( len == sizeof ( uint8 ) ) 
      {
        simpleProfileStart = *((uint8*)value);
        
        // See if Notification has been enabled
        GATTServApp_ProcessCharCfg( simpleProfileStartConfig, &simpleProfileStart, FALSE,
                                    simpleProfileAttrTbl, GATT_NUM_ATTRS( simpleProfileAttrTbl ),
                                    INVALID_TASK_ID );
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;
  
    case SIMPLEPROFILE_BATTY:
      if ( len == sizeof ( uint8 ) ) 
      {
        simpleProfileBatty = *((uint8*)value);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;
      
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return ( ret );
}

/*********************************************************************
 * @fn      SimpleProfile_GetParameter
 *
 * @brief   Get a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   value - pointer to data to put.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t SimpleProfile_GetParameter( uint8 param, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    /*case SIMPLEPROFILE_CHAR1:
      *((uint8*)value) = simpleProfileChar1;
      break;

    case SIMPLEPROFILE_CHAR2:
      *((uint8*)value) = simpleProfileChar2;
      break;      

    case SIMPLEPROFILE_CHAR3:
      *((uint8*)value) = simpleProfileChar3;
      break;  

    case SIMPLEPROFILE_CHAR4:
      *((uint8*)value) = simpleProfileChar4;
      break;

    case SIMPLEPROFILE_CHAR5:
      VOID osal_memcpy( value, simpleProfileChar5, SIMPLEPROFILE_CHAR5_LEN );
      break;      */
      
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return ( ret );
}

/*********************************************************************
 * @fn          simpleProfile_ReadAttrCB
 *
 * @brief       Read an attribute.
 *
 * @param       connHandle - connection message was received on
 * @param       pAttr - pointer to attribute
 * @param       pValue - pointer to data to be read
 * @param       pLen - length of data to be read
 * @param       offset - offset of the first octet to be read
 * @param       maxLen - maximum length of data to be read
 *
 * @return      Success or Failure
 */
static uint8 simpleProfile_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
                            uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen )
{
  bStatus_t status = SUCCESS;

  // If attribute permissions require authorization to read, return error
  if ( gattPermitAuthorRead( pAttr->permissions ) )
  {
    // Insufficient authorization
    return ( ATT_ERR_INSUFFICIENT_AUTHOR );
  }
  
  // Make sure it's not a blob operation (no attributes in the profile are long)
  if ( offset > 0 )
  {
    return ( ATT_ERR_ATTR_NOT_LONG );
  }
 
  if ( pAttr->type.len == ATT_BT_UUID_SIZE )
  {
    // 16-bit UUID
    uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
    switch ( uuid )
    {
      // No need for "GATT_SERVICE_UUID" or "GATT_CLIENT_CHAR_CFG_UUID" cases;
      // gattserverapp handles those reads

      // characteristics 1 and 2 have read permissions
      // characteritisc 3 does not have read permissions; therefore it is not
      //   included here
      // characteristic 4 does not have read permissions, but because it
      //   can be sent as a notification, it is included here


	  case SIMPLEPROFILE_NODE0_UUID:
	  case SIMPLEPROFILE_NODE1_UUID:
	  case SIMPLEPROFILE_NODE2_UUID:
	  case SIMPLEPROFILE_NODE3_UUID:
	  case SIMPLEPROFILE_NODE4_UUID:
	  case SIMPLEPROFILE_NODE5_UUID:
	  case SIMPLEPROFILE_NODE6_UUID:
	  case SIMPLEPROFILE_NODE7_UUID:
	  case SIMPLEPROFILE_NODE8_UUID:
	  case SIMPLEPROFILE_NODE9_UUID:
        *pLen = SIMPLEPROFILE_NODE_LEN;
        VOID osal_memcpy( pValue, pAttr->pValue, SIMPLEPROFILE_NODE_LEN );
        break;
	  case SIMPLEPROFILE_START_UUID:
	  case SIMPLEPROFILE_BATTY_UUID:
        *pLen = 1;
        pValue[0] = *pAttr->pValue;
        break;
	  
	  
     /* case SIMPLEPROFILE_CHAR1_UUID:
      case SIMPLEPROFILE_CHAR2_UUID:
      case SIMPLEPROFILE_CHAR4_UUID:
        *pLen = 1;
        pValue[0] = *pAttr->pValue;
        break;

      case SIMPLEPROFILE_CHAR5_UUID:
        *pLen = SIMPLEPROFILE_CHAR5_LEN;
        VOID osal_memcpy( pValue, pAttr->pValue, SIMPLEPROFILE_CHAR5_LEN );
        break;*/
        
      default:
        // Should never get here! (characteristics 3 and 4 do not have read permissions)
        *pLen = 0;
        status = ATT_ERR_ATTR_NOT_FOUND;
        break;
    }
  }
  else
  {
    // 128-bit UUID
    *pLen = 0;
    status = ATT_ERR_INVALID_HANDLE;
  }

  return ( status );
}

/*********************************************************************
 * @fn      simpleProfile_WriteAttrCB
 *
 * @brief   Validate attribute data prior to a write operation
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   len - length of data
 * @param   offset - offset of the first octet to be written
 *
 * @return  Success or Failure
 */
static bStatus_t simpleProfile_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                 uint8 *pValue, uint8 len, uint16 offset )
{
  bStatus_t status = SUCCESS;
  uint8 notifyApp = 0xFF;
  
  // If attribute permissions require authorization to write, return error
  if ( gattPermitAuthorWrite( pAttr->permissions ) )
  {
    // Insufficient authorization
    return ( ATT_ERR_INSUFFICIENT_AUTHOR );
  }
  
  if ( pAttr->type.len == ATT_BT_UUID_SIZE )
  {
    // 16-bit UUID
    uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
    switch ( uuid )
    {
      /*case SIMPLEPROFILE_CHAR1_UUID:
      case SIMPLEPROFILE_CHAR3_UUID:

        //Validate the value
        // Make sure it's not a blob oper
        if ( offset == 0 )
        {
          if ( len != 1 )
          {
            status = ATT_ERR_INVALID_VALUE_SIZE;
          }
        }
        else
        {
          status = ATT_ERR_ATTR_NOT_LONG;
        }
        
        //Write the value
        if ( status == SUCCESS )
        {
          uint8 *pCurValue = (uint8 *)pAttr->pValue;        
          *pCurValue = pValue[0];

          if( pAttr->pValue == &simpleProfileChar1 )
          {
            notifyApp = SIMPLEPROFILE_CHAR1;        
          }
          else
          {
            notifyApp = SIMPLEPROFILE_CHAR3;           
          }
        }
             
        break;*/

      case GATT_CLIENT_CHAR_CFG_UUID:
        status = GATTServApp_ProcessCCCWriteReq( connHandle, pAttr, pValue, len,
                                                 offset, GATT_CLIENT_CFG_NOTIFY );
        break;
        
      default:
        // Should never get here! (characteristics 2 and 4 do not have write permissions)
        status = ATT_ERR_ATTR_NOT_FOUND;
        break;
    }
  }
  else
  {
    // 128-bit UUID
    status = ATT_ERR_INVALID_HANDLE;
  }

  // If a charactersitic value changed then callback function to notify application of change
  if ( (notifyApp != 0xFF ) && simpleProfile_AppCBs && simpleProfile_AppCBs->pfnSimpleProfileChange )
  {
    simpleProfile_AppCBs->pfnSimpleProfileChange( notifyApp );  
  }
  
  return ( status );
}

/*********************************************************************
 * @fn          simpleProfile_HandleConnStatusCB
 *
 * @brief       Simple Profile link status change handler function.
 *
 * @param       connHandle - connection handle
 * @param       changeType - type of change
 *
 * @return      none
 */
static void simpleProfile_HandleConnStatusCB( uint16 connHandle, uint8 changeType )
{ 
  // Make sure this is not loopback connection
  if ( connHandle != LOOPBACK_CONNHANDLE )
  {
    // Reset Client Char Config if connection has dropped
    if ( ( changeType == LINKDB_STATUS_UPDATE_REMOVED )      ||
         ( ( changeType == LINKDB_STATUS_UPDATE_STATEFLAGS ) && 
           ( !linkDB_Up( connHandle ) ) ) )
    { 
      GATTServApp_InitCharCfg( connHandle, simpleProfileStartConfig );
    }
  }
}


/*********************************************************************
*********************************************************************/
