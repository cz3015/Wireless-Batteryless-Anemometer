/**********************************************************************************************
 * Filename:       adc_service.c
 *
 * Description:    This file contains the implementation of the service.
 *
 * Copyright (c) 2015-2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *************************************************************************************************/


/*********************************************************************
 * INCLUDES
 */
#include <string.h>

#include "bcomdef.h"
#include "OSAL.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "gapbondmgr.h"

#include "adc_service.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
* GLOBAL VARIABLES
*/

// adc_service Service UUID
CONST uint8_t adc_serviceUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(ADC_SERVICE_SERV_UUID), HI_UINT16(ADC_SERVICE_SERV_UUID)
};

// FREQ UUID
CONST uint8_t adc_service_FREQUUID[ATT_UUID_SIZE] =
{
  TI_BASE_UUID_128(ADC_SERVICE_FREQ_UUID)
};
// Gradient UUID
CONST uint8_t adc_service_GradientUUID[ATT_UUID_SIZE] =
{
  TI_BASE_UUID_128(ADC_SERVICE_GRADIENT_UUID)
};
// Temp UUID
CONST uint8_t adc_service_TempUUID[ATT_UUID_SIZE] =
{
  TI_BASE_UUID_128(ADC_SERVICE_TEMP_UUID)
};
// VDD UUID
CONST uint8_t adc_service_VDDUUID[ATT_UUID_SIZE] =
{
  TI_BASE_UUID_128(ADC_SERVICE_VDD_UUID)
};

/*********************************************************************
 * LOCAL VARIABLES
 */

static adc_serviceCBs_t *pAppCBs = NULL;

/*********************************************************************
* Profile Attributes - variables
*/

// Service declaration
static CONST gattAttrType_t adc_serviceDecl = { ATT_BT_UUID_SIZE, adc_serviceUUID };

// Characteristic "FREQ" Properties (for declaration)
static uint8_t adc_service_FREQProps = GATT_PROP_READ | GATT_PROP_WRITE | GATT_PROP_NOTIFY;

// Characteristic "FREQ" Value variable
static uint8_t adc_service_FREQVal[ADC_SERVICE_FREQ_LEN] = {0};

// Characteristic "FREQ" CCCD
static gattCharCfg_t *adc_service_FREQConfig;
// Characteristic "Gradient" Properties (for declaration)
static uint8_t adc_service_GradientProps = GATT_PROP_READ | GATT_PROP_WRITE | GATT_PROP_NOTIFY;

// Characteristic "Gradient" Value variable
static uint8_t adc_service_GradientVal[ADC_SERVICE_GRADIENT_LEN] = {0};

// Characteristic "Gradient" CCCD
static gattCharCfg_t *adc_service_GradientConfig;
// Characteristic "Temp" Properties (for declaration)
static uint8_t adc_service_TempProps = GATT_PROP_READ | GATT_PROP_WRITE | GATT_PROP_NOTIFY;

// Characteristic "Temp" Value variable
static uint8_t adc_service_TempVal[ADC_SERVICE_TEMP_LEN] = {0};

// Characteristic "Temp" CCCD
static gattCharCfg_t *adc_service_TempConfig;
// Characteristic "VDD" Properties (for declaration)
static uint8_t adc_service_VDDProps = GATT_PROP_READ | GATT_PROP_WRITE | GATT_PROP_NOTIFY;

// Characteristic "VDD" Value variable
static uint8_t adc_service_VDDVal[ADC_SERVICE_VDD_LEN] = {0};

// Characteristic "VDD" CCCD
static gattCharCfg_t *adc_service_VDDConfig;

/*********************************************************************
* Profile Attributes - Table
*/

static gattAttribute_t adc_serviceAttrTbl[] =
{
  // adc_service Service Declaration
  {
    { ATT_BT_UUID_SIZE, primaryServiceUUID },
    GATT_PERMIT_READ,
    0,
    (uint8_t *)&adc_serviceDecl
  },
    // FREQ Characteristic Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &adc_service_FREQProps
    },
      // FREQ Characteristic Value
      {
        { ATT_UUID_SIZE, adc_service_FREQUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        adc_service_FREQVal
      },
      // FREQ CCCD
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8 *)&adc_service_FREQConfig
      },
    // Gradient Characteristic Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &adc_service_GradientProps
    },
      // Gradient Characteristic Value
      {
        { ATT_UUID_SIZE, adc_service_GradientUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        adc_service_GradientVal
      },
      // Gradient CCCD
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8 *)&adc_service_GradientConfig
      },
    // Temp Characteristic Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &adc_service_TempProps
    },
      // Temp Characteristic Value
      {
        { ATT_UUID_SIZE, adc_service_TempUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        adc_service_TempVal
      },
      // Temp CCCD
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8 *)&adc_service_TempConfig
      },
    // VDD Characteristic Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &adc_service_VDDProps
    },
      // VDD Characteristic Value
      {
        { ATT_UUID_SIZE, adc_service_VDDUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        adc_service_VDDVal
      },
      // VDD CCCD
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8 *)&adc_service_VDDConfig
      },
};

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static bStatus_t adc_service_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                           uint8 *pValue, uint16 *pLen, uint16 offset,
                                           uint16 maxLen, uint8 method );
