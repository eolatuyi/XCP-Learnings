/*
 * tester_utilities.c
 *
 *  Created on: Jul 25, 2020
 *      Author: Ebenezer O
 */
#include <stdbool.h>
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/can.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "tester_utilities.h"

#define RATE_1000MS             1000

static void tester_utilities_console_init(void);

 /****************************************************************************/
 /* Init                                                           */
 /****************************************************************************/
  void tester_utilities_init(void)
 {
     tester_utilities_console_init();

     return;
 }
 
 /****************************************************************************/
 /* Simulator thread                                                           */
 /****************************************************************************/
void tester_utilities_simulator_thread(void* arg)
{
    struct tester_utilities_stimulator_cblk* cblk = (struct tester_utilities_stimulator_cblk*)arg;
     // Establish the task's period.
     const TickType_t   period = pdMS_TO_TICKS(RATE_1000MS);
     TickType_t last_wake_time = xTaskGetTickCount();

     for (;;)
     {
         if (cblk->trigger_generator)
         {
             cblk->generator(cblk->gen_arg);
             cblk->trigger_generator = 0;
         }
        
         //TODO: No blocking
         //while(!cblk->sut_status)
         
         //cblk->transducer(cblk); 
         
         // Block until the next release time.
         vTaskDelayUntil(&last_wake_time, period);   
     }
}
 
//*****************************************************************************
//
// This function sets up UART0 to be used for a console to display information
// as the example is running.
//
//*****************************************************************************
static void tester_utilities_console_init(void)
{
    //
    // Enable GPIO port A which is used for UART0 pins.
    // TODO: change this to whichever GPIO port you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Configure the pin muxing for UART0 functions on port A0 and A1.
    // This step is not necessary if your part does not support pin muxing.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    //
    // Enable UART0 so that we can configure the clock.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Select the alternate (UART) function for these pins.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}
