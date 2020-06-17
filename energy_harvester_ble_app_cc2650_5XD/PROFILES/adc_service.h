/**********************************************************************************************
 * Filename:       adc_service.h
 *
 * Description:    This file contains the adc_service service definitions and
 *                 prototypes.
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


#ifndef _ADC_SERVICE_H_
#define _ADC_SERVICE_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "bcomdef.h"
/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
* CONSTANTS
*/
// Service UUID
#define ADC_SERVICE_SERV_UUID 0xBABE

//  Characteristic defines
#define ADC_SERVICE_FREQ      0
#define ADC_SERVICE_FREQ_UUID 0xBEEF
#define ADC_SERVICE_FREQ_LEN  4

//  Characteristic defines
#define ADC_SERVICE_GRADIENT      1
#define ADC_SERVICE_GRADIENT_UUID 0xBEF0
#define ADC_SERVICE_GRADIENT_LEN  8

//  Characteristic defines
#define ADC_SERVICE_TEMP      2
#define ADC_SERVICE_TEMP_UUID 0xBEF1
#define ADC_SERVICE_TEMP_LEN  4

//  Characteristic defines
#define ADC_SERVICE_VDD      3
#define ADC_SERVICE_VDD_UUID 0xBEF2
#define ADC_SERVICE_VDD_LEN  4

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * Profile Callbacks
 */

// Callback when a characteristic value has changed
typedef void (*adc_serviceChange_t)( uint8 paramID );

typedef struct
{
  adc_serviceChange_t        pfnChangeCb;  // Called when characteristic value changes
} adc_serviceCBs_t;



/*********************************************************************
 * API FUNCTIONS
 */


/*
 * Adc_service_AddService- Initializes the Adc_service service by registering
 *          GATT attributes with the GATT server.
 *
 */
extern bStatus_t Adc_service_AddService( void );

/*
 * Adc_service_RegisterAppCBs - Registers the application callback function.
 *                    Only call this function once.
 *
 *    appCallbacks - pointer to application callbacks.
 */
extern bStatus_t Adc_service_RegisterAppCBs( adc_serviceCBs_t *appCallbacks );

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
extern bStatus_t Adc_service_SetParameter( uint8 param, uint8 len, void *value );

/*
 * Adc_service_GetParameter - Get a Adc_service parameter.
 *
 *    param - Profile parameter ID
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 */
extern bStatus_t Adc_service_GetParameter( uint8 param, void *value );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _ADC_SERVICE_H_ */