static bStatus_t adc_service_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                            uint8 *pValue, uint16 len, uint16 offset,
                                            uint8 method );

/*********************************************************************
 * PROFILE CALLBACKS
 */
// Simple Profile Service Callbacks
CONST gattServiceCBs_t adc_serviceCBs =
{
  adc_service_ReadAttrCB,  // Read callback function pointer
  adc_service_WriteAttrCB, // Write callback function pointer
  NULL                       // Authorization callback function pointer
};

/*********************************************************************
* PUBLIC FUNCTIONS
*/

/*
 * Adc_service_AddService- Initializes the Adc_service service by registering
 *          GATT attributes with the GATT server.
 *
 */
bStatus_t Adc_service_AddService( void )
{
  uint8_t status;

  // Allocate Client Characteristic Configuration table
  adc_service_FREQConfig = (gattCharCfg_t *)ICall_malloc( sizeof(gattCharCfg_t) * linkDBNumConns );
  if ( adc_service_FREQConfig == NULL )
  {
    return ( bleMemAllocError );
  }

  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, adc_service_FREQConfig );
  // Allocate Client Characteristic Configuration table
  adc_service_GradientConfig = (gattCharCfg_t *)ICall_malloc( sizeof(gattCharCfg_t) * linkDBNumConns );
  if ( adc_service_GradientConfig == NULL )
  {
    return ( bleMemAllocError );
  }

  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, adc_service_GradientConfig );
  // Allocate Client Characteristic Configuration table
  adc_service_TempConfig = (gattCharCfg_t *)ICall_malloc( sizeof(gattCharCfg_t) * linkDBNumConns );
  if ( adc_service_TempConfig == NULL )
  {
    return ( bleMemAllocError );
  }

  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, adc_service_TempConfig );
  // Allocate Client Characteristic Configuration table
  adc_service_VDDConfig = (gattCharCfg_t *)ICall_malloc( sizeof(gattCharCfg_t) * linkDBNumConns );
  if ( adc_service_VDDConfig == NULL )
  {
    return ( bleMemAllocError );
  }

  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, adc_service_VDDConfig );
  // Register GATT attribute list and CBs with GATT Server App
  status = GATTServApp_RegisterService( adc_serviceAttrTbl,
                                        GATT_NUM_ATTRS( adc_serviceAttrTbl ),
                                        GATT_MAX_ENCRYPT_KEY_SIZE,
                                        &adc_serviceCBs );

  return ( status );
}

/*
 * Adc_service_RegisterAppCBs - Registers the application callback function.
 *                    Only call this function once.
 *
 *    appCallbacks - pointer to application callbacks.
 */
bStatus_t Adc_service_RegisterAppCBs( adc_serviceCBs_t *appCallbacks )
{
  if ( appCallbacks )
  {
    pAppCBs = appCallbacks;

    return ( SUCCESS );
  }
  else
  {
    return ( bleAlreadyInRequestedMode );
  }
}

