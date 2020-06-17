/*
 * Copyright (c) 2016, Texas Instruments Incorporated
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
 */

/*********************************************************************
 * INCLUDES
 */
#include <string.h>

#include <xdc/std.h>
#include <stdio.h>

//#define xdc_runtime_Log_DISABLE_ALL 1  // Add to disable logs from this file
#include <xdc/runtime/Log.h>

#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/System.h>
#include <driverlib/aon_batmon.h>
//#include "aux_adc.h"

#include "custom_fmt.h"

// Sensor Controller Interface
#include "scif.h"

// BLuetooth Developer Studio
#include "adc_service.h"

#include "project_zero.h"
#include "Board.h"
#include <ti/drivers/PIN.h>
//#include <chrono>
//#include <thread>
/*********************************************************************
 * GLOBAL VARIABLES
 */

// Ranger task Clock struct and last tick value
static uint32_t     g_rangerLastTick = 0;
static Clock_Struct g_rangerClock;

/*********************************************************************
 * LOCAL FUNCTION DECLARATIONS
 */
// HWI
static void SC_ctrlReadyHwiCb(void);
static void SC_taskAlertHwiCb(void);

// SWI
static void SC_rangerClockSwiFxn(UArg a0);

// TASK
static void SC_processAdc(void);
//static void SC_processRanger(void);
/* Pin driver handles */
static PIN_Handle ledPINHandle;

/* Global memory storage for a PIN_Config table */
static PIN_State ledPINState;


PIN_Config ledPINTable[] = {
//  Board_LED0 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MIN,
//  Board_LED1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MIN,
//  IOID_0 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MIN,
//  IOID_1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MIN,
  IOID_3 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MIN,
  IOID_4 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PULLDOWN | PIN_DRVSTR_MIN,
  IOID_8 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MIN,
  IOID_11 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MIN,
  IOID_12 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MIN,
  PIN_TERMINATE
};

/*********************************************************************
 * HWI CALLBACKS
 */

/*
 * @brief   Callback from Scif driver on Control READY interrupt.
 *
 *          Signals main task with empty msg APP_MSG_SC_CTRL_READY.
 *
 * @param   None.
 *
 * @return  None.
 */
static void SC_ctrlReadyHwiCb(void)
{
    // Signal main loop
    user_enqueueRawAppMsg(APP_MSG_SC_CTRL_READY, NULL, 0);
} // SC_ctrlReadyHwiCb


/*
 * @brief   Callback from Scif driver on Task ALERT interrupt.
 *
 *          Signals main task with empty msg APP_MSG_SC_TASK_ALERT.
 *
 * @param   None.
 *
 * @return  None.
 */
static void SC_taskAlertHwiCb(void)
{
    // Signal main loop
    user_enqueueRawAppMsg(APP_MSG_SC_TASK_ALERT, NULL, 0);
} // SC_taskAlertHwiCb


/*********************************************************************
 * SWI CALLBACKS
 */

/*
 * @brief   Callback from Clock module on timeout.
 *
 *          Stores current clock tick, and signals main task with
 *          empty msg APP_MSG_SC_EXEC_RANGER.
 *
 * @param   None.
 *
 * @return  None.
 */
static void SC_rangerClockSwiFxn(UArg a0)
{
} // SC_rangerClockSwiFxn


/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*
 * @brief   Processing function for the ADC SC task.
 *
 *          Is called whenever the APP_MSG_SC_TASK_ALERT msg is sent
 *          and ADC SC task has generated an alert.
 *
 *          Retrieves ADC value from SC, and calculates and sets new period and
 *          timeout for Clock object.
 *
 * @param   None.
 *
 * @return  None.
 */
