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
#include "tester_utilities.h"

#define RATE_1000MS             1000


 /****************************************************************************/
 /* Init                                                           */
 /****************************************************************************/
  void tester_utilities_init(void)
 {
     //tester_utilities_console_init();

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