/*
 * Adc_service_SetParameter - Set a Adc_service parameter.
 *
 *    param - Profile parameter ID
 *    len - length of data to right
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 */
bStatus_t Adc_service_SetParameter( uint8 param, uint8 len, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case ADC_SERVICE_FREQ:
      if ( len == ADC_SERVICE_FREQ_LEN )
      {
        memcpy(adc_service_FREQVal, value, len);

        // Try to send notification.
        GATTServApp_ProcessCharCfg( adc_service_FREQConfig, (uint8_t *)&adc_service_FREQVal, 0,
                                    adc_serviceAttrTbl, GATT_NUM_ATTRS( adc_serviceAttrTbl ),
                                    INVALID_TASK_ID,  adc_service_ReadAttrCB);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case ADC_SERVICE_GRADIENT:
      if ( len == ADC_SERVICE_GRADIENT_LEN )
      {
        memcpy(adc_service_GradientVal, value, len);

        // Try to send notification.
        GATTServApp_ProcessCharCfg( adc_service_GradientConfig, (uint8_t *)&adc_service_GradientVal, 0,
                                    adc_serviceAttrTbl, GATT_NUM_ATTRS( adc_serviceAttrTbl ),
                                    INVALID_TASK_ID,  adc_service_ReadAttrCB);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case ADC_SERVICE_TEMP:
      if ( len == ADC_SERVICE_TEMP_LEN )
      {
        memcpy(adc_service_TempVal, value, len);

        // Try to send notification.
        GATTServApp_ProcessCharCfg( adc_service_TempConfig, (uint8_t *)&adc_service_TempVal, 0,
                                    adc_serviceAttrTbl, GATT_NUM_ATTRS( adc_serviceAttrTbl ),
                                    INVALID_TASK_ID,  adc_service_ReadAttrCB);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case ADC_SERVICE_VDD:
      if ( len == ADC_SERVICE_VDD_LEN )
      {
        memcpy(adc_service_VDDVal, value, len);

        // Try to send notification.
        GATTServApp_ProcessCharCfg( adc_service_VDDConfig, (uint8_t *)&adc_service_VDDVal, 0,
                                    adc_serviceAttrTbl, GATT_NUM_ATTRS( adc_serviceAttrTbl ),
                                    INVALID_TASK_ID,  adc_service_ReadAttrCB);
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
  return ret;
}


/*
 * Adc_service_GetParameter - Get a Adc_service parameter.
 *
 *    param - Profile parameter ID
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 */
bStatus_t Adc_service_GetParameter( uint8 param, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case ADC_SERVICE_FREQ:
      memcpy(value, adc_service_FREQVal, ADC_SERVICE_FREQ_LEN);
      break;

    case ADC_SERVICE_GRADIENT:
      memcpy(value, adc_service_GradientVal, ADC_SERVICE_GRADIENT_LEN);
      break;

    case ADC_SERVICE_TEMP:
      memcpy(value, adc_service_TempVal, ADC_SERVICE_TEMP_LEN);
      break;

    case ADC_SERVICE_VDD:
      memcpy(value, adc_service_VDDVal, ADC_SERVICE_VDD_LEN);
      break;

    default:
      ret = INVALIDPARAMETER;
      break;
  }
  return ret;
}


/*********************************************************************
 * @fn          adc_service_ReadAttrCB
 *
 * @brief       Read an attribute.
 *
 * @param       connHandle - connection message was received on
 * @param       pAttr - pointer to attribute
 * @param       pValue - pointer to data to be read
 * @param       pLen - length of data to be read
 * @param       offset - offset of the first octet to be read
 * @param       maxLen - maximum length of data to be read
 * @param       method - type of read message
 *
 * @return      SUCCESS, blePending or Failure
 */
static bStatus_t adc_service_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                       uint8 *pValue, uint16 *pLen, uint16 offset,
                                       uint16 maxLen, uint8 method )
{
  bStatus_t status = SUCCESS;

  // See if request is regarding the FREQ Characteristic Value
if ( ! memcmp(pAttr->type.uuid, adc_service_FREQUUID, pAttr->type.len) )
  {
    if ( offset > ADC_SERVICE_FREQ_LEN )  // Prevent malicious ATT ReadBlob offsets.
    {
      status = ATT_ERR_INVALID_OFFSET;
    }
    else
    {
      *pLen = MIN(maxLen, ADC_SERVICE_FREQ_LEN - offset);  // Transmit as much as possible
      memcpy(pValue, pAttr->pValue + offset, *pLen);
    }
  }
  // See if request is regarding the Gradient Characteristic Value
else if ( ! memcmp(pAttr->type.uuid, adc_service_GradientUUID, pAttr->type.len) )
  {
    if ( offset > ADC_SERVICE_GRADIENT_LEN )  // Prevent malicious ATT ReadBlob offsets.
    {
      status = ATT_ERR_INVALID_OFFSET;
    }
    else
    {
      *pLen = MIN(maxLen, ADC_SERVICE_GRADIENT_LEN - offset);  // Transmit as much as possible
      memcpy(pValue, pAttr->pValue + offset, *pLen);
    }
  }
  // See if request is regarding the Temp Characteristic Value
else if ( ! memcmp(pAttr->type.uuid, adc_service_TempUUID, pAttr->type.len) )
  {
    if ( offset > ADC_SERVICE_TEMP_LEN )  // Prevent malicious ATT ReadBlob offsets.
    {
      status = ATT_ERR_INVALID_OFFSET;
    }
    else
    {
      *pLen = MIN(maxLen, ADC_SERVICE_TEMP_LEN - offset);  // Transmit as much as possible
      memcpy(pValue, pAttr->pValue + offset, *pLen);
    }
  }
  // See if request is regarding the VDD Characteristic Value
else if ( ! memcmp(pAttr->type.uuid, adc_service_VDDUUID, pAttr->type.len) )
  {
    if ( offset > ADC_SERVICE_VDD_LEN )  // Prevent malicious ATT ReadBlob offsets.
    {
      status = ATT_ERR_INVALID_OFFSET;
    }
    else
    {
      *pLen = MIN(maxLen, ADC_SERVICE_VDD_LEN - offset);  // Transmit as much as possible
      memcpy(pValue, pAttr->pValue + offset, *pLen);
    }
  }
  else
  {
    // If we get here, that means you've forgotten to add an if clause for a
    // characteristic value attribute in the attribute table that has READ permissions.
    *pLen = 0;
    status = ATT_ERR_ATTR_NOT_FOUND;
  }

  return status;
}


/*********************************************************************
 * @fn      adc_service_WriteAttrCB
 *
 * @brief   Validate attribute data prior to a write operation
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   len - length of data
 * @param   offset - offset of the first octet to be written
 * @param   method - type of write message
 *
 * @return  SUCCESS, blePending or Failure
 */
static bStatus_t adc_service_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                        uint8 *pValue, uint16 len, uint16 offset,
                                        uint8 method )
{
  bStatus_t status  = SUCCESS;
  uint8_t   paramID = 0xFF;

  // See if request is regarding a Client Characterisic Configuration
  if ( ! memcmp(pAttr->type.uuid, clientCharCfgUUID, pAttr->type.len) )
  {
    // Allow only notifications.
    status = GATTServApp_ProcessCCCWriteReq( connHandle, pAttr, pValue, len,
                                             offset, GATT_CLIENT_CFG_NOTIFY);
  }
  // See if request is regarding the FREQ Characteristic Value
  else if ( ! memcmp(pAttr->type.uuid, adc_service_FREQUUID, pAttr->type.len) )
  {
    if ( offset + len > ADC_SERVICE_FREQ_LEN )
    {
      status = ATT_ERR_INVALID_OFFSET;
    }
    else
    {
      // Copy pValue into the variable we point to from the attribute table.
      memcpy(pAttr->pValue + offset, pValue, len);

      // Only notify application if entire expected value is written
      if ( offset + len == ADC_SERVICE_FREQ_LEN)
        paramID = ADC_SERVICE_FREQ;
    }
  }
  // See if request is regarding the Gradient Characteristic Value
  else if ( ! memcmp(pAttr->type.uuid, adc_service_GradientUUID, pAttr->type.len) )
  {
    if ( offset + len > ADC_SERVICE_GRADIENT_LEN )
    {
      status = ATT_ERR_INVALID_OFFSET;
    }
    else
    {
      // Copy pValue into the variable we point to from the attribute table.
      memcpy(pAttr->pValue + offset, pValue, len);

      // Only notify application if entire expected value is written
      if ( offset + len == ADC_SERVICE_GRADIENT_LEN)
        paramID = ADC_SERVICE_GRADIENT;
    }
  }
  // See if request is regarding the Temp Characteristic Value
  else if ( ! memcmp(pAttr->type.uuid, adc_service_TempUUID, pAttr->type.len) )
  {
    if ( offset + len > ADC_SERVICE_TEMP_LEN )
    {
      status = ATT_ERR_INVALID_OFFSET;
    }
    else
    {
      // Copy pValue into the variable we point to from the attribute table.
      memcpy(pAttr->pValue + offset, pValue, len);

      // Only notify application if entire expected value is written
      if ( offset + len == ADC_SERVICE_TEMP_LEN)
        paramID = ADC_SERVICE_TEMP;
    }
  }
  // See if request is regarding the VDD Characteristic Value
  else if ( ! memcmp(pAttr->type.uuid, adc_service_VDDUUID, pAttr->type.len) )
  {
    if ( offset + len > ADC_SERVICE_VDD_LEN )
    {
      status = ATT_ERR_INVALID_OFFSET;
    }
    else
    {
      // Copy pValue into the variable we point to from the attribute table.
      memcpy(pAttr->pValue + offset, pValue, len);

      // Only notify application if entire expected value is written
      if ( offset + len == ADC_SERVICE_VDD_LEN)
        paramID = ADC_SERVICE_VDD;
    }
  }
  else
  {
    // If we get here, that means you've forgotten to add an if clause for a
    // characteristic value attribute in the attribute table that has WRITE permissions.
    status = ATT_ERR_ATTR_NOT_FOUND;
  }

  // Let the application know something changed (if it did) by using the
  // callback it registered earlier (if it did).
  if (paramID != 0xFF)
    if ( pAppCBs && pAppCBs->pfnChangeCb )
      pAppCBs->pfnChangeCb( paramID ); // Call app function from stack task context.

  return status;
}