int32_t adcData[2];
uint32_t VDDstatus;
char VDDarray[4];
char voltageArray[10];
char powerArray[10];
char freqArray[10];
uint32_t OmegaArray[5];
int32_t OmegaAve;
char OmegaAveArray[4];
int32_t Grad;
char GradArray[4];
uint32_t frequency;
uint16_t time_high; //Storage for 16 MSBs
uint16_t time_low; //Storage for 16 LSBs
uint32_t time_tot; //Merge of time_high and time_low
uint32_t rtc_Hz; //set SCS task interval
//For storing ADC parameters used in the ADC API for converting raw value to voltage
int32_t ADCGain;
int32_t ADCOffset;
int n = 0;
int connInterval = 1; // variable to control transmission rate
static void SC_processAdc(void)
{
    // enable battery monitor enable
    AONBatMonEnable();

    //Get battery voltage (this will return battery voltage in decimal form hence need to convert)
    VDDstatus = (AONBatMonBatteryVoltageGet()*125) >> 5;

    if(VDDstatus < 10){
                   itoaAppendStr(VDDarray, VDDstatus, "   ");
               }
               else if( VDDstatus < 100 &&  VDDstatus >= 10){
                   itoaAppendStr(VDDarray, VDDstatus, "  ");
               }
               else if( VDDstatus < 1000 && VDDstatus >= 100){
                   itoaAppendStr(VDDarray, VDDstatus, " ");
               }
               else{
                   itoaAppendStr(VDDarray, VDDstatus, "");
               }
               user_enqueueCharDataMsg(APP_MSG_UPDATE_CHARVAL, 0,
                                       ADC_SERVICE_SERV_UUID, ADC_SERVICE_VDD,
                                       (uint8_t *)VDDarray, strlen(VDDarray));



//temperature in millivolt
  int TEMP_Vout= scifTaskData.compHandle.output.ADCout * 4300/4096 ;
   if(TEMP_Vout < 10){
               itoaAppendStr(voltageArray, TEMP_Vout, "   ");
           }
           else if( TEMP_Vout < 100 &&  TEMP_Vout >= 10){
               itoaAppendStr(voltageArray, TEMP_Vout, "  ");
           }
           else if( TEMP_Vout < 1000 && TEMP_Vout >= 100){
               itoaAppendStr(voltageArray, TEMP_Vout, " ");
           }
           else{
               itoaAppendStr(voltageArray, TEMP_Vout, "");
           }
           user_enqueueCharDataMsg(APP_MSG_UPDATE_CHARVAL, 0,
                                   ADC_SERVICE_SERV_UUID, ADC_SERVICE_TEMP,
                                   (uint8_t *)voltageArray, strlen(voltageArray));


    //System_printf ("Millivolt: %d\n", TEMP_Vout);
   //printf("hello\n");
    //System_flush();

//    // use 2 DIO pins to create 4 cases (00, 01, 10, 11)
//    // using 4 separate pins is a waste
//    if(PIN_getOutputValue(IOID_0) == 0 && PIN_getOutputValue(IOID_1) == 0){
//        connInterval = 1;
//    }
//    else if(PIN_getOutputValue(IOID_0) == 0 && PIN_getOutputValue(IOID_1) == 1){
//        connInterval = 3;
//    }
//    else if(PIN_getOutputValue(IOID_0) == 1 && PIN_getOutputValue(IOID_1) == 0){
//        connInterval = 2;
//    }
//    else if(PIN_getOutputValue(IOID_0) == 1 && PIN_getOutputValue(IOID_1) == 1){
//        connInterval = 4;
//    }
//
//    if(n < connInterval){
//        n++;
//    }
//    // only send data when n = data transmit interval
//    if(n == connInterval){
////        // voltage
////       ADCGain = AUXADCGetAdjustmentGain (AUXADC_REF_FIXED);
////        ADCOffset = AUXADCGetAdjustmentOffset(AUXADC_REF_FIXED);
////        adcData[1] = adcData[0]; //Shift previous value
////        adcData[0] = scifTaskData.compHandle.output.ADCout; //Insert new value
////        //Adjust for gain and offset errors and convert to uV
////        adcData[0] = AUXADCAdjustValueForGainAndOffset((int32_t) adcData[0], ADCGain, ADCOffset);
////        adcData[0] = AUXADCValueToMicrovolts(AUXADC_FIXED_REF_VOLTAGE_NORMAL, (int32_t) adcData[0]);
////        if(adcData[0] < 10){
////            itoaAppendStr(voltageArray, adcData[0], "       ");
////        }
////        else if( adcData[0] < 100 &&  adcData[0] >= 10){
////            itoaAppendStr(voltageArray, adcData[0], "      ");
////        }
////        else if( adcData[0] < 1000 && adcData[0] >= 100){
////            itoaAppendStr(voltageArray, adcData[0], "     ");
////        }
////        else if( adcData[0] < 10000 && adcData[0] >= 1000){
////            itoaAppendStr(voltageArray, adcData[0], "    ");
////        }
////        else if( adcData[0] < 100000 && adcData[0] >= 10000){
////            itoaAppendStr(voltageArray, adcData[0], "   ");
////        }
////        else if( adcData[0] < 1000000 && adcData[0] >= 100000){
////            itoaAppendStr(voltageArray, adcData[0], "  ");
////        }
////        else if( adcData[0] < 10000000 && adcData[0] >= 1000000){
////            itoaAppendStr(voltageArray, adcData[0], " ");
////        }
////        else{
////            itoaAppendStr(voltageArray, adcData[0], "");
////        }
////        user_enqueueCharDataMsg(APP_MSG_UPDATE_CHARVAL, 0,
////                                ADC_SERVICE_SERV_UUID, ADC_SERVICE_VCAP,
////                                (uint8_t *)voltageArray, strlen(voltageArray));
////
////        // power
////        if(adcData[1] < 10){
////                itoaAppendStr(powerArray, adcData[1], "       ");
////        }
////        else if( adcData[1] < 100 &&  adcData[1] >= 10){
////            itoaAppendStr(powerArray, adcData[1], "      ");
////        }
////        else if( adcData[1] < 1000 && adcData[1] >= 100){
////            itoaAppendStr(powerArray, adcData[1], "     ");
////        }
////        else if( adcData[1] < 10000 && adcData[1] >= 1000){
////            itoaAppendStr(powerArray, adcData[1], "    ");
////        }
////        else if( adcData[1] < 100000 && adcData[1] >= 10000){
////            itoaAppendStr(powerArray, adcData[1], "   ");
////        }
////        else if( adcData[1] < 1000000 && adcData[1] >= 100000){
////            itoaAppendStr(powerArray, adcData[1], "  ");
////        }
////        else if( adcData[1] < 10000000 && adcData[1] >= 1000000){
////            itoaAppendStr(powerArray, adcData[1], " ");
////        }
////        else{
////            itoaAppendStr(powerArray, adcData[1], "");
////        }
////        // adcData[0] and adcData[1] needed to calculate power
////        // calculation will be done in LabView
////        user_enqueueCharDataMsg(APP_MSG_UPDATE_CHARVAL, 0,
////                                ADC_SERVICE_SERV_UUID, ADC_SERVICE_NETPWR,
////                                (uint8_t *)powerArray, strlen(powerArray));

           //obtaining omega
           PIN_setOutputValue(ledPINHandle, IOID_11, 1);

           Task_sleep(25000/Clock_tickPeriod);

    for (n=0; n<=4; n++)   {
       // PIN_setOutputValue(ledPINHandle, IOID_3, 1);

        //LED to show the that the board is powered up
        PIN_setOutputValue(ledPINHandle, IOID_4, 1);
           //frequency
        time_high = scifTaskData.compHandle.output.TimeOutHigh;
        time_low = scifTaskData.compHandle.output.TimeOutLow;
        time_tot = (time_high << 16) + time_low; //Merge high and low words

        // waiting
        Task_sleep(20000/Clock_tickPeriod);


        PIN_setOutputValue(ledPINHandle, IOID_4, 0);
        //Convert to frequency
        frequency = 24000000/time_tot ; //Time_tot is time for half a cycle, see sensor controller code
        //frequency = 48000000 / frequency;
        //float frequencyarray[n] = frequency;

       OmegaArray[n] = frequency*22/56;
        //System_printf ("radian per second is %u. \n",OmegaArray[n]);
        //System_flush();

       //over speed condition and VDD > Vstor_os_hyst
        if (OmegaArray[n] >= 500 && VDDstatus> 3200   ){

            PIN_setOutputValue(ledPINHandle, IOID_12, 1);

        }
        // over speed condition but VDD <= Vstor_os
        else if (OmegaArray[n] >= 500 && VDDstatus<=3000){
            PIN_setOutputValue(ledPINHandle, IOID_12, 0);
        }else{
            PIN_setOutputValue(ledPINHandle, IOID_12, 0);
        }
    }

            PIN_setOutputValue(ledPINHandle, IOID_11, 0);


        OmegaAve = (OmegaArray[0]+OmegaArray[1]+OmegaArray[2]+OmegaArray[3]+ OmegaArray[4])/5;
           // OmegaAve = 300;
        if (OmegaAve >= 500 && VDDstatus> 3200   ){

                 PIN_setOutputValue(ledPINHandle, IOID_12, 1);

             }
             // over speed condition but VDD <= Vstor_os
             else if (OmegaAve >= 500 && VDDstatus<=3000){
                 PIN_setOutputValue(ledPINHandle, IOID_12, 0);
             }else{
                 PIN_setOutputValue(ledPINHandle, IOID_12, 0);
             }


         if(OmegaAve < 10){
               itoaAppendStr(OmegaAveArray, OmegaAve, "   ");
               user_enqueueCharDataMsg(APP_MSG_UPDATE_CHARVAL, 0,
                                       ADC_SERVICE_SERV_UUID, ADC_SERVICE_FREQ,
                                       (uint8_t *)OmegaAveArray, strlen(OmegaAveArray));
           }
           else if(OmegaAve >= 10 && OmegaAve < 100){
               itoaAppendStr(OmegaAveArray, OmegaAve, "  ");
               user_enqueueCharDataMsg(APP_MSG_UPDATE_CHARVAL, 0,
                                       ADC_SERVICE_SERV_UUID, ADC_SERVICE_FREQ,
                                       (uint8_t *)OmegaAveArray, strlen(OmegaAveArray));
           }
           else if (OmegaAve >= 100 && OmegaAve < 1000){
               itoaAppendStr(OmegaAveArray, OmegaAve, " ");
               user_enqueueCharDataMsg(APP_MSG_UPDATE_CHARVAL, 0,
                                       ADC_SERVICE_SERV_UUID, ADC_SERVICE_FREQ,
                                       (uint8_t *)OmegaAveArray, strlen(OmegaAveArray));
           }
           else if (OmegaAve >= 1000){
               itoaAppendStr(OmegaAveArray, OmegaAve, "");
               user_enqueueCharDataMsg(APP_MSG_UPDATE_CHARVAL, 0,
                                       ADC_SERVICE_SERV_UUID, ADC_SERVICE_FREQ,
                                       (uint8_t *)OmegaAveArray, strlen(OmegaAveArray));
           }




         Grad = (-4*OmegaArray[0] -2*OmegaArray[1]+2*OmegaArray[3]+ 4*OmegaArray[4])/2000000;


                  if(Grad < 10){
                        itoaAppendStr(GradArray, Grad, "   ");
                        user_enqueueCharDataMsg(APP_MSG_UPDATE_CHARVAL, 0,
                                                ADC_SERVICE_SERV_UUID, ADC_SERVICE_GRADIENT,
                                                (uint8_t *)GradArray, strlen(GradArray));
                    }
                    else if(Grad >= 10 && Grad < 100){
                        itoaAppendStr(GradArray, Grad, "  ");
                        user_enqueueCharDataMsg(APP_MSG_UPDATE_CHARVAL, 0,
                                                ADC_SERVICE_SERV_UUID, ADC_SERVICE_GRADIENT,
                                                (uint8_t *)GradArray, strlen(GradArray));
                    }
                    else if (Grad >= 100 && Grad < 1000){
                        itoaAppendStr(GradArray, Grad, " ");
                        user_enqueueCharDataMsg(APP_MSG_UPDATE_CHARVAL, 0,
                                                ADC_SERVICE_SERV_UUID, ADC_SERVICE_GRADIENT,
                                                (uint8_t *)GradArray, strlen(GradArray));
                    }
                    else  if (Grad >= 1000){
                        itoaAppendStr(GradArray, Grad, "");
                        user_enqueueCharDataMsg(APP_MSG_UPDATE_CHARVAL, 0,
                                                ADC_SERVICE_SERV_UUID, ADC_SERVICE_GRADIENT,
                                                (uint8_t *)GradArray, strlen(GradArray));
                     }


       /* char freqArray[10];
        if(frequency >= 1380000){
            user_enqueueCharDataMsg(APP_MSG_UPDATE_CHARVAL, 0,
                                          ADC_SERVICE_SERV_UUID, ADC_SERVICE_FREQ,
                                          "0", strlen(freqArray));
        }
        else if(frequency > 40000 && frequency < 100000){
            itoaAppendStr(freqArray, frequency, "   ");
            user_enqueueCharDataMsg(APP_MSG_UPDATE_CHARVAL, 0,
                                    ADC_SERVICE_SERV_UUID, ADC_SERVICE_FREQ,
                                    (uint8_t *)freqArray, strlen(freqArray));
        }
        else if(frequency >= 100000 && frequency < 1000000){
            itoaAppendStr(freqArray, frequency, "  ");
            user_enqueueCharDataMsg(APP_MSG_UPDATE_CHARVAL, 0,
                                    ADC_SERVICE_SERV_UUID, ADC_SERVICE_FREQ,
                                    (uint8_t *)freqArray, strlen(freqArray));
        }
        else if (frequency >= 1000000 && frequency < 1380000){
            itoaAppendStr(freqArray, frequency, " ");
            user_enqueueCharDataMsg(APP_MSG_UPDATE_CHARVAL, 0,
                                    ADC_SERVICE_SERV_UUID, ADC_SERVICE_FREQ,
                                    (uint8_t *)freqArray, strlen(freqArray));
        }
*/

        // Toggle LED0 (Red) on LP
            user_toggleLED(0);
        n = 0;

}
 // SC_processAdc


/*
 * @brief   Processing function for the Ranger SC task.
 *
 *          Is called whenever the APP_MSG_SC_TASK_ALERT msg is sent
 *          and Ranger SC task has generated an alert.
 *
 *          Retrieves both high and low tdc values from SC, extends into 32-bit,
 *          converts into cm, and prints out the value on Log.
 *
 * @param   None.
 *
 * @return  None.
 */
static void SC_processRanger(void)
{
} // SC_processRanger


/*********************************************************************
 * EXTERN FUNCTIONS
 */

/*
 * @brief   Called before main loop.
 *
 *          Initializes Scif driver, registers callbacks, configures RTC, and
 *          starts SC tasks.
 *
 * @param   None.
 *
 * @return  None.
 */
void SC_init(void)
{

    ledPINHandle = PIN_open(&ledPINState, ledPINTable);
     if(!ledPINHandle) {
       Log_error0("Error initializing board LED pins");

     }
    // Insert default params
    Clock_Params clockParams;
    Clock_Params_init(&clockParams);
    // Set period to 0 ms
    clockParams.period = 0;
    // Initialize the clock object / Clock_Struct previously added globally.
    Clock_construct(&g_rangerClock,        // global clock struct
                    SC_rangerClockSwiFxn,  // callback from clock
                    0,                     // Initial delay before first timeout
                    &clockParams);         // clock parameters

    // Initialize the Sensor Controller
    scifOsalInit();
    scifOsalRegisterCtrlReadyCallback(SC_ctrlReadyHwiCb);
    scifOsalRegisterTaskAlertCallback(SC_taskAlertHwiCb);
    scifInit(&scifDriverSetup);

    uint32_t rtcHz = 1; // 1Hz RTC
    scifStartRtcTicksNow(0x00010000 / rtcHz);

    // Configure SC Tasks here, if any

    // Start Sensor Controller
    scifStartTasksNbl(BV(SCIF_COMP_HANDLE_TASK_ID));

    Log_info0 ("scTask initialization done");
} // SC_init


/*
 * @brief   Processing function for the APP_MSG_SC_CTRL_READY event.
 *
 *          Is called from main loop whenever the APP_MSG_SC_CTRL_READY msg is
 *          sent.
 *
 *          Currently does nothing.
 *
 * @param   None.
 *
 * @return  None.
 */
void SC_processCtrlReady(void)
{
  // Do nothing

} // SC_processCtrlReady


/*
 * @brief   Processing function for the APP_MSG_SC_TASK_ALERT event.
 *
 *          Is called from main loop whenever the APP_MSG_SC_TASK_ALERT msg is
 *          sent.
 *
 *          Checks which SC tasks are active, and calls their corresponding
 *          processing function. Also clears and ACKs the interrupts to the
 *          Scif driver.
 *
 * @param   None.
 *
 * @return  None.
 */
void SC_processTaskAlert(void)
{
    // Clear the ALERT interrupt source
    scifClearAlertIntSource();

    // Do SC Task processing here
    // Get the alert events
    uint32_t bvAlertEvents = scifGetAlertEvents();
    SC_processAdc();

    // Acknowledge the ALERT event
    scifAckAlertEvents();
} // SC_processTaskAlert


/*
 * @brief   Processing function for the APP_MSG_SC_EXEC_RANGER event.
 *
 *          Is called from main loop whenever the APP_MSG_SC_EXEC_RANGER msg is
 *          sent.
 *
 *          Executes the Ranger SC task once.
 *
 * @param   None.
 *
 * @return  None.
 */
void SC_execRanger(void)
{
} // SC_execRanger